#include "StandardMaterialTranslator.h"
#include "c4d.h"
#include "ostandardmaterial.h"
#include "DL_TypeConversions.h"
#include "DL_Utilities.h"
#include "nsi.hpp"


void StandardMaterialTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Create the shader node
	shader_handle=string(parser->GetUniqueName("standard_material"));
	ctx.Create(shader_handle, "shader");

	BaseObject* obj=(BaseObject*)C4DNode;
	BaseContainer* data=obj->GetDataInstance();

	//Shader file path
	Filename shaderpath=Filename(GeGetPluginPath()+Filename("OSL")+Filename("StandardMaterial.oso"));
	vector<char> c_shaderpath =StringToChars(shaderpath.GetString());

	//Coating parameters
	bool coating_on=data->GetBool(COATING_ON);
	int coating_on_int=0;
	if(coating_on){coating_on_int=1;}

	Vector coating_color=toLinear(data->GetVector( COATING_COLOR ),doc);
	RtColor coat_col;
	coat_col[0]=coating_color.x;
	coat_col[1]=coating_color.y;
	coat_col[2]=coating_color.z;
	
	Vector coating_transmittance=toLinear(data->GetVector(COATING_TRANSMITTANCE),doc);
	RtColor coat_trans;
	coat_trans[0]=coating_transmittance.x;
	coat_trans[1]=coating_transmittance.y;
	coat_trans[2]=coating_transmittance.z;

	float coating_roughness=data->GetFloat(COATING_ROUGHNESS );
	
	float coating_fr=data->GetFloat(COATING_FR);
	
	//Diffuse parameters
	float diffuse_intensity= data->GetFloat(DIFFUSE_WEIGHT);
	Vector diffuse_color= toLinear(data->GetVector(DIFFUSE_COLOR),doc);

	RtColor diff_col;
	diff_col[0]=diffuse_color.x;
	diff_col[1]=diffuse_color.y;
	diff_col[2]=diffuse_color.z;

	float diffuse_roughness = 1.55795*data->GetFloat(DIFFUSE_ROUGHNESS); //Range [1, PI/2]

	//Specular parameters
	float specular_intensity= data->GetFloat(SPECULAR_WEIGHT);
	

	Vector specular_color= toLinear(data->GetVector(SPECULAR_COLOR),doc);
	RtColor spec_col;
	spec_col[0]=specular_color.x;
	spec_col[1]=specular_color.y;
	spec_col[2]=specular_color.z;

	float specular_anisotropy = data->GetFloat(SPECULAR_ANISOTROPY, 0);
	//GePrint("Anisotropy: " + String::FloatToString(specular_anisotropy));
	Vector anisotropy_direction = data->GetVector(SPECULAR_ANISOTROPY_DIRECTION);
	RtColor aniso_dir;
	aniso_dir[0] = anisotropy_direction.x;
	aniso_dir[1] = anisotropy_direction.y;
	aniso_dir[2] = anisotropy_direction.z;

	float specular_roughness= data->GetFloat(SPECULAR_ROUGHNESS);


	float specular_fr = data->GetFloat(SPECULAR_FR);


	//Emission parameters
	bool emission_on=data->GetBool(EMISSION_ON);
	int emission_on_int=0;
	if(emission_on){emission_on_int=1;}
	
	
	float emission_weight= data->GetFloat(EMISSION_WEIGHT);
	
	
	Vector emission_color= toLinear(data->GetVector(EMISSION_COLOR),doc);
	RtColor emi_col;
	emi_col[0]=emission_color.x;
	emi_col[1]=emission_color.y;
	emi_col[2]=emission_color.z;
	

	//int coating_on_int=0;
	//params.push_back(GetParameter("coating_color", NSITypeColor, &coat_col[0]));
	//params-push_back(GetParameter("coating_roughness", NSITypeFloat, &coating_roughness));
	//params.push_back(GetParameter("coating_fresnel_reflectance", NSITypeFloat, &coating_fr));

	//params.push_back(GetParameter("diffuse_color", NSITypeColor, &diff_col[0]));
	//params.push_back(GetParameter("diffuse_weight", NSITypeFloat, &diffuse_intensity));
	//params.push_back(GetParameter("diffuse_roughness", NSITypeFloat, &diffuse_roughness));

	//params.push_back(GetParameter("specular_weight", NSITypeFloat, &specular_intensity));
	//params.push_back(GetParameter("specular_color", NSITypeColor, &spec_col[0]));
	//params.push_back(GetParameter("specular_roughness", NSITypeFloat, &specular_roughness));
	//params.push_back(GetParameter("specular_fresnel_reflectance", NSITypeFloat, &specular_fr));
	
	//params.push_back(GetParameter("emission_on", NSITypeInteger, &emission_on_int));
	//params.push_back(GetParameter("emission_weight", NSITypeFloat, &emission_weight));
	//params.push_back(GetParameter("emission_color", NSITypeColor, &emi_col[0]));

	ctx.SetAttribute(shader_handle,(
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])),
		NSI::IntegerArg("coating_on",coating_on_int),
		NSI::ColorArg("coating_color", &coat_col[0]),
		NSI::FloatArg("coating_roughness", coating_roughness),
		NSI::FloatArg("coating_fresnel_reflectance", coating_fr),	
		NSI::ColorArg("diffuse_color", &diff_col[0]),
		NSI::FloatArg("diffuse_weight", diffuse_intensity),	
		NSI::FloatArg("diffuse_roughness", diffuse_roughness),
		NSI::FloatArg("specular_weight", specular_intensity),
		NSI::ColorArg("specular_color", &spec_col[0]),
		NSI::FloatArg("specular_roughness", specular_roughness),
		NSI::FloatArg("specular_fresnel_reflectance", specular_fr),
		NSI::FloatArg("specular_anisotropy", specular_anisotropy),
		NSI::ColorArg("anisotropy_direction", &aniso_dir[0]),
		NSI::IntegerArg("emission_on", emission_on_int),
		NSI::FloatArg("emission_weight", emission_weight),
		NSI::ColorArg("emission_color", &emi_col[0])
		));

	parser->SetAssociatedHandle((BaseList2D*)C4DNode, shader_handle.c_str());
}

