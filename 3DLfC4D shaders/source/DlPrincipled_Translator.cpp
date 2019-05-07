#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DlPrincipled_translator.h"
#include "dl_principled.h"
#include <assert.h>
#include <utility>

Delight_Principled::Delight_Principled()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlPrincipled.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	m_ids_to_names[Shader_Path] =							make_pair("",&c_shaderpath[0]);
	m_ids_to_names[COATING_LAYER_THICKNESS] =				make_pair("","coating_thickness");
	m_ids_to_names[COATING_LAYER_THICKNESS_SHADER] =		make_pair("outColor[0]","coating_thickness");
	m_ids_to_names[COATING_LAYER_COLOR] =					make_pair("","coating_color");
	m_ids_to_names[COATING_LAYER_COLOR_SHADER] =			make_pair("outColor","coating_color");
	m_ids_to_names[COATING_LAYER_ROUGHNESS] =			    make_pair("","coating_roughness");
	m_ids_to_names[COATING_LAYER_ROUGHNESS_SHADER] =		make_pair("outColor[0]","coating_roughness");
	m_ids_to_names[COATING_LAYER_SPECULAR_LEVEL] =			make_pair("","coating_specular_level");
	m_ids_to_names[COATING_LAYER_SPECULAR_LEVEL_SHADER] =	make_pair("outColor[0]","coating_specular_level");
	m_ids_to_names[BASE_LAYER_COLOR] =						make_pair("","i_color");
	m_ids_to_names[BASE_LAYER_COLOR_SHADER] =				make_pair("outColor", "i_color");
	m_ids_to_names[BASE_LAYER_ROUGHNESS] =					make_pair("","roughness");
	m_ids_to_names[BASE_LAYER_ROUGHNESS_SHADER] =			make_pair("outColor", "roughness");
	m_ids_to_names[BASE_LAYER_SPECULAR_LEVEL] =				make_pair("","specular_level");
	m_ids_to_names[BASE_LAYER_SPECULAR_LEVEL_SHADER] =		make_pair("outColor[0]", "specular_level");
	m_ids_to_names[BASE_LAYER_METALLIC] =					make_pair("","metallic");
	m_ids_to_names[BASE_LAYER_METALLIC_SHADER] =			make_pair("outColor[0]", "metallic");
	m_ids_to_names[BASE_LAYER_ANISOTROPY] =					make_pair("","anisotropy");
	m_ids_to_names[BASE_LAYER_ANISOTROPY_SHADER] =			make_pair("outColor[0]", "anisotropy");
	m_ids_to_names[BASE_LAYER_ANISOTROPY_DIRECTION] =		make_pair("","anisotropy_direction");
	m_ids_to_names[BASE_LAYER_ANISOTROPY_DIRECTION_SHADER]= make_pair("outColor","anisotropy_direction");
	m_ids_to_names[BASE_LAYER_OPACITY] =					make_pair("","opacity");
	m_ids_to_names[BASE_LAYER_OPACITY_SHADER] =				make_pair("outColor[0]","opacity");
	m_ids_to_names[USE_SUBSURFACE] =						make_pair("","sss_on");
	m_ids_to_names[SUBSURFACE_COLOR] =						make_pair("","sss_color");
	m_ids_to_names[SUBSURFACE_COLOR_SHADER] =				make_pair("outColor", "sss_color");
	m_ids_to_names[SUBSURFACE_IOR] =						make_pair("", "sss_ior");
	m_ids_to_names[SUBSURFACE_IOR_SHADER] =					make_pair("outColor[0]", "sss_ior");
	m_ids_to_names[SUBSURFACE_SCALE] =						make_pair("", "sss_scale");
	m_ids_to_names[SUBSURFACE_SCALE_SHADER] =				make_pair("outColor[0]", "sss_scale");
	m_ids_to_names[INCADESCENCE_COLOR] =					make_pair("", "incandescence");
	m_ids_to_names[INCADESCENCE_COLOR_SHADER] =				make_pair("outColor", "incandescence");
	m_ids_to_names[INCADESCENCE_INTENSITY] =				make_pair("","incandescence_intensity");
	m_ids_to_names[BUMP_TYPE] =								make_pair("","disp_normal_bump_type");
	m_ids_to_names[BUMP_INTENSITY] =						make_pair("","disp_normal_bump_intensity");
	m_ids_to_names[BUMP_VALUE] =							make_pair("outColor","disp_normal_bump_value");
	m_ids_to_names[BUMP_LAYERS_AFFECTED] =					make_pair("","normal_bump_affect_layer");

}

