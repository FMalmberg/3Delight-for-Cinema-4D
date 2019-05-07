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

	m_ids_to_names[Shader_Path] = make_pair("", &c_shaderpath[0]);
	m_ids_to_names[SKIN_COLOR] = make_pair("", "skin_color");
	m_ids_to_names[SKIN_COLOR_SHADER] = make_pair("outColor", "skin_color");
	m_ids_to_names[SKIN_ROUGHNESS] = make_pair("", "roughness");
	m_ids_to_names[SKIN_ROUGHNESS_SHADER] = make_pair("outColor[0]", "roughness");
	m_ids_to_names[SKIN_SPECULAR_LEVEL] = make_pair("", "specular_level");
	m_ids_to_names[SKIN_SPECULAR_LEVEL_SHADER] = make_pair("outColor[0]", "specular_level");

	m_ids_to_names[SUBSURFACE_COLOR] = make_pair("", "sss_color");
	m_ids_to_names[SUBSURFACE_COLOR_SHADER] = make_pair("outColor", "sss_color");
	m_ids_to_names[SUBSURFACE_SCALE] = make_pair("", "sss_scale");
	m_ids_to_names[SUBSURFACE_SCALE_SHADER] = make_pair("outColor[0]", "sss_scale");
	m_ids_to_names[SUBSURFACE_IOR] = make_pair("", "sss_ior");
	m_ids_to_names[SUBSURFACE_IOR_SHADER] = make_pair("outColor[0]", "sss_ior");

	m_ids_to_names[BUMP_TYPE] = make_pair("", "disp_normal_bump_type");
	m_ids_to_names[BUMP_VALUE] = make_pair("outColor", "disp_normal_bump_value");
	m_ids_to_names[BUMP_INTENSITY] = make_pair("", "disp_normal_bump_intensity");
}

