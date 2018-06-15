#include "RangeTranslator.h"
#include "nsi.hpp"
#include "c4d.h"
#include "xrange.h"
#include "DL_TypeConversions.h"

using namespace std;

void RangeTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Create the shader node
	shader_handle=string(parser->GetUniqueName("range_shader"));
	ctx.Create(shader_handle, "shader");

	BaseShader* shader=(BaseShader*)C4DNode;
	BaseContainer* data=shader->GetDataInstance();

	//Shader file path
	Filename shaderpath=Filename(GeGetPluginPath()+Filename("OSL")+Filename("Range.oso"));
	vector<char> c_shaderpath =StringToChars(shaderpath.GetString());

	float input_min= data->GetFloat(IN_MIN);
	float input_max= data->GetFloat(IN_MAX);
	float output_min= data->GetFloat(OUT_MIN);
	float output_max= data->GetFloat(OUT_MAX);

	ctx.SetAttribute(shader_handle,(
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])),
		NSI::FloatArg("in_min", input_min ),
		NSI::FloatArg("in_max", input_max  ),
		NSI::FloatArg("out_min", output_min),
		NSI::FloatArg("out_max", output_max)
		));

	parser->SetAssociatedHandle((BaseList2D*)C4DNode, shader_handle.c_str());
}

void RangeTranslator::ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser){
		
	NSI::Context ctx(parser->GetContext());

	BaseObject* obj=(BaseObject*)C4DNode;
	BaseContainer* data=obj->GetDataInstance();

	BaseList2D* shader=data->GetLink(INPUT_SHADER, doc);
	if(shader){
		string link_handle=string(parser->GetAssociatedHandle(shader));
		if(link_handle!=""){
			ctx.Connect(link_handle,"c_out",shader_handle,"c_in");
			ctx.Connect(link_handle, "f_out", shader_handle, "f_in");
		}
	}
}
	