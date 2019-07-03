#include "SceneParser.h"
#include "DL_API.h"
#include "DL_TypeConversions.h"
#include "PluginManager.h"
#include "tmotionsamples.h"
#include "IDs.h"
#include "nsi.hpp"
#include "dlrendersettings.h"
#include <vector>
#include <stdint.h>
#include <string>
#include <sstream>      
#include <algorithm>
#include "../../core.framework/source/maxon/stringencoding.h"
#include <math.h>
#include <regex>


// std::stringstream, std::stringbuf
using namespace std;


void StoppedCallback(
	void* stoppedcallbackdata,
	NSIContext_t ctx,
	int status) {

	int* data = (int*)stoppedcallbackdata;
	*data = status;
}

extern PluginManager PM;

char* null_shader = "";

void RenderProgress(float i_progress, void *data) {
	float* progress = (float*)data;
	*progress = i_progress;
}





vector<float> GetSamples(double open, double close, int samples) {
	vector<float> values(samples);
	if (samples == 1) {
		values[0] = open;
	}
	else {
		double factor = (close - open) / (samples - 1);
		for (int i = 0; i < samples; i++) {
			values[i] = open + i * factor;
		}
	}
	return values;
}
SceneParser::~SceneParser() {}

SceneParser::SceneParser() {
	context_handle = NSI_BAD_CONTEXT;

	doc = NULL;

	//settings=NULL;
	//stage=NONE;

	rendermode = PREVIEW_RENDER;
	nMotionSamples = 1;
	//shutterOpen=0;
	//shutterClose=0;
	//shutterTime = 0;
	//ExportAOVs=false;

	//UpdateMotionSampleTimes();
}

SceneParser::SceneParser(BaseDocument* document, NSIContext_t context) {
	context_handle = context;
	doc = document;
	rendermode = PREVIEW_RENDER;
}

/*void SceneParser::RequestMinimumShutterTime(float t){
	if (t > shutterTime){
		shuttehrTime = t;
	}
}*/

void SceneParser::GetNodesAndTransforms(BaseDocument* doc, std::vector<Node>& nodes, std::vector<Transform>& transforms) {
	nodes.clear();
	transforms.clear();

	//Traverse objects (and their tags) 
	HierarchyData hdata;
	hdata.parent_transform = ".root";
	hdata.cachePos = 0;
	hdata.isVisible = true;
	hdata.deformationSamples = 2;
	hdata.transformSamples = 2;

	TraverseObjects(doc->GetFirstObject(), doc, hdata, nodes, transforms);

	//Traverse materials 
	BaseMaterial* mat = doc->GetFirstMaterial();

	HierarchyData mat_hdata;
	mat_hdata.cachePos = 0;
	mat_hdata.deformationSamples = 0;
	mat_hdata.transformSamples = 0;
	mat_hdata.parent_transform = "";
	mat_hdata.isVisible = true;

	while (mat) {
		//Create node for material
		Node n(mat);
		n.hdata = mat_hdata;
		n.handle = GetHandleName((BaseList2D*)mat);
		DL_Translator* translator = n.GetTranslator();
		if (translator) {
			nodes.push_back(n);

			//Do this later
			//string mat_handle = GetHandleName((BaseList2D*)mat);
			//translator->Init(mat, doc, this);
			//translator->CreateNSINodes(mat_handle.c_str(), "", mat, doc, this);
		}

		//Traverse sub-shaders
		BaseShader* shader = mat->GetFirstShader();
		TraverseShaders(shader, doc, nodes);

		mat = mat->GetNext();
	}

}

RENDER_MODE SceneParser::GetRenderMode() {
	return rendermode;
}

