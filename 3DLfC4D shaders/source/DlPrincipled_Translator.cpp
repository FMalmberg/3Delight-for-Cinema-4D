#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DlPrincipled_translator.h"
#include <assert.h>
#include <utility>

Delight_Principled::Delight_Principled()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlPrincipled.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	m_ids_to_names[Shader_Path] =							pair("",&c_shaderpath[0]);
	m_ids_to_names[COATING_LAYER_THICKNESS] =				pair("Cout[0]","coating_thickness");
	m_ids_to_names[COATING_LAYER_THICKNESS_SHADER] =		pair("Cout[0]","coating_thickness");
	m_ids_to_names[COATING_LAYER_COLOR] =					pair("Cout","coating_color");
	m_ids_to_names[COATING_LAYER_COLOR_SHADER] =			pair("Cout","coating_color");
	m_ids_to_names[COATING_LAYER_ROUGHNESS] =			    pair("Cout[0]","coating_roughness");
	m_ids_to_names[COATING_LAYER_ROUGHNESS_SHADER] =		pair("Cout[0]","coating_roughness");
	m_ids_to_names[COATING_LAYER_SPECULAR_LEVEL] =			pair("Cout[0]","coating_specular_level");
	m_ids_to_names[COATING_LAYER_SPECULAR_LEVEL_SHADER] =	pair("Cout[0]","coating_specular_level");
	m_ids_to_names[BASE_LAYER_COLOR] =						pair("Cout","i_color");
	m_ids_to_names[BASE_LAYER_COLOR_SHADER] =				pair("Cout", "i_color");
	m_ids_to_names[BASE_LAYER_ROUGHNESS] =					pair("Cout[0]","roughness");
	m_ids_to_names[BASE_LAYER_ROUGHNESS_SHADER] =			pair("Cout", "roughness");
	m_ids_to_names[BASE_LAYER_SPECULAR_LEVEL] =				pair("Cout[0]","specular_level");
	m_ids_to_names[BASE_LAYER_SPECULAR_LEVEL_SHADER] =		pair("Cout[0]", "specular_level");
	m_ids_to_names[BASE_LAYER_METALLIC] =					pair("Cout[0]","metallic");
	m_ids_to_names[BASE_LAYER_METALLIC_SHADER] =			pair("Cout[0]", "metallic");
	m_ids_to_names[BASE_LAYER_ANISOTROPY] =					pair("Cout[0]","anisotropy");
	m_ids_to_names[BASE_LAYER_ANISOTROPY_SHADER] =			pair("Cout[0]", "anisotropy");
	m_ids_to_names[BASE_LAYER_ANISOTROPY_DIRECTION] =		pair("Cout","anisotropy_direction");
	m_ids_to_names[BASE_LAYER_ANISOTROPY_DIRECTION_SHADER]= pair("Cout","anisotropy_direction");
	m_ids_to_names[BASE_LAYER_OPACITY] =					pair("Cout[0]","opacity");
	m_ids_to_names[BASE_LAYER_OPACITY_SHADER] =				pair("Cout[0]","opacity");
	m_ids_to_names[USE_SUBSURFACE] =						pair("","sss_on");
	m_ids_to_names[SUBSURFACE_COLOR] =						pair("Cout","sss_color");
	m_ids_to_names[SUBSURFACE_COLOR_SHADER] =				pair("Cout", "sss_color");
	m_ids_to_names[SUBSURFACE_IOR] =						pair("Cout[0]", "sss_ior");
	m_ids_to_names[SUBSURFACE_IOR_SHADER] =					pair("Cout[0]", "sss_ior");
	m_ids_to_names[SUBSURFACE_SCALE] =						pair("Cout[0]", "sss_scale");
	m_ids_to_names[SUBSURFACE_SCALE_SHADER] =				pair("Cout[0]", "sss_scale");
	m_ids_to_names[INCADESCENCE_COLOR] =					pair("Cout", "incandescence");
	m_ids_to_names[INCADESCENCE_COLOR_SHADER] =				pair("Cout", "incandescence");
	m_ids_to_names[INCADESCENCE_INTENSITY] =				pair("","incandescence_intensity");
	m_ids_to_names[BUMP_TYPE] =								pair("","disp_normal_bump_type");
	m_ids_to_names[BUMP_INTENSITY] =						pair("","disp_normal_bump_intensity");
	m_ids_to_names[BUMP_LAYERS_AFFECTED] =					pair("","normal_bump_affect_layer");

}

