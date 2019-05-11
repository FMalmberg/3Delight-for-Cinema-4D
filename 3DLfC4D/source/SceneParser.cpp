#include "SceneParser.h"
#include "DL_API.h"
#include "DL_TypeConversions.h"
#include "PluginManager.h"
#include "IDs.h"
#include "nsi.hpp"
#include "myres.h"
#include <vector>
#include <stdint.h>
#include <string>
#include <sstream>      
#include <algorithm>
#include "../../core.framework/source/maxon/stringencoding.h"
#include <math.h>

// std::stringstream, std::stringbuf
using namespace std;

void NSIErrorHandlerC4D(void *userdata, int level, int code, const char *message)
{
	const char *pre = (const char *)userdata;
	std::string buffer("3Delight");

	if (pre)
	{
		buffer += " (" + std::string(pre) + "): ";
	}
	else
		buffer += ": ";

	buffer += message;

	switch (level)
	{
	case NSIErrMessage:
		ApplicationOutput(buffer.c_str());
		break;
	case NSIErrInfo:
		ApplicationOutput(buffer.c_str());
		break;
	case NSIErrWarning:
		ApplicationOutput(buffer.c_str());
		break;
	default:
	case NSIErrError:
		ApplicationOutput(buffer.c_str());
		break;
	}
}


void StoppedCallback(
	void* stoppedcallbackdata,
	NSIContext_t ctx,
	int status){
	
	int* data = (int*)stoppedcallbackdata;
	*data = status;
}

extern PluginManager PM;

char* null_shader="";

void RenderProgress( float i_progress, void *data ){
	float* progress=(float*)data;
	*progress=i_progress;
}





vector<float> GetSamples(double open, double close, int samples){
	vector<float> values(samples);
	if(samples==1){
		values[0]=open;
	}
	else{ 
		double factor=(close-open)/(samples-1);
		for(int i=0; i<samples; i++){
			values[i]=open+i*factor;
		}
	}
	return values;
}
SceneParser::~SceneParser(){}

SceneParser::SceneParser(){
	context_handle=NSI_BAD_CONTEXT;
	
	doc = NULL;

	//settings=NULL;
	//stage=NONE;

	rendermode = PREVIEW_RENDER;
	nMotionSamples=1;
	shutterOpen=0;
	shutterClose=0;
	shutterTime = 0;
	//ExportAOVs=false;

	UpdateMotionSampleTimes();
}

SceneParser::SceneParser(BaseDocument* document, NSIContext_t context) {
	context_handle = context;
	doc = document;
	rendermode = PREVIEW_RENDER;
}

void SceneParser::RequestMinimumShutterTime(float t){
	if (t > shutterTime){
		shutterTime = t;
	}
}

RENDER_MODE SceneParser::GetRenderMode(){
	return rendermode;
}

NSIContext_t SceneParser::GetContext(){
	return context_handle;
}

/*void SceneParser::SetAOVExport(bool export_aovs){
	ExportAOVs=export_aovs;
}*/

void SceneParser::FillRenderSettings() {
	if (!doc) { return; }

	RenderData* rd = doc->GetActiveRenderData();
	BaseContainer* render_data = rd->GetDataInstance();
	BaseVideoPost* vp = rd->GetFirstVideoPost();

	bool has_vp = false;
	while (vp != NULL && !has_vp) {
		has_vp = (vp->GetType() == ID_RENDERSETTINGS); //Look for rendersettings
		if (!has_vp) {
			vp = vp->GetNext();
		}
	}

	if (!has_vp) { //If there are not rendersettings in the document, create them
		BaseVideoPost* pvp = BaseVideoPost::Alloc(ID_RENDERSETTINGS);
		rd->InsertVideoPostLast(pvp);

		vp = rd->GetFirstVideoPost();
		while (vp != NULL && !has_vp) {
			has_vp = (vp->GetType() == ID_RENDERSETTINGS); //Look for rendersettings
			if (!has_vp) {
				vp = vp->GetNext();
			}
		}

	}

	if (has_vp) {
		settings = vp->GetData();
	}
	
}