NSIContext_t SceneParser::GetContext() {
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
	//ApplicationOutput("Init scene");

	FillRenderSettings();

	//Motion sampling settings
	//bool useMotionBlur = settings.GetBool(DL_MOTION_BLUR);
	//long motionSamples = 2; // settings.GetInt32(DL_MOTION_SAMPLES, 2);
	//if (!useMotionBlur) { motionSamples = 1; }

	//float ShutterAngle = settings.GetFloat(DL_SHUTTER_ANGLE,0.5);
	long fps = doc->GetFps();
	float shutterOpen_t = float(frame) / float(fps);
	//float shutterClose_t = float(frame + ShutterAngle) / float(fps);

	//SetMotionSamples(motionSamples);
	//SetShutter(shutterOpen_t, shutterClose_t);

	if (animate) {
		AnimateDoc(doc, shutterOpen_t);
	}





	//Clear previous scene info
	//transforms.clear();
	//nodes.clear();
	//hooks.clear();



	//Traverse objects
	//TraverseScene(doc->GetFirstObject(), doc, ".root");

	//Traverse materials 
	/*BaseMaterial* mat = doc->GetFirstMaterial();
	while (mat) {
		//Create translator for material
		Node n(mat);
		DL_Translator* translator = n.GetTranslator();
		if (translator) {
			nodes.push_back(n);
			string mat_handle = GetHandleName((BaseList2D*)mat);
			translator->Init(mat, doc, this);
			translator->CreateNSINodes(mat_handle.c_str(),"", mat, doc, this);
		}
		//Traverse sub-shaders
		BaseShader* shader = mat->GetFirstShader();
		TraverseShaders(shader, doc);
		mat = mat->GetNext();
	}*/

	//Create nodes and transforms
	vector<Node> nodes;
	vector<Transform> transforms;
	GetNodesAndTransforms(doc, nodes, transforms);

	//Create hooks
	vector<DL_HookPtr> hooks = PM.GetHooks();

	//--------------------------------------//
	//Create NSI nodes 
	//--------------------------------------//

	//Transforms
	for (long i = 0; i < transforms.size(); i++) {
		transforms[i].CreateNodes(doc, this);
	}

	//Nodes (after transforms -important!)
	for (long i = 0; i < nodes.size(); i++) {
		DL_Translator* translator = nodes[i].GetTranslator();
		if (translator) {
			HierarchyData hdata = nodes[i].hdata;
			translator->Init(nodes[i].GetC4DNode(), doc, this);
			translator->CreateNSINodes(nodes[i].handle.c_str(), hdata.parent_transform.c_str(), nodes[i].GetC4DNode(), doc, this);
		}
	}



	//Hooks
	for (long i = 0; i < hooks.size(); i++) {
		hooks[i]->Init(doc, this);
		hooks[i]->CreateNSINodes(doc, this);
	}

	//--------------------------------------//
	//Make connections
	//--------------------------------------//

	//Nodes
	for (long i = 0; i < nodes.size(); i++) {
		DL_Translator* translator = nodes[i].GetTranslator();
		if (translator) {
			//string handle = GetHandleName(nodes[i].GetC4DNode());
			translator->ConnectNSINodes(nodes[i].handle.c_str(), nodes[i].hdata.parent_transform.c_str(), nodes[i].GetC4DNode(), doc, this);
		}
	}

	//Hooks
	for (long i = 0; i < hooks.size(); i++) {
		hooks[i]->ConnectNSINodes(doc, this);
	}

	return true;

}

void SceneParser::SampleFrameMotion(long frame) {


	FillRenderSettings();

	//Motion sampling settings
	bool useMotionBlur = settings.GetBool(DL_MOTION_BLUR);
	long motionSamples = 2; // settings.GetInt32(DL_MOTION_SAMPLES, 2);
	if (!useMotionBlur) { motionSamples = 1; }

	float ShutterAngle = 0.5; //Get from camera instead
	long fps = doc->GetFps();
	float shutterOpen_t = float(frame) / float(fps);
	float shutterClose_t = (float(frame) + ShutterAngle) / float(fps);


	DL_SampleInfo info;
	info.samples_max = motionSamples;
	info.shutter_open_time = shutterOpen_t;
	info.shutter_close_time = shutterClose_t;

	//Create nodes and transforms
	vector<Node> nodes;
	vector<Transform> transforms;


	//Create hooks
	vector<DL_HookPtr> hooks = PM.GetHooks();
	for (long i = 0; i < hooks.size(); i++) {
		hooks[i]->Init(doc, this);
	}

	for (long s = 0; s < motionSamples; s++) {

		float t = 0;
		if (motionSamples > 1) {
			t = float(s) / float(motionSamples - 1);
		}

		info.sample = s;
		info.sample_time = t * shutterOpen_t + (1 - t)*shutterClose_t;

		AnimateDoc(doc, info.sample_time);
		GetNodesAndTransforms(doc, nodes, transforms);

		for (long i = 0; i < hooks.size(); i++) {
			hooks[i]->SampleAttributes(&info, doc, this);
		}

		for (long i = 0; i < transforms.size(); i++) {

			transforms[i].SampleAttributes(&info, doc, this);
			//ApplicationOutput("Transform samples: " + String::IntToString(transforms[i].motionsSamples));
		}

		DL_Translator* translator;
		for (long i = 0; i < nodes.size(); i++) {
			translator = nodes[i].GetTranslator();
			BaseList2D* c4d_node = nodes[i].GetC4DNode();
			if (translator && c4d_node) {
				//ApplicationOutput(c4d_node->GetName() + " deformation samples: " + String::IntToString(nodes[i].hdata.deformationSamples));

				//string handle = GetHandleName(c4d_node);
				translator->SampleAttributes(&info, nodes[i].handle.c_str(), nodes[i].GetC4DNode(), doc, this);
			}
		}
	}



}



