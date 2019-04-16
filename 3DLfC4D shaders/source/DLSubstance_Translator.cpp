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

	m_ids_to_names[Shader_Path] = pair("", &c_shaderpath[0]);
	m_ids_to_names[SUBSTANCE_COLOR] = pair("Cout", "i_color");
	m_ids_to_names[SUBSTANCE_COLOR_SHADER] = pair("Cout", "i_color");
	m_ids_to_names[SUBSTANCE_ROUGHNESS] = pair("Cout[0]", "roughness");
	m_ids_to_names[SUBSTANCE_ROUGHNESS_SHADER] = pair("Cout[0]", "roughness");
	m_ids_to_names[SUBSTANCE_SPECULAR_LEVEL] = pair("Cout[0]", "specular_level");
	m_ids_to_names[SUBSTANCE_SPECULAR_LEVEL_SHADER] = pair("Cout[0]", "specular_level");
	m_ids_to_names[SUBSTANCE_METALLIC] = pair("Cout[0]", "metallic");
	m_ids_to_names[SUBSTANCE_METALLIC_SHADER] = pair("Cout[0]", "metallic");
	m_ids_to_names[SUBSTANCE_OPACITY] = pair("Cout[0]", "opacity");
	m_ids_to_names[SUBSTANCE_OPACITY_SHADER] = pair("Cout[0]", "opacity");

	m_ids_to_names[EMISSIVE_COLOR] = pair("Cout", "emissive");
	m_ids_to_names[EMISSIVE_COLOR_SHADER] = pair("Cout", "emissive");
	m_ids_to_names[EMISSIVE_INTENSITY] = pair("", "emissive_intensity");

	m_ids_to_names[BUMP_TYPE] = pair("", "disp_normal_bump_type");
	m_ids_to_names[BUMP_INTENSITY] = pair("", "disp_normal_bump_intensity");
}

