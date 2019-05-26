#include "StandardShaderTranslator.h"
#include "c4d.h"
#include "xstandardshader.h"
#include "DL_TypeConversions.h"
#include "DL_Utilities.h"
#include "nsi.hpp"
void StandardShaderTranslator::CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser) {
	NSI::Context ctx(parser->GetContext());

	//Create the shader node
	//shader_handle = string(parser->GetUniqueName("standard_material"));
	ctx.Create(shader_handle, "shader");
	
	BaseObject* obj = (BaseObject*)C4DNode;
	BaseContainer* data = obj->GetDataInstance();

	//Shader file path
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("StandardMaterial.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());

	//Coating parameters
	bool coating_on = data->GetBool(COATING_ON);
	int coating_on_int = 0;
	if (coating_on) { coating_on_int = 1; }

	Vector coating_color = toLinear(data->GetVector(COATING_COLOR), doc);
	float coat_col[3];
	coat_col[0] = coating_color.x;
	coat_col[1] = coating_color.y;
	coat_col[2] = coating_color.z;

	Vector coating_transmittance = toLinear(data->GetVector(COATING_TRANSMITTANCE), doc);
	float coat_trans[3];
	coat_trans[0] = coating_transmittance.x;
	coat_trans[1] = coating_transmittance.y;
	coat_trans[2] = coating_transmittance.z;

	float coating_roughness = data->GetFloat(COATING_ROUGHNESS);

	float coating_fr = data->GetFloat(COATING_FR);

	//Diffuse parameters
	float diffuse_intensity = data->GetFloat(DIFFUSE_WEIGHT);
	Vector diffuse_color = toLinear(data->GetVector(DIFFUSE_COLOR), doc);

	float diff_col[3];
	diff_col[0] = diffuse_color.x;
	diff_col[1] = diffuse_color.y;
	diff_col[2] = diffuse_color.z;

	float diffuse_roughness = 1.55795*data->GetFloat(DIFFUSE_ROUGHNESS); //Range [1, PI/2]

	//Specular parameters
	float specular_intensity = data->GetFloat(SPECULAR_WEIGHT);


	Vector specular_color = toLinear(data->GetVector(SPECULAR_COLOR), doc);
	float spec_col[3];
	spec_col[0] = specular_color.x;
	spec_col[1] = specular_color.y;
	spec_col[2] = specular_color.z;

	float specular_anisotropy = data->GetFloat(SPECULAR_ANISOTROPY, 0);
	//GePrint("Anisotropy: " + String::FloatToString(specular_anisotropy));
	Vector anisotropy_direction = data->GetVector(SPECULAR_ANISOTROPY_DIRECTION);
	float aniso_dir[3];
	aniso_dir[0] = 0.5*(anisotropy_direction.x + 1);
	aniso_dir[1] = 0.5*(anisotropy_direction.y + 1);
	aniso_dir[2] = 0.5*(anisotropy_direction.z + 1);

	float specular_roughness = data->GetFloat(SPECULAR_ROUGHNESS);
	float specular_fr = data->GetFloat(SPECULAR_FR);


	//Emission parameters
	bool emission_on = data->GetBool(EMISSION_ON);
	int emission_on_int = 0;
	if (emission_on) { emission_on_int = 1; }


	float emission_weight = data->GetFloat(EMISSION_WEIGHT);


	Vector emission_color = toLinear(data->GetVector(EMISSION_COLOR), doc);
	float emi_col[3];
	emi_col[0] = emission_color.x;
	emi_col[1] = emission_color.y;
	emi_col[2] = emission_color.z;

	ctx.SetAttribute(shader_handle, (
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])),
		NSI::IntegerArg("coating_on", coating_on_int),
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

	//parser->SetAssociatedHandle((BaseList2D*)C4DNode, shader_handle.c_str());
}

void StandardShaderTranslator::ConnectNSINodes(const char* Handle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser) {

	NSI::Context ctx(parser->GetContext());

	BaseShader* shadernode = (BaseShader*)C4DNode;
	std::string str = shadernode->GetName().GetCStringCopy();
	BaseContainer* data = shadernode->GetDataInstance();

	//Diffuse color
	BaseList2D* shader = data->GetLink(DIFFUSE_COLOR_SHADER, doc);
	if (shader) {
		string link_handle(""); // = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "c_out", shader_handle, "diffuse_color");
		}
	}

	//Diffuse roughness
	shader = data->GetLink(DIFFUSE_ROUGHNESS_SHADER, doc);
	if (shader) {
		string link_handle(""); // = string(parser->GetAssociatedHandle(shader));
		if (link_handle != "") {
			ctx.Connect(link_handle, "f_out", shader_handle, "diffuse_roughness");
		}
	}

	//Coating color
	shader = data->GetLink(COATING_COLOR_SHADER, doc);
	if (shader) {
		string link_handle(""); // = string(parser->GetAssociatedHandle(shader));
		if (link_handle != "") {
			ctx.Connect(link_handle, "c_out", shader_handle, "coating_color");
		}
	}

	//Coating roughness
	shader = data->GetLink(COATING_ROUGHNESS_SHADER, doc);
	if (shader) {
		string link_handle(""); // = string(parser->GetAssociatedHandle(shader));
		if (link_handle != "") {
			ctx.Connect(link_handle, "f_out", shader_handle, "coating_roughness");
		}
	}

	//Specular color
	shader = data->GetLink(SPECULAR_COLOR_SHADER, doc);
	if (shader) {
		string link_handle(""); // = string(parser->GetAssociatedHandle(shader));
		if (link_handle != "") {
			ctx.Connect(link_handle, "c_out", shader_handle, "specular_color");
		}
	}

	//Specular roughness
	shader = data->GetLink(SPECULAR_ROUGHNESS_SHADER, doc);
	if (shader) {
		string link_handle(""); // = string(parser->GetAssociatedHandle(shader));
		if (link_handle != "") {
			ctx.Connect(link_handle, "f_out", shader_handle, "specular_roughness");
		}
	}

	//Specular anisotropy
	shader = data->GetLink(SPECULAR_ANISOTROPY_SHADER, doc);
	if (shader) {
		string link_handle(""); // = string(parser->GetAssociatedHandle(shader));
		if (link_handle != "") {
			ctx.Connect(link_handle, "f_out", shader_handle, "specular_anisotropy");
		}
	}

	//Specular direction
	shader = data->GetLink(SPECULAR_ANISOTROPY_DIRECTION_SHADER, doc);
	if (shader) {
		string link_handle(""); // = string(parser->GetAssociatedHandle(shader));
		if (link_handle != "") {
			ctx.Connect(link_handle, "c_out", shader_handle, "anisotropy_direction");
		}
	}

	//Emission color
	shader = 0;
	shader = data->GetLink(EMISSION_COLOR_SHADER, doc);
	if (shader) {
		string link_handle(""); // = string(parser->GetAssociatedHandle(shader));
		if (link_handle != "") {
			ctx.Connect(link_handle, "c_out", shader_handle, "emission_color");
		}
	}
}
