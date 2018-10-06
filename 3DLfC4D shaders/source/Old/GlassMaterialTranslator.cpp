#include "GlassMaterialTranslator.h"
#include "c4d.h"
#include "oglassmaterial.h"
#include "DL_TypeConversions.h"
#include "DL_Utilities.h"
#include "nsi.hpp"

void GlassMaterialTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Create the shader node
	shader_handle=string(parser->GetUniqueName("glass_material"));
	ctx.Create(shader_handle, "shader");

	BaseObject* obj=(BaseObject*)C4DNode;
	BaseContainer* data=obj->GetDataInstance();

	//Shader file path
	Filename shaderpath=Filename(GeGetPluginPath()+Filename("OSL")+Filename("GlassMaterial.oso"));
	vector<char> c_shaderpath =StringToChars(shaderpath.GetString());

	Vector reflection_color= toLinear(data->GetVector(REFLECTION_COLOR),doc);
	RtColor reflect_col;
	reflect_col[0]=reflection_color.x;
	reflect_col[1]=reflection_color.y;
	reflect_col[2]=reflection_color.z;
	
	Vector refraction_color=toLinear(data->GetVector(	REFRACTION_COLOR),doc);
	RtColor refract_col;
	refract_col[0]=refraction_color.x;
	refract_col[1]=refraction_color.y;
	refract_col[2]=refraction_color.z;

	Vector tint=toLinear(data->GetVector(TINT),doc);
	RtColor tint_col;
	tint_col[0]=tint.x;
	tint_col[1]=tint.y;
	tint_col[2]=tint.z;

	float roughness= data->GetFloat(ROUGHNESS);

	float ior=data->GetFloat(IOR);

	ctx.SetAttribute(shader_handle,(
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])),
		//NSI::ColorArg("tint", &tint_col[0]),
		NSI::ColorArg("reflection_color", &reflect_col[0]),
		NSI::ColorArg("refraction_color", &refract_col[0]),
		NSI::FloatArg("roughness", roughness),
		NSI::FloatArg("ior", ior)
		));

	parser->SetAssociatedHandle((BaseList2D*)C4DNode, shader_handle.c_str());
}

void GlassMaterialTranslator::ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser){
}


//void GlassMaterialTranslator::RegisterSubshaders(BaseList2D* node, BaseDocument* doc, DL_SceneParser* parser){
	//data=node->GetData();
	/*BaseList2D* DiffuseColorShader=data.GetLink(DIFFUSE_COLOR_SHADER,doc);
	parser->RegisterShader(DiffuseColorShader,doc);

	BaseList2D* SpecularColorShader=data.GetLink(SPECULAR_COLOR_SHADER,doc);
	parser->RegisterShader(SpecularColorShader,doc);

	BaseList2D* SpecularRoughnessShader=data.GetLink(SPECULAR_ROUGHNESS_SHADER,doc);
	parser->RegisterShader(SpecularRoughnessShader,doc);

	BaseList2D* EmissionColorShader=data.GetLink(EMISSION_COLOR_SHADER,doc);
	parser->RegisterShader(EmissionColorShader,doc);*/

	//BaseObject* DiffuseColorShader=data.GetObjectLink(DIFFUSE_COLOR_SHADER,doc);
	//parser->RegisterShader(DiffuseColorShader,doc);
//}