void StandardMaterialTranslator::ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser){
	
	NSI::Context ctx(parser->GetContext());

	BaseObject* obj=(BaseObject*)C4DNode;
	BaseContainer* data=obj->GetDataInstance();

	//Diffuse color
	BaseList2D* shader=data->GetLink(DIFFUSE_COLOR_SHADER, doc);
	if(shader){
		string link_handle=string(parser->GetAssociatedHandle(shader));
		if(link_handle!=""){
			ctx.Connect(link_handle,"c_out",shader_handle,"diffuse_color");
		}
	}

	//Diffuse roughness
	shader=data->GetLink(DIFFUSE_ROUGHNESS_SHADER, doc);
	if(shader){
		string link_handle=string(parser->GetAssociatedHandle(shader));
		if(link_handle!=""){
			ctx.Connect(link_handle,"f_out",shader_handle,"diffuse_roughness");
		}
	}

	//Coating color
	shader=data->GetLink(COATING_COLOR_SHADER, doc);
	if(shader){
		string link_handle=string(parser->GetAssociatedHandle(shader));
		if(link_handle!=""){
			ctx.Connect(link_handle,"c_out",shader_handle,"coating_color");
		}
	}

	//Coating roughness
	shader=data->GetLink(COATING_ROUGHNESS_SHADER, doc);
	if(shader){
		string link_handle=string(parser->GetAssociatedHandle(shader));
		if(link_handle!=""){
			ctx.Connect(link_handle,"f_out",shader_handle,"coating_roughness");
		}
	}

	//Specular color
	shader=data->GetLink(SPECULAR_COLOR_SHADER, doc);
	if(shader){
		string link_handle=string(parser->GetAssociatedHandle(shader));
		if(link_handle!=""){
			ctx.Connect(link_handle,"c_out",shader_handle,"specular_color");
		}
	}

	//Specular roughness
	shader=data->GetLink(SPECULAR_ROUGHNESS_SHADER, doc);
	if(shader){
		string link_handle=string(parser->GetAssociatedHandle(shader));
		if(link_handle!=""){
			ctx.Connect(link_handle,"f_out",shader_handle,"specular_roughness");
		}
	}

	//Specular anisotropy
	shader = data->GetLink(SPECULAR_ANISOTROPY_SHADER, doc);
	if (shader){
		string link_handle = string(parser->GetAssociatedHandle(shader));
		if (link_handle != ""){
			ctx.Connect(link_handle, "f_out", shader_handle, "specular_anisotropy");
		}
	}

	//Specular direction
	shader = data->GetLink(SPECULAR_ANISOTROPY_DIRECTION_SHADER, doc);
	if (shader){
		string link_handle = string(parser->GetAssociatedHandle(shader));
		if (link_handle != ""){
			ctx.Connect(link_handle, "c_out", shader_handle, "anisotropy_direction");
		}
	}

	//Emission color
	shader = 0;
	shader=data->GetLink(EMISSION_COLOR_SHADER, doc);
	if(shader){
		string link_handle=string(parser->GetAssociatedHandle(shader));
		if(link_handle!=""){
			ctx.Connect(link_handle,"c_out",shader_handle,"emission_color");
		}
	}
}