bool SceneParser::InitScene(bool animate, long frame) {
	if (!doc) { return false; }

	FillRenderSettings();

	//Motion sampling settings
	bool useMotionBlur = settings.GetBool(DL_MOTION_BLUR);
	long motionSamples = 2; // settings.GetInt32(DL_MOTION_SAMPLES, 2);
	if (!useMotionBlur) { motionSamples = 1; }

	float ShutterAngle = settings.GetFloat(DL_SHUTTER_ANGLE,0.5);
	long fps = doc->GetFps();
	float shutterOpen_t = float(frame) / float(fps);
	float shutterClose_t = float(frame + ShutterAngle) / float(fps);

	SetMotionSamples(motionSamples);
	SetShutter(shutterOpen_t, shutterClose_t);

	if (animate) {
		AnimateDoc(doc, shutterOpen);
	}

	//Clear previous scene info
	transforms.clear();
	nodes.clear();
	hooks.clear();

	//Create nodes and transforms

	//Traverse objects
	TraverseScene(doc->GetFirstObject(), doc, ".root");

	//Traverse materials 
	BaseMaterial* mat = doc->GetFirstMaterial();
	while (mat) {
		//Create translator for material
		Node n(mat);
		DL_Translator* translator = n.GetTranslator();
		if (translator) {
			nodes.push_back(n);
			translator->CreateNSINodes("", mat, doc, this);
		}

		//Traverse sub-shaders
		BaseShader* shader = mat->GetFirstShader();
		TraverseShaders(shader, doc);

		mat = mat->GetNext();
	}

	//Create hooks
	hooks = PM.GetHooks();
	for (long i = 0; i<hooks.size(); i++) {
		hooks[i]->CreateNSINodes(doc, this);
	}

	//Make connections
	for (long i = 0; i<nodes.size(); i++) {
		DL_Translator* translator = nodes[i].GetTranslator();
		if (translator) {
			translator->ConnectNSINodes(nodes[i].GetC4DNode(doc), doc, this);
		}
	}

	for (long i = 0; i<hooks.size(); i++) {
		hooks[i]->ConnectNSINodes(doc, this);
	}

	return true;

}

void SceneParser::SampleFrameMotion() {
	DL_SampleInfo info;
	info.samples_max = nMotionSamples;
	info.shutter_open_time = shutterOpen;
	info.shutter_close_time = shutterClose;

	for (long s = 0; s<nMotionSamples; s++) {

		info.sample = s;
		info.sample_time = motionSampleTimes[s];

		AnimateDoc(doc, motionSampleTimes[s]);
		for (long i = 0; i<hooks.size(); i++) {
			hooks[i]->SampleMotion(&info, doc, this);
		}

		for (long i = 0; i<transforms.size(); i++) {
			transforms[i].SampleMotion(&info, doc, this);
		}

		DL_Translator* translator;
		for (long i = 0; i<nodes.size(); i++) {
			translator = nodes[i].GetTranslator();
			BaseList2D* c4d_node = nodes[i].GetC4DNode(doc);
			if (translator && c4d_node) {
				translator->SampleMotion(&info, nodes[i].GetC4DNode(doc), doc, this);
			}
		}

		//SampleMotion(i, doc);
	}


}