//void GlassMaterialTranslator::Emit(char* handle, BaseList2D* node, BaseDocument* doc, DL_SceneParser* parser){
	//GePrint("Glass shader:: Emit");
	//BaseContainer *data = node->GetDataInstance();
	
	/*Vector reflection_color= data.GetVector(REFLECTION_COLOR);
	RtColor reflect_col;
	reflect_col[0]=toLinear(reflection_color.x);
	reflect_col[1]=toLinear(reflection_color.y);
	reflect_col[2]=toLinear(reflection_color.z);
	
	Vector refraction_color=data.GetVector(	REFRACTION_COLOR);
	RtColor refract_col;
	refract_col[0]=toLinear(refraction_color.x);
	refract_col[1]=toLinear(refraction_color.y);
	refract_col[2]=toLinear(refraction_color.z);

	float roughness= data.GetFloat(ROUGHNESS);

	float ior=data.GetFloat(IOR);

	RiShader("GlassMaterial",handle, 
			"uniform color reflection_color", &reflect_col,
			"uniform color refraction_color", &refract_col,
			"uniform float roughness", &roughness,
			"uniform float IOR", &ior,
			RI_NULL);*/
		

	//Coating parameters
	/*bool coating_on=data.GetBool(COATING_ON);
	float coating_on_float=0;
	if(coating_on){coating_on_float=1;}

	Vector coating_color=data.GetVector( COATING_COLOR );
	RtColor coat_col;
	coat_col[0]=toLinear(coating_color.x);
	coat_col[1]=toLinear(coating_color.y);
	coat_col[2]=toLinear(coating_color.z);

	Vector coating_transmittance=data.GetVector(COATING_TRANSMITTANCE);
	RtColor coat_trans;
	coat_trans[0]=toLinear(coating_transmittance.x);
	coat_trans[1]=toLinear(coating_transmittance.y);
	coat_trans[2]=toLinear(coating_transmittance.z);

	float coating_roughness=data.GetFloat(COATING_ROUGHNESS );
	float coating_fr=data.GetFloat(COATING_FR);

	//Diffuse parameters
	float diffuse_intensity= data.GetFloat(DIFFUSE_WEIGHT);
	Vector diffuse_color= data.GetVector(DIFFUSE_COLOR);

	RtColor diff_col;
	diff_col[0]=toLinear(diffuse_color.x);
	diff_col[1]=toLinear(diffuse_color.y);
	diff_col[2]=toLinear(diffuse_color.z);

	BaseList2D* DiffuseColorShader=data.GetLink(DIFFUSE_COLOR_SHADER,doc);
	RtString DiffuseColor_handle=parser->GetShaderHandle(DiffuseColorShader,doc,TYPE_SURFACE);

	float diffuse_roughness = 1.55795*data.GetFloat(DIFFUSE_ROUGHNESS); //Range [1, PI/2]

	//Specular parameters
	float specular_intensity= data.GetFloat(SPECULAR_WEIGHT);
	Vector specular_color= data.GetVector(SPECULAR_COLOR);

	RtColor spec_col;
	spec_col[0]=toLinear(specular_color.x);
	spec_col[1]=toLinear(specular_color.y);
	spec_col[2]=toLinear(specular_color.z);

	BaseList2D* SpecularColorShader=data.GetLink(SPECULAR_COLOR_SHADER,doc);
	RtString SpecularColor_handle=parser->GetShaderHandle(SpecularColorShader,doc,TYPE_SURFACE);
	
	float specular_roughness= data.GetFloat(SPECULAR_ROUGHNESS);
	
	BaseList2D* SpecularRoughnessShader=data.GetLink(SPECULAR_ROUGHNESS_SHADER,doc);
	RtString SpecularRoughness_handle=parser->GetShaderHandle(SpecularRoughnessShader,doc,TYPE_SURFACE);

	float specular_fr = data.GetFloat(SPECULAR_FR);

	//Emission parameters
	bool emission_on=data.GetBool(EMISSION_ON);
	float emission_on_float=0;
	if(emission_on){emission_on_float=1;}
	float emission_weight= data.GetFloat(EMISSION_WEIGHT);
	Vector emission_color= data.GetVector(EMISSION_COLOR);

	RtColor emi_col;
	emi_col[0]=toLinear(emission_color.x);
	emi_col[1]=toLinear(emission_color.y);
	emi_col[2]=toLinear(emission_color.z);

	BaseList2D* EmissionColorShader=data.GetLink(EMISSION_COLOR_SHADER,doc);
	RtString EmissionColor_handle=parser->GetShaderHandle(EmissionColorShader,doc,TYPE_SURFACE);


	



	//Handle for this shader
	//ShaderInfo info;
	//info.handle= parser->GetUniqueShaderName();
	//vector<char> handlechars=StringToChars(info.handle);
	 
	RiShader("StandardMaterial",handle, 
				"uniform float coating_on", &coating_on_float,
			    "uniform color coating_color", &coat_col,
				"uniform float coating_roughness", &coating_roughness,
				"uniform float coating_fr", &coating_fr,
				"uniform float diffuse_weight",&diffuse_intensity, 
				"uniform color diffuse_color", &diff_col, 
				"string diffuse_color_shader",&DiffuseColor_handle,
				"uniform float diffuse_roughness", &diffuse_roughness,
				"uniform float specular_weight", &specular_intensity,
				"uniform color specular_color", &spec_col,
				"string specular_color_shader",&SpecularColor_handle,
				"uniform float specular_roughness", &specular_roughness,
				"string specular_roughness_shader",&SpecularRoughness_handle,
				"uniform float specular_fr", &specular_fr,
				"uniform float emission_on", &emission_on_float,
				"uniform float emission_weight", &emission_weight,
				"uniform color emission_color", &emi_col,
				"string emission_color_shader",&EmissionColor_handle,
				RI_NULL);*/
	
//}