bool SceneParser::IsDirty(Node* n) {
	if (!n) { return false; }
	BaseList2D* c4d_node = n->GetC4DNode();
	if (!c4d_node) { return false; }

	bool dirty = false;

	/*if (dirtystates.empty()) {
		ApplicationOutput("empty state");
	}*/

	Int64 dirty_checksum = c4d_node->GetDirty(DIRTYFLAGS::ALL);
	std::map<string, Int64>::iterator it = dirtystates.find(n->handle);

	if (it == dirtystates.end()) {
		dirty = true;
	}
	else if (it->second != dirty_checksum) {
		dirty = true;
	}


	dirtystates[n->handle] = dirty_checksum; //Touch this node

	return dirty;
}


void SceneParser::InteractiveUpdate() {

	//Create nodes and transforms
	vector<Node> nodes;
	vector<Transform> transforms;

	GetNodesAndTransforms(doc, nodes, transforms);

	//Create hooks
	vector<DL_HookPtr> hooks = PM.GetHooks();
	for (long i = 0; i < hooks.size(); i++) {
		hooks[i]->Init(doc, this);
	}



	DL_SampleInfo info;
	info.sample = 0;
	info.samples_max = 1;
	info.shutter_open_time = 0;
	info.shutter_close_time = 0;
	info.sample_time = 0;

	for (long i = 0; i < hooks.size(); i++) {
		hooks[i]->SampleAttributes(&info, doc, this);
	}

	for (long i = 0; i < transforms.size(); i++) {
		transforms[i].SampleAttributes(&info, doc, this);
	}

	DL_Translator* translator;
	for (long i = 0; i < nodes.size(); i++) {


		BaseList2D* c4d_node = nodes[i].GetC4DNode();
		//Int64 new_dirtyval = Int64(c4d_node->GetDirty(DIRTYFLAGS::ALL));
		//if (nodes[i].dirty_checksum != new_dirtyval) {
			//nodes[i].dirty_checksum = new_dirtyval;
		if (IsDirty(&nodes[i])) {
			translator = nodes[i].GetTranslator();
			if (translator && c4d_node) {
				//string handle = GetHandleName(c4d_node);
				translator->Init(nodes[i].GetC4DNode(), doc, this);
				translator->SampleAttributes(&info, nodes[i].handle.c_str(), nodes[i].GetC4DNode(), doc, this);
			}
		}
		//}
	}

}

/*
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
	if (settings.GetString(DL_ISCLICKED) == "Export") {
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
	//transforms.clear();
	//TraverseScene(doc->GetFirstObject(), doc, ".root");
	//Traverse materials
	BaseMaterial* mat = doc->GetFirstMaterial();
	while (mat){
		//Create translator for material
		Node n(mat);
		DL_Translator* translator = n.GetTranslator();
		if (translator){
			nodes.push_back(n);
			translator->CreateNSINodes("","", mat, doc, this);
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
			translator->ConnectNSINodes("",nodes[i].GetC4DNode(doc), doc, this);
		}
	}
	for(long i=0; i<hooks.size(); i++){
		hooks[i]->ConnectNSINodes(doc,this);
	}
	for(long i=0; i<nMotionSamples; i++){
		if(i!=0){
			AnimateDoc(doc,motionSampleTimes[i]);
		}
		//SampleAttributes(i, doc);
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
*/


