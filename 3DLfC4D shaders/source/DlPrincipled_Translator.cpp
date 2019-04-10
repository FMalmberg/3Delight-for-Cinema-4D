#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DlPrincipled_translator.h"
#include <assert.h>


void Delight_Principled::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());
	m_material_handle = string(parser->GetUniqueName("dl_Principled"));
	ctx.Create(m_material_handle, "shader");
	m_material_attributes = string(parser->GetUniqueName("dl_principled_attribute"));
	ctx.Create(m_material_attributes, "attributes");

	BaseMaterial* material = (BaseMaterial*)C4DNode;
	BaseContainer* data = material->GetDataInstance();

	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlPrincipled.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());

	float coating_thickness = data->GetFloat(COATING_LAYER_THICKNESS);
	Vector c_color = data->GetVector(COATING_LAYER_COLOR);
	float coating_color[3] { c_color.x, c_color.y, c_color.z };
	float coating_roughness = data->GetFloat(COATING_LAYER_ROUGHNESS);
	float coating_specular = data->GetFloat(COATING_LAYER_SPECULAR_LEVEL);
	
	Vector b_color = data->GetVector(BASE_LAYER_COLOR);
	float base_color[3]{ b_color.x, b_color.y, b_color.z };
	float base_roughness = data->GetFloat(BASE_LAYER_ROUGHNESS);
	float base_specular = data->GetFloat(BASE_LAYER_SPECULAR_LEVEL);
	float base_metallic = data->GetFloat(BASE_LAYER_METALLIC);
	float base_anisotropy = data->GetFloat(BASE_LAYER_ANISOTROPY);
	Vector b_anisotropy_color = data->GetVector(BASE_LAYER_ANISOTROPY_DIRECTION);
	float base_anisotropy_color[3]{ b_anisotropy_color.x, b_anisotropy_color.y, b_anisotropy_color.z };
	float base_opacity = data->GetFloat(BASE_LAYER_OPACITY);

	int use_subsurface = data->GetInt32(USE_SUBSURFACE);
	Vector s_color = data->GetVector(SUBSURFACE_COLOR);
	float subsurface_color[3]{ s_color.x, s_color.y, s_color.z };
	float subsurface_ior = data->GetFloat(SUBSURFACE_IOR);
	float subsurface_scale = data->GetFloat(SUBSURFACE_SCALE);

	Vector i_color = data->GetVector(INCADESCENCE_COLOR);
	float incadescence_color[3]{ i_color.x, i_color.y, i_color.z };
	float incadescence_intensity = data->GetFloat(INCADESCENCE_INTENSITY);

	int bump_type = data->GetInt32(BUMP_TYPE);
	float bump_intensity = data->GetFloat(BUMP_INTENSITY);
	int bump_affect = data->GetInt32(BUMP_LAYERS_AFFECTED);


	ctx.SetAttribute(m_material_handle, (
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])),
		NSI::FloatArg("coating_thickness", coating_thickness),
		NSI::ColorArg("coating_color", &coating_color[0]),
		NSI::FloatArg("coating_roughness", coating_roughness),
		NSI::FloatArg("coating_specular_level", coating_specular),
		NSI::ColorArg("i_color", &base_color[0]),
		NSI::FloatArg("roughness", base_roughness),
		NSI::FloatArg("specular_level", base_specular),
		NSI::FloatArg("metallic", base_metallic),
		NSI::FloatArg("anisotropy", base_anisotropy),
		NSI::ColorArg("anisotropy_direction", &base_anisotropy_color[0]),
		NSI::FloatArg("opacity", base_opacity),
		NSI::FloatArg("sss_on", use_subsurface),
		NSI::ColorArg("sss_color", &subsurface_color[0]),
		NSI::FloatArg("sss_ior", subsurface_ior),
		NSI::FloatArg("sss_scale", subsurface_scale),
		NSI::ColorArg("incandescence", &incadescence_color[0]),
		NSI::FloatArg("incandescence_intensity", incadescence_intensity),
		NSI::IntegerArg("disp_normal_bump_type", bump_type),
		NSI::FloatArg("disp_normal_bump_intensity", bump_intensity),
		NSI::FloatArg("normal_bump_affect_layer", bump_affect)

		));

		parser->SetAssociatedHandle((BaseList2D*)C4DNode, m_material_attributes.c_str());
}

void Delight_Principled::ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());

	BaseMaterial* material = (BaseMaterial*)C4DNode;
	BaseContainer* data = material->GetDataInstance();

	ctx.Connect(m_material_handle, "", m_material_attributes, "surfaceshader");
	
	BaseList2D* shader = data->GetLink(COATING_LAYER_THICKNESS_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout[0]", m_material_handle, "coating_thickness");
		}
	}


	shader = data->GetLink(COATING_LAYER_COLOR_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout", m_material_handle, "coating_color");
		}
	}


	shader = data->GetLink(COATING_LAYER_ROUGHNESS_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout[0]", m_material_handle, "coating_roughness");
		}
	}


	shader = data->GetLink(COATING_LAYER_SPECULAR_LEVEL_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout[0]", m_material_handle, "coating_specular_level");
		}
	}


	shader = data->GetLink(BASE_LAYER_COLOR_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout", m_material_handle, "i_color");
		}
	}


	shader = data->GetLink(BASE_LAYER_ROUGHNESS_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout[0]", m_material_handle, "roughness");
		}
	}


	shader = data->GetLink(BASE_LAYER_SPECULAR_LEVEL_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout[0]", m_material_handle, "specular_level");
		}
	}


	shader = data->GetLink(BASE_LAYER_METALLIC_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout[0]", m_material_handle, "metallic");
		}
	}


	shader = data->GetLink(BASE_LAYER_ANISOTROPY_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout[0]", m_material_handle, "anisotropy");
		}
	}


	shader = data->GetLink(BASE_LAYER_ANISOTROPY_DIRECTION_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout", m_material_handle, "anisotropy_direction");
		}
	}


	shader = data->GetLink(BASE_LAYER_OPACITY_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout[0]", m_material_handle, "opacity");
		}
	}


	shader = data->GetLink(SUBSURFACE_COLOR_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout", m_material_handle, "sss_color");
		}
	}


	shader = data->GetLink(SUBSURFACE_IOR_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout[0]", m_material_handle, "sss_ior");
		}
	}


	shader = data->GetLink(SUBSURFACE_SCALE_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout[0]", m_material_handle, "sss_scale");
		}
	}


	shader = data->GetLink(INCADESCENCE_COLOR_SHADER, doc);
	if (shader) {
		string link_handle = string(parser->GetAssociatedHandle(shader));
		ApplicationOutput("Linkage @", link_handle.c_str());
		if (link_handle != "") {
			ctx.Connect(link_handle, "Cout", m_material_handle, "incandescence");
		}
	}
}
