#include "GlassShaderTranslator.h"
#include "c4d.h"
#include "xglassshader.h"
#include "DL_TypeConversions.h"
#include "DL_Utilities.h"
#include "nsi.hpp"

void GlassShaderTranslator::CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	ApplicationOutput(ParentTransformHandle);
	//Create the shader node
	//shader_handle = string(parser->GetUniqueName("glass_material"));
	ctx.Create(shader_handle, "shader");

	BaseObject* obj = (BaseObject*)C4DNode;
	BaseContainer* data = obj->GetDataInstance();

	//Shader file path
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("GlassMaterial.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());

	Vector reflection_color = toLinear(data->GetVector(REFLECTION_COLOR), doc);
	float reflect_col[3];
	reflect_col[0] = reflection_color.x;
	reflect_col[1] = reflection_color.y;
	reflect_col[2] = reflection_color.z;

	Vector refraction_color = toLinear(data->GetVector(REFRACTION_COLOR), doc);
	float refract_col[3];
	refract_col[0] = refraction_color.x;
	refract_col[1] = refraction_color.y;
	refract_col[2] = refraction_color.z;

	Vector tint = toLinear(data->GetVector(TINT), doc);
	float tint_col[3];
	tint_col[0] = tint.x;
	tint_col[1] = tint.y;
	tint_col[2] = tint.z;

	float roughness = data->GetFloat(ROUGHNESS);

	float ior = data->GetFloat(IOR);

	int use_refraction = 0;
	Int32 mode = data->GetInt32(MODE);
	if (mode == MODE_REFRACTION) {
		use_refraction = 1;
	}

	ctx.SetAttribute(shader_handle, (
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])),
		NSI::ColorArg("reflection_color", &reflect_col[0]),
		NSI::ColorArg("refraction_color", &refract_col[0]),
		NSI::FloatArg("roughness", roughness),
		NSI::FloatArg("ior", ior),
		NSI::IntegerArg("use_refraction", use_refraction)
		));

	//parser->SetAssociatedHandle((BaseList2D*)C4DNode, shader_handle.c_str());
}

void GlassShaderTranslator::ConnectNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser){

	NSI::Context ctx(parser->GetContext());

	BaseShader* shadernode = (BaseShader*)C4DNode;
	BaseContainer* data = shadernode->GetDataInstance();

	//Roughness
	BaseList2D* shader = data->GetLink(ROUGHNESS_SHADER, doc);
	if (shader){	
		string link_handle(""); // = string(parser->GetAssociatedHandle(shader));
		if (link_handle != ""){
			ctx.Connect(link_handle, "f_out", shader_handle, "roughness");
		}
	}
}