void SceneParser::SetRenderMode(RENDER_MODE mode) {
	rendermode = mode;
}


/*void SceneParser::SetMotionSamples(int samples){
	nMotionSamples=samples;
	UpdateMotionSampleTimes();
}*/

/*void SceneParser::SetShutter(double open, double close){
	shutterOpen=open;
	shutterClose=close;
	UpdateMotionSampleTimes();
}*/

/*float* SceneParser::GetMotionSampleTimes(){
	return &motionSampleTimes[0];
}*/

/*float SceneParser::GetShutterOpen(){
	return shutterOpen;
}*/

/*float SceneParser::GetShutterClose(){
	return shutterClose;
}
int SceneParser::GetMotionSamples(){
	return nMotionSamples;
}*/

BaseContainer* SceneParser::GetSettings() {
	return &settings;
}

/*void SceneParser::UpdateMotionSampleTimes(){
	motionSampleTimes=GetSampleTimes(nMotionSamples);
}*/

/*vector<float> SceneParser::GetSampleTimes(int nsamples){
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
}*/

const char* SceneParser::GetHandleName(BaseList2D* node) {
	Int memsize = 0;
	const Char* mem;
	node->FindUniqueID(MAXON_CREATOR_ID, mem, memsize);
	String ID_STR;
	ID_STR.SetCString(mem, memsize);
	std::string ID_string = StringToStdString(ID_STR) + string("::");
	handle.clear();
	for (int i = 0; i < ID_string.size(); i++) {
		handle = handle + to_string((int)reinterpret_cast<unsigned char &>(ID_string[i]));
	}
	handle = handle + string("::");
	return handle.c_str();
}

/*std::string SceneParser::GetIDString(BaseList2D* node){
	Int memsize=0;
	const Char* mem;
	node->FindUniqueID(MAXON_CREATOR_ID, mem,  memsize);
	String ID_STR;
	ID_STR.SetCString(mem,memsize);
	std::string result=StringToStdString(ID_STR);
	return result;
}*/

/*
const char* SceneParser::GetUniqueName(char* basename){
	string s(basename);
	names[s]++;
	unsigned long long n = names[s];

	stringstream ss;
	ss << s << "_" << n;
	name = ss.str();
	return name.c_str();
}*/

/*
void SceneParser::SetAssociatedHandle(BaseList2D* node, const char* handle){
	if(!node){
		return;
	}
	std::string node_ID=GetIDString(node);
	HandleMap[node_ID]=std::string(handle);
}*/

/*
const char* SceneParser::GetAssociatedHandle(BaseList2D* node){
	auto it=HandleMap.find(GetIDString(node));
	if(it==HandleMap.end()){
		return "";
	}
	return it->second.c_str();
}*/


/*void SceneParser::SampleMotion(long s, BaseDocument* doc){

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
}*/

void SceneParser::AnimateDoc(BaseDocument* doc, double t) {
	doc->SetTime(BaseTime(t));
	doc->ExecutePasses(0, true, true, true, BUILDFLAGS::EXTERNALRENDERER);
}

void SceneParser::TraverseShaders(BaseShader* shader, BaseDocument* doc, vector<Node>& nodes) {
	if (!shader) { return; }

	//Create translator node for shader
	Node n(shader);
	DL_Translator* translator = n.GetTranslator();
	if (translator) {
		//Fill hierarchy data
		n.hdata.cachePos = 0;
		n.hdata.isVisible = true;
		n.hdata.deformationSamples = 0;
		n.hdata.transformSamples = 0;
		n.hdata.parent_transform = "";

		n.handle = GetHandleName(shader);

		nodes.push_back(n);

		//string handle = GetHandleName(shader);
		//translator->Init(shader, doc, this);
		//translator->CreateNSINodes(handle.c_str(), "",shader, doc, this);
	}

	TraverseShaders(shader->GetDown(), doc, nodes);
	TraverseShaders(shader->GetNext(), doc, nodes);
}

