#include "BasicDisplacementTranslator.h"
#include "c4d.h"
#include "obasicdisplacement.h"
#include "DL_TypeConversions.h"
#include "DL_Utilities.h"
#include "nsi.hpp"

void BasicDisplacementTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	//GePrint("Displacement");
	
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

	/*Vector reflection_color= data->GetVector(REFLECTION_COLOR);
	RtColor reflect_col;
	reflect_col[0]=toLinear(reflection_color.x);
	reflect_col[1]=toLinear(reflection_color.y);
	reflect_col[2]=toLinear(reflection_color.z);
	
	Vector refraction_color=data->GetVector(	REFRACTION_COLOR);
	RtColor refract_col;
	refract_col[0]=toLinear(refraction_color.x);
	refract_col[1]=toLinear(refraction_color.y);
	refract_col[2]=toLinear(refraction_color.z);

	Vector tint=data->GetVector(TINT);
	RtColor tint_col;
	tint_col[0]=toLinear(tint.x);
	tint_col[1]=toLinear(tint.y);
	tint_col[2]=toLinear(tint.z);

	float roughness= data->GetFloat(ROUGHNESS);

	float ior=data->GetFloat(IOR);*/

	ctx.SetAttribute(shader_handle,(
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])),
		//NSI::ColorArg("tint", &tint_col[0]),
		//NSI::ColorArg("reflection_color", &reflect_col[0]),
		//NSI::ColorArg("refraction_color", &refract_col[0]),
		NSI::FloatArg("height", height),
		NSI::FloatArg("strength", strength),
		NSI::FloatArg("displacementbound", height)
		//NSI::FloatArg("ior", ior)
		));

	parser->SetAssociatedHandle((BaseList2D*)C4DNode, shader_handle.c_str());
}

void BasicDisplacementTranslator::ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser){
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
