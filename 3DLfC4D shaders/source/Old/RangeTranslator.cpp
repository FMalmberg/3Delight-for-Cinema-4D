#include "RangeTranslator.h"
#include "nsi.hpp"
#include "c4d.h"
#include "orange.h"
#include "NS_TypeConversions.h"

void RangeTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, NS_SceneParser* parser){
NSI::Context ctx(parser->GetContext());

	//Create the shader node
	shader_handle=string(parser->GetUniqueName("range_shader"));
	ctx.Create(shader_handle, "shader");

	BaseObject* obj=(BaseObject*)C4DNode;
	BaseContainer* data=obj->GetDataInstance();

	//Shader file path
	Filename shaderpath=Filename(GeGetPluginPath()+Filename("OSL")+Filename("Range.oso"));
	vector<char> c_shaderpath =StringToChars(shaderpath.GetString());

	float input_min= data->GetReal(IN_MIN);
	float input_max= data->GetReal(IN_MAX);
	float output_min= data->GetReal(OUT_MIN);
	float output_max= data->GetReal(OUT_MAX);

	ctx.SetAttribute(shader_handle,(
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])),
		NSI::FloatArg("in_min", input_min ),
		NSI::FloatArg("in_max", input_max  ),
		NSI::FloatArg("out_min", output_min),
		NSI::FloatArg("out_max", output_max)
		));

	parser->SetAssociatedHandle((BaseList2D*)C4DNode, shader_handle.c_str());
}

void RangeTranslator::ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc,NS_SceneParser* parser){
		
	NSI::Context ctx(parser->GetContext());

	BaseObject* obj=(BaseObject*)C4DNode;
	BaseContainer* data=obj->GetDataInstance();

	BaseList2D* shader=data->GetLink(INPUT_SHADER, doc);
	if(shader){
		string link_handle=string(parser->GetAssociatedHandle(shader));
		if(link_handle!=""){
			ctx.Connect(link_handle,"c_out",shader_handle,"c_in");
		}
	}
}
	
/*void RangeTranslator::RegisterSubshaders(BaseList2D* node, BaseDocument* doc, NS_SceneParser* parser){
	
	data=node->GetData();
	BaseList2D* InputShader=data.GetLink(INPUT_SHADER,doc);
	parser->RegisterShader(InputShader,doc);

}*/

/*void RangeTranslator::Emit(char* handle, BaseList2D* node, BaseDocument* doc, NS_SceneParser* parser){
	//BaseContainer *data = node->GetDataInstance();
	
	RtFloat input_min= data.GetReal(IN_MIN);
	RtFloat input_max= data.GetReal(IN_MAX);
	RtFloat output_min= data.GetReal(OUT_MIN);
	RtFloat output_max= data.GetReal(OUT_MAX);

	BaseList2D*InputShader=data.GetLink(INPUT_SHADER,doc);
	RtString Input_handle=parser->GetShaderHandle(InputShader,doc,TYPE_SURFACE);
	 
	RiShader("Range",handle,
			"string input_shader ",&Input_handle,
			"uniform float in_min  ", &input_min,
			"uniform float in_max  ", &input_max,
			"uniform float out_min  ", &output_min,
			"uniform float out_max  ", &output_max,
			RI_NULL);
	
}

void RangeTranslator::CacheStaticData(BaseList2D* node, BaseDocument* doc){
	data=node->GetData();
}

bool RangeTranslator::IsType(ShaderType type){
	return type==TYPE_SURFACE;
}
*/