void SceneParser::TraverseObjects(BaseObject* obj, BaseDocument* doc, HierarchyData hdata, std::vector<Node>& nodes, std::vector<Transform>& transforms) {
	if (!obj) { return; }

	bool obj_visible = hdata.isVisible;
	long rendermode = obj->GetRenderMode();
	if (rendermode == MODE_ON) {
		obj_visible = true;
	}
	else if (rendermode == MODE_OFF) {
		obj_visible = false;
	}

	int transform_samples = hdata.transformSamples;
	int deformation_samples = hdata.deformationSamples;

	BaseTag* motionblur_tag = obj->GetTag(ID_DL_MOTIONBLURTAG);
	if (motionblur_tag) {
		BaseContainer* mbdata = motionblur_tag->GetDataInstance();
		transform_samples = mbdata->GetInt32(TRANSFORMATION_EXTRA_SAMPLES) + 2;
		if (mbdata->GetBool(USE_TRANSFORMATION_BLUR) == false) {
			transform_samples = 1;
		}

		deformation_samples = mbdata->GetInt32(DEFORMATION_EXTRA_SAMPLES) + 2;
		if (mbdata->GetBool(USE_DEFORMATION_BLUR) == false) {
			deformation_samples = 1;
		}
	}

	string object_handle = string(GetHandleName((BaseList2D*)obj)); //Change this later: dress ID with cachepos for virtual objects

	Transform t(object_handle, obj, hdata.parent_transform);
	t.motionsSamples = transform_samples;
	std::string transform_handle = t.GetHandle();
	transforms.push_back(t);

	HierarchyData n_hdata = hdata;
	n_hdata.deformationSamples = deformation_samples;
	n_hdata.transformSamples = transform_samples;
	n_hdata.parent_transform = transform_handle;
	n_hdata.isVisible = obj_visible;

	if (obj_visible) {
		Node n(obj);
		n.hdata = n_hdata;
		n.handle = object_handle;

		DL_Translator* translator = n.GetTranslator();
		if (translator) {
			nodes.push_back(n);


			//Do this later, only store the node object for now
			//translator->Init(obj, doc, this);
			//translator->CreateNSINodes(object_handle.c_str(), transform_handle.c_str(), obj, doc, this);
		}
	}

	//Handle tags
	BaseTag* tag = obj->GetFirstTag();
	while (tag) {
		Node tagnode(tag);
		tagnode.hdata = n_hdata;
		string tag_handle = string(GetHandleName((BaseList2D*)tag));
		tagnode.handle = tag_handle;
		DL_Translator* tagtranslator = tagnode.GetTranslator();
		if (tagtranslator) {
			nodes.push_back(tagnode);

			//tagtranslator->Init(tag, doc, this);
			//tagtranslator->CreateNSINodes(tag_handle.c_str(), transform_handle.c_str(), tag, doc, this);
		}
		tag = tag->GetNext();
	}

	TraverseObjects(obj->GetDown(), doc, n_hdata, nodes, transforms);
	TraverseObjects(obj->GetNext(), doc, hdata, nodes, transforms);


	//TraverseScene(obj->GetDown(), doc, transform_handle, obj_visible);
	//TraverseScene(obj->GetNext(), doc, parent_transform, visible);

}

/*void SceneParser::TraverseScene(BaseObject* obj, BaseDocument* doc,std::string parent_transform, bool visible){
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
			string object_handle = string(GetHandleName((BaseList2D*)obj));
			translator->Init(obj, doc, this);
			translator->CreateNSINodes(object_handle.c_str(),transform_handle.c_str(), obj, doc, this);
		}
	}
	//Handle tags
	BaseTag* tag=obj->GetFirstTag();
	while(tag){
		Node tagnode(tag);
		DL_Translator* tagtranslator=tagnode.GetTranslator();
		if(tagtranslator){
			nodes.push_back(tagnode);
			string tag_handle = string(GetHandleName((BaseList2D*)tag));
			tagtranslator->Init(tag, doc, this);
			tagtranslator->CreateNSINodes(tag_handle.c_str(), transform_handle.c_str(), tag, doc, this);
		}
		tag=tag->GetNext();
	}
	BaseShader* shader = obj->GetFirstShader();
	TraverseShaders(shader, doc);
	TraverseScene(obj->GetDown(),doc,transform_handle, obj_visible);
	TraverseScene(obj->GetNext(), doc, parent_transform, visible);
}*/