bool SceneParser::Parse(BaseDocument* document, long frame){
	if (!doc){ return false; }

	doc = doc;

	RenderData* rd = doc->GetActiveRenderData();
	BaseContainer* render_data = rd->GetDataInstance();
	BaseVideoPost* vp = rd->GetFirstVideoPost();
	

	bool has_vp = false;
	while (vp != NULL && !has_vp){
		has_vp = (vp->GetType() == ID_RENDERSETTINGS); //Look for rendersettings
		if (!has_vp){
			vp = vp->GetNext();
		}
	}
	


	//settings = NULL;
	//if (has_vp){ settings = vp->GetDataInstance(); }
	//if (settings == NULL){ return false; }



	NSI::Context context;

	NSIErrorHandler_t eh = NSIErrorHandlerC4D;

	NSIParam_t param[1];
	param[0].name = "errorhandler";
	param[0].data = &eh;
	param[0].type = NSITypePointer; param[0].count = 1;
	param[0].flags = 0; param[0].arraylength = 0;
	
	if(settings.GetString(DL_ISCLICKED) == "Render")
	context.Begin(NSI::PointerArg("errorhandler", NSIErrorHandlerC4D));

	else if (settings.GetString(DL_ISCLICKED) == "Export") {
		String flnm = settings.GetFilename(DL_FOLDER_OUTPUT).GetString();
		std::string exported = flnm.GetCStringCopy();
		context.Begin(NSI::StringArg("streamfilename", exported.c_str()));
	}

	context_handle = context.Handle();

	//Motion sampling settings
	bool useMotionBlur=settings.GetBool(DL_USE_MOTION_BLUR);
	long motionSamples=settings.GetInt32(DL_MOTION_SAMPLES,2);
	if(!useMotionBlur){ motionSamples=1; }

	float ShutterAngle=settings.GetFloat(DL_SHUTTER_ANGLE);
	long fps=doc->GetFps();
	float shutterOpen_t=float(frame)/float(fps);
	float shutterClose_t=float(frame+ShutterAngle)/float(fps);

	SetMotionSamples(motionSamples);
	SetShutter(shutterOpen_t,shutterClose_t);

	AnimateDoc(doc,shutterOpen);


	//stage=BUILD_SCENE_TREE;
	
	//Create nodes and transforms
	transforms.clear();
	TraverseScene(doc->GetFirstObject(), doc, ".root");

	//Traverse materials 
	BaseMaterial* mat = doc->GetFirstMaterial();
	while (mat){
		//Create translator for material
		Node n(mat);
		DL_Translator* translator = n.GetTranslator();
		if (translator){
			nodes.push_back(n);
			translator->CreateNSINodes("", mat, doc, this);
		}

		//Traverse sub-shaders
		BaseShader* shader = mat->GetFirstShader();
		TraverseShaders(shader, doc);

		mat = mat->GetNext();
	}

	//Create hooks
	hooks=PM.GetHooks();
	for(long i=0; i<hooks.size(); i++){
		hooks[i]->CreateNSINodes(doc,this);
	}

	//Make connections
	for(long i=0; i<nodes.size(); i++){
		DL_Translator* translator=nodes[i].GetTranslator();
		if(translator){
			translator->ConnectNSINodes(nodes[i].GetC4DNode(doc), doc, this);		
		}
	}

	for(long i=0; i<hooks.size(); i++){
		hooks[i]->ConnectNSINodes(doc,this);
	}

	for(long i=0; i<nMotionSamples; i++){
		if(i!=0){
			AnimateDoc(doc,motionSampleTimes[i]);
		}
		SampleMotion(i, doc);
	}

	hooks.clear();
	transforms.clear();
	 

	int renderstatus=-1;

	//context.RenderControl();
	context.RenderControl((
		NSI::StringArg("action", "start"), 
		NSI::PointerArg("stoppedcallback", (void*)&StoppedCallback),
		NSI::PointerArg("stoppedcallbackdata", (void*)&renderstatus)
		));

	context.RenderControl((
		NSI::StringArg("action", "wait")
		));

	context_handle=NSI_BAD_CONTEXT;
	//stage=NONE;
	//return (progress>99.9); 
	return (renderstatus == NSIRenderCompleted);
}



void SceneParser::SetRenderMode(RENDER_MODE mode){
	rendermode = mode;
}


void SceneParser::SetMotionSamples(int samples){
	nMotionSamples=samples;
	UpdateMotionSampleTimes();
}

void SceneParser::SetShutter(double open, double close){
	shutterOpen=open;
	shutterClose=close;
	UpdateMotionSampleTimes();
}

float* SceneParser::GetMotionSampleTimes(){
	return &motionSampleTimes[0];
}

float SceneParser::GetShutterOpen(){
	return shutterOpen;
}

float SceneParser::GetShutterClose(){
	return shutterClose;
}

int SceneParser::GetMotionSamples(){
	return nMotionSamples;
}

BaseContainer* SceneParser::GetSettings(){
	return &settings;
}

void SceneParser::UpdateMotionSampleTimes(){
	motionSampleTimes=GetSampleTimes(nMotionSamples);
}