/*
void StandardMaterialTranslator::RegisterSubshaders(BaseList2D* node, BaseDocument* doc, DL_SceneParser* parser){
	data=node->GetData();
	vector<long> IDs;
	IDs.push_back(COATING_COLOR_SHADER);
	IDs.push_back(COATING_ROUGHNESS_SHADER);
	IDs.push_back(DIFFUSE_COLOR_SHADER);
	IDs.push_back(DIFFUSE_ROUGHNESS_SHADER);
	IDs.push_back(SPECULAR_COLOR_SHADER);
	IDs.push_back(SPECULAR_ROUGHNESS_SHADER);
	IDs.push_back(EMISSION_COLOR_SHADER);
	IDs.push_back(REFRACTION_COLOR_SHADER);
	IDs.push_back(REFRACTION_ROUGHNESS_SHADER);

	BaseList2D* link;
	for(int i=0; i<IDs.size(); i++){
		link=data.GetLink(IDs[i],doc);
		parser->RegisterShader(link,doc);
	}

}
*/

/*
void StandardMaterialTranslator::Emit(char* handle, BaseList2D* node, BaseDocument* doc, DL_SceneParser* parser){
	
	//Get contexts
	NSIContext_t ctx;
	RtContextHandle RiCtx;
	RiCtx = RiGetContext();
	ctx = RiToNSIContext(RiCtx);

	//Create a NSI node for the shader
	NSICreate(ctx, handle, "shader", 0, NULL);

	vector<NSIParam_t> params;

	char* shaderfile="StandardMaterial.oso";
	params.push_back(GetParameter("shaderfilename", NSITypeString, &shaderfile));

	//Coating parameters
	bool coating_on=data.GetBool(COATING_ON);
	int coating_on_int=0;
	if(coating_on){coating_on_int=1;}
	params.push_back(GetParameter("coating_on", NSITypeInteger, &coating_on_int));

	Vector coating_color=data.GetVector( COATING_COLOR );
	RtColor coat_col;
	coat_col[0]=toLinear(coating_color.x);
	coat_col[1]=toLinear(coating_color.y);
	coat_col[2]=toLinear(coating_color.z);
	params.push_back(GetParameter("coating_color", NSITypeColor, &coat_col[0]));

	Vector coating_transmittance=data.GetVector(COATING_TRANSMITTANCE);
	RtColor coat_trans;
	coat_trans[0]=toLinear(coating_transmittance.x);
	coat_trans[1]=toLinear(coating_transmittance.y);
	coat_trans[2]=toLinear(coating_transmittance.z);

	float coating_roughness=data.GetFloat(COATING_ROUGHNESS );
	params.push_back(GetParameter("coating_roughness", NSITypeFloat, &coating_roughness));
	float coating_fr=data.GetFloat(COATING_FR);
	params.push_back(GetParameter("coating_fresnel_reflectance", NSITypeFloat, &coating_fr));

	//Diffuse parameters
	float diffuse_intensity= data.GetFloat(DIFFUSE_WEIGHT);
	Vector diffuse_color= data.GetVector(DIFFUSE_COLOR);

	RtColor diff_col;
	diff_col[0]=toLinear(diffuse_color.x);
	diff_col[1]=toLinear(diffuse_color.y);
	diff_col[2]=toLinear(diffuse_color.z);

	float diffuse_roughness = 1.55795*data.GetFloat(DIFFUSE_ROUGHNESS); //Range [1, PI/2]

	params.push_back(GetParameter("diffuse_color", NSITypeColor, &diff_col[0]));
	params.push_back(GetParameter("diffuse_weight", NSITypeFloat, &diffuse_intensity));
	params.push_back(GetParameter("diffuse_roughness", NSITypeFloat, &diffuse_roughness));

	

	//Specular parameters
	float specular_intensity= data.GetFloat(SPECULAR_WEIGHT);
	params.push_back(GetParameter("specular_weight", NSITypeFloat, &specular_intensity));

	Vector specular_color= data.GetVector(SPECULAR_COLOR);
	RtColor spec_col;
	spec_col[0]=toLinear(specular_color.x);
	spec_col[1]=toLinear(specular_color.y);
	spec_col[2]=toLinear(specular_color.z);
	params.push_back(GetParameter("specular_color", NSITypeColor, &spec_col[0]));

	float specular_roughness= data.GetFloat(SPECULAR_ROUGHNESS);
	params.push_back(GetParameter("specular_roughness", NSITypeFloat, &specular_roughness));

	float specular_fr = data.GetFloat(SPECULAR_FR);
	params.push_back(GetParameter("specular_fresnel_reflectance", NSITypeFloat, &specular_fr));

	//Emission parameters
	bool emission_on=data.GetBool(EMISSION_ON);
	int emission_on_int=0;
	if(emission_on){emission_on_int=1;}
	params.push_back(GetParameter("emission_on", NSITypeInteger, &emission_on_int));
	
	float emission_weight= data.GetFloat(EMISSION_WEIGHT);
	params.push_back(GetParameter("emission_weight", NSITypeFloat, &emission_weight));
	
	Vector emission_color= data.GetVector(EMISSION_COLOR);
	RtColor emi_col;
	emi_col[0]=toLinear(emission_color.x);
	emi_col[1]=toLinear(emission_color.y);
	emi_col[2]=toLinear(emission_color.z);
	params.push_back(GetParameter("emission_color", NSITypeColor, &emi_col[0]));

	NSISetAttribute(ctx,handle,params.size(), &params[0] );

	//Make connections
	BaseList2D* CoatingColorShader=data.GetLink(COATING_COLOR_SHADER,doc);
	RtString CoatingColor_handle=parser->GetShaderHandle(CoatingColorShader,doc,TYPE_SURFACE);
	if(strcmp(CoatingColor_handle,"")!=0){
		NSIConnect(ctx,CoatingColor_handle, "c_out", handle, "coating_color",0,NULL);
	}

	BaseList2D* CoatingRoughnessShader=data.GetLink(COATING_COLOR_SHADER,doc);
	RtString CoatingRoughness_handle=parser->GetShaderHandle(CoatingRoughnessShader,doc,TYPE_SURFACE);
	if(strcmp(CoatingRoughness_handle,"")!=0){
		NSIConnect(ctx,CoatingRoughness_handle, "f_out", handle, "coating_roughness",0,NULL);
	}

	BaseList2D* DiffuseColorShader=data.GetLink(DIFFUSE_COLOR_SHADER,doc);
	RtString DiffuseColor_handle=parser->GetShaderHandle(DiffuseColorShader,doc,TYPE_SURFACE);
	if(strcmp(DiffuseColor_handle,"")!=0){
		NSIConnect(ctx,DiffuseColor_handle, "c_out", handle, "diffuse_color",0,NULL);
	}

	BaseList2D* SpecularColorShader=data.GetLink(SPECULAR_COLOR_SHADER,doc);
	RtString SpecularColor_handle=parser->GetShaderHandle(SpecularColorShader,doc,TYPE_SURFACE);
	if(strcmp(SpecularColor_handle,"")!=0){
		NSIConnect(ctx,SpecularColor_handle, "c_out", handle, "specular_color",0,NULL);
	}
	
	BaseList2D* SpecularRoughnessShader=data.GetLink(SPECULAR_ROUGHNESS_SHADER,doc);
	RtString SpecularRoughness_handle=parser->GetShaderHandle(SpecularRoughnessShader,doc,TYPE_SURFACE);
	if(strcmp(SpecularRoughness_handle,"")!=0){
		NSIConnect(ctx,SpecularRoughness_handle, "f_out", handle, "specular_roughness",0,NULL);
	}

	BaseList2D* EmissionColorShader=data.GetLink(EMISSION_COLOR_SHADER,doc);
	RtString EmissionColor_handle=parser->GetShaderHandle(EmissionColorShader,doc,TYPE_SURFACE);
	if(strcmp(EmissionColor_handle,"")!=0){
		NSIConnect(ctx,EmissionColor_handle, "c_out", handle, "emission_color",0,NULL);
	}

	
}
*/
/*
void StandardMaterialTranslator::CacheStaticData(BaseList2D* node, BaseDocument* doc){
	data=node->GetData();
}

bool StandardMaterialTranslator::IsType(ShaderType type){
	return type==TYPE_SURFACE;
}*/
