#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLSubstance_Translator.h"
#include "dl_substance.h"
#include <assert.h>
#include <utility>

Delight_Substance::Delight_Substance()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlSubstance.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());

	m_ids_to_names[Shader_Path] = make_pair("", &c_shaderpath[0]);
	m_ids_to_names[SUBSTANCE_COLOR] = make_pair("", "i_color");
	m_ids_to_names[SUBSTANCE_COLOR_SHADER] = make_pair("outColor", "i_color");
	m_ids_to_names[SUBSTANCE_ROUGHNESS] = make_pair("", "roughness");
	m_ids_to_names[SUBSTANCE_ROUGHNESS_SHADER] = make_pair("outColor[0]", "roughness");
	m_ids_to_names[SUBSTANCE_SPECULAR_LEVEL] = make_pair("", "specular_level");
	m_ids_to_names[SUBSTANCE_SPECULAR_LEVEL_SHADER] = make_pair("outColor[0]", "specular_level");
	m_ids_to_names[SUBSTANCE_METALLIC] = make_pair("", "metallic");
	m_ids_to_names[SUBSTANCE_METALLIC_SHADER] = make_pair("outColor[0]", "metallic");
	m_ids_to_names[SUBSTANCE_OPACITY] = make_pair("", "opacity");
	m_ids_to_names[SUBSTANCE_OPACITY_SHADER] = make_pair("outColor[0]", "opacity");

	m_ids_to_names[EMISSIVE_COLOR] = make_pair("", "emissive");
	m_ids_to_names[EMISSIVE_COLOR_SHADER] = make_pair("outColor", "emissive");
	m_ids_to_names[EMISSIVE_INTENSITY] = make_pair("", "emissive_intensity");

	m_ids_to_names[BUMP_TYPE] = make_pair("", "disp_normal_bump_type");
	m_ids_to_names[BUMP_VALUE] = make_pair("outColor", "disp_normal_bump_value");
	m_ids_to_names[BUMP_INTENSITY] = make_pair("", "disp_normal_bump_intensity");
}

