#include "NormalDisplacementTranslator.h"
#include "c4d.h"
#include "xnormaldisplacementshader.h"
#include "DL_TypeConversions.h"
#include "DL_Utilities.h"
#include "nsi.hpp"

void NormalDisplacementTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	GePrint("Displacement"_s);
	
	NSI::Context ctx(parser->GetContext());

	//Create the shader node
	shader_handle=string(parser->GetUniqueName("normal_displacement"));
	ctx.Create(shader_handle, "shader");

	BaseObject* obj=(BaseObject*)C4DNode;
	BaseContainer* data=obj->GetDataInstance();

	//Shader file path
	Filename shaderpath=Filename(GeGetPluginPath()+Filename("OSL")+Filename("NormalDisplacement.oso"));
	vector<char> c_shaderpath =StringToChars(shaderpath.GetString());

	float height=data->GetFloat(DISPLACEMENT_HEIGHT);
	float strength=data->GetFloat(DISPLACEMENT_STRENGTH);
	float scalarzero = data->GetFloat(DISPLACEMENT_SCALARZERO);
	

	ctx.SetAttribute(shader_handle,(
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])),
		NSI::FloatArg("height", height),
		NSI::FloatArg("strength", strength),
		NSI::FloatArg("scalarzero", scalarzero)
		));

	parser->SetAssociatedHandle((BaseList2D*)C4DNode, shader_handle.c_str());
}

void NormalDisplacementTranslator::ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());
	BaseObject* obj=(BaseObject*)C4DNode;
	BaseContainer* data=obj->GetDataInstance();

	BaseList2D* shader=data->GetLink(DISPLACEMENT_STRENGTH_SHADER, doc);
	if(shader){
		string link_handle=string(parser->GetAssociatedHandle(shader));
		if(link_handle!=""){
			ctx.Connect(link_handle,"f_out",shader_handle,"strength");
		}
	}
}
