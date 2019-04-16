#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLSkin_Translator.h"
#include "dl_skin.h"
#include <assert.h>
#include <utility>

Delight_Skin::Delight_Skin()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlSkin.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());

	m_ids_to_names[Shader_Path] = pair("", &c_shaderpath[0]);
	m_ids_to_names[SKIN_COLOR] = pair("Cout", "skin_color");
	m_ids_to_names[SKIN_COLOR_SHADER] = pair("Cout", "skin_color");
	m_ids_to_names[SKIN_ROUGHNESS] = pair("Cout[0]", "roughness");
	m_ids_to_names[SKIN_ROUGHNESS_SHADER] = pair("Cout[0]", "roughness");
	m_ids_to_names[SKIN_SPECULAR_LEVEL] = pair("Cout[0]", "specular_level");
	m_ids_to_names[SKIN_SPECULAR_LEVEL_SHADER] = pair("Cout[0]", "specular_level");

	m_ids_to_names[SUBSURFACE_COLOR] = pair("Cout", "sss_color");
	m_ids_to_names[SUBSURFACE_COLOR_SHADER] = pair("Cout", "sss_color");
	m_ids_to_names[SUBSURFACE_SCALE] = pair("Cout[0]", "sss_scale");
	m_ids_to_names[SUBSURFACE_SCALE_SHADER] = pair("Cout[0]", "sss_scale");
	m_ids_to_names[SUBSURFACE_IOR] = pair("Cout[0]", "sss_ior");
	m_ids_to_names[SUBSURFACE_IOR_SHADER] = pair("Cout[0]", "sss_ior");

	m_ids_to_names[BUMP_TYPE] = pair("", "disp_normal_bump_type");
	m_ids_to_names[BUMP_INTENSITY] = pair("", "disp_normal_bump_intensity");
}