vector<float> SceneParser::GetSampleTimes(int nsamples){
	vector<float> times(nsamples); 

	if(nsamples==1){
		times[0]=shutterOpen;
	}
	else{
		for(int i=0; i<nsamples; i++){
			float t=float(i)/float(nsamples-1);
			times[i]=(1-t)*shutterOpen + t*shutterClose;
		}
	}

	return times;
}

std::string SceneParser::GetIDString(BaseList2D* node){
	Int memsize=0;
	const Char* mem;
	node->FindUniqueID(MAXON_CREATOR_ID, mem,  memsize);
	String ID_STR;
	ID_STR.SetCString(mem,memsize);
	std::string result=StringToStdString(ID_STR);
	return result;

}

const char* SceneParser::GetUniqueName(char* basename){
	string s(basename);
	names[s]++;
	unsigned long long n = names[s];
	
	stringstream ss;
	ss << s << "_" << n;
	name = ss.str();
	return name.c_str();
}

void SceneParser::SetAssociatedHandle(BaseList2D* node, const char* handle){
	if(!node){
		return;
	}

	std::string node_ID=GetIDString(node);
	HandleMap[node_ID]=std::string(handle);
}

const char* SceneParser::GetAssociatedHandle(BaseList2D* node){
	auto it=HandleMap.find(GetIDString(node));

	if(it==HandleMap.end()){
		return "";
	}
	return it->second.c_str();
}


void SceneParser::SampleMotion(long s, BaseDocument* doc){
	
	for(long i=0; i<hooks.size(); i++){
		//hooks[i]->SampleMotion(motionSampleTimes[s], s,doc,this);
	}
	
	for(long i=0; i<transforms.size(); i++){
		//transforms[i].SampleMotion(motionSampleTimes[s],s,doc,this);
	}

	DL_Translator* translator;
	for(long i=0; i<nodes.size(); i++){
		translator=nodes[i].GetTranslator();
		BaseList2D* c4d_node = nodes[i].GetC4DNode(doc);
		if(translator && c4d_node){ 
			//translator->SampleMotion(motionSampleTimes[s], s,nodes[i].GetC4DNode(doc), doc,this);
		}
	}
}

void SceneParser::AnimateDoc(BaseDocument* doc,double t){
	doc->SetTime(BaseTime(t));
	doc->ExecutePasses(0,true,true,true, BUILDFLAGS::EXTERNALRENDERER);
}

void SceneParser::TraverseShaders(BaseShader* shader, BaseDocument* doc){
	if (!shader) { return; }

	//Create translator node for shader
	Node n(shader);
	DL_Translator* translator = n.GetTranslator();
	if (translator){
		nodes.push_back(n);
		translator->CreateNSINodes("", shader, doc, this);
	}

	TraverseShaders(shader->GetDown(),doc);
	TraverseShaders(shader->GetNext(),doc);
}

void SceneParser::TraverseScene(BaseObject* obj, BaseDocument* doc,std::string parent_transform, bool visible){
	if(!obj){return;}
	
	bool obj_visible=visible;
	long rendermode=obj->GetRenderMode();
	if(rendermode==MODE_ON){
		obj_visible=true;
	}
	else if(rendermode==MODE_OFF){
		obj_visible=false;
	}

	Transform t(obj, this, parent_transform);
	std::string transform_handle=t.GetHandle();
	transforms.push_back(t);

	if(obj_visible){
		Node n(obj);

		DL_Translator* translator=n.GetTranslator();
		if(translator){
			nodes.push_back(n);
			translator->CreateNSINodes(transform_handle.c_str(), obj, doc, this);
		}
	}

	//Handle tags
	BaseTag* tag=obj->GetFirstTag();
	while(tag){
		Node tagnode(tag);
		DL_Translator* tagtranslator=tagnode.GetTranslator();
		if(tagtranslator){
			nodes.push_back(tagnode);
			tagtranslator->CreateNSINodes(transform_handle.c_str(), tag, doc, this);
		}
		tag=tag->GetNext();
	}

	BaseShader* shader = obj->GetFirstShader();
	TraverseShaders(shader, doc);

	TraverseScene(obj->GetDown(),doc,transform_handle, obj_visible);
	TraverseScene(obj->GetNext(), doc, parent_transform, visible);

}
