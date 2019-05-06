#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLHairAndFur_Translator.h"
#include "dl_hair_and_fur.h"
#include <assert.h>
#include <utility>

Delight_HairAndFur::Delight_HairAndFur()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlHairAndFur.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());

	m_ids_to_names[Shader_Path] = pair("", &c_shaderpath[0]);
	m_ids_to_names[HAIR_COLOR_MELANIN] = pair("", "eumelanine");
	m_ids_to_names[HAIR_COLOR_MELANIN_SHADER] = pair("outColor[0]", "eumelanine");
	m_ids_to_names[HAIR_COLOR_MELANIN_RED] = pair("", "phenomelanine");
	m_ids_to_names[HAIR_COLOR_MELANIN_RED_SHADER] = pair("outColor[0]", "phenomelanine");
	m_ids_to_names[HAIR_COLOR_DYE] = pair("", "i_color");
	m_ids_to_names[HAIR_COLOR_DYE_SHADER] = pair("outColor", "i_color");
	m_ids_to_names[HAIR_COLOR_DYE_WEIGHT] = pair("", "dye_weight");
	m_ids_to_names[HAIR_COLOR_DYE_WEIGHT_SHADER] = pair("outColor[0]", "dye_weight");

	m_ids_to_names[HAIR_LOOK_REFLECTIVITY] = pair("", "reflectivity");
	m_ids_to_names[HAIR_LOOK_REFLECTIVITY_SHADER] = pair("outColor[0]", "reflectivity");
	m_ids_to_names[HAIR_LOOK_ROUGHNESS_ALONG] = pair("", "longitudinal_roughness");
	m_ids_to_names[HAIR_LOOK_ROUGHNESS_ALONG_SHADER] = pair("outColor[0]", "longitudinal_roughness");
	m_ids_to_names[HAIR_LOOK_ROUGHNESS_AROUND] = pair("", "azimuthal_roughness");
	m_ids_to_names[HAIR_LOOK_ROUGHNESS_AROUND_SHADER] = pair("outColor[0]", "azimuthal_roughness");
	m_ids_to_names[HAIR_LOOK_SYNTHETIC_FIBER] = pair("", "synthetic");
	m_ids_to_names[HAIR_LOOK_SYNTHETIC_FIBER_SHADER] = pair("outColor[0]", "synthetic");

	m_ids_to_names[HAIR_VARIATION_MELANIN] = pair("", "variation_melanin");
	m_ids_to_names[HAIR_VARIATION_MELANIN_SHADER] = pair("outColor[0]", "variation_melanin");
	m_ids_to_names[HAIR_VARIATION_MELANIN_RED] = pair("", "variation_melanin_red");
	m_ids_to_names[HAIR_VARIATION_MELANIN_RED_SHADER] = pair("outColor[0]", "variation_melanin_red");
	m_ids_to_names[HAIR_VARIATION_WHITE_HAIR] = pair("", "variation_white_hair");
	m_ids_to_names[HAIR_VARIATION_WHITE_HAIR_SHADER] = pair("outColor[0]", "variation_white_hair");
	m_ids_to_names[HAIR_VARIATION_DYE_HUE] = pair("", "variation_dye_hue");
	m_ids_to_names[HAIR_VARIATION_DYE_HUE_SHADER] = pair("outColor[0]", "variation_dye_hue");
	m_ids_to_names[HAIR_VARIATION_DYE_SATURATION] = pair("", "variation_dye_saturation");
	m_ids_to_names[HAIR_VARIATION_DYE_SATURATION_SHADER] = pair("outColor[0]", "variation_dye_saturation");
	m_ids_to_names[HAIR_VARIATION_DYE_VALUE] = pair("", "variation_dye_value");
	m_ids_to_names[HAIR_VARIATION_DYE_VALUE_SHADER] = pair("outColor[0]", "variation_dye_value");
	m_ids_to_names[HAIR_VARIATION_ROUGHNESS] = pair("", "variation_roughness");
	m_ids_to_names[HAIR_VARIATION_ROUGHNESS_SHADER] = pair("outColor[0]", "variation_roughness");
	m_ids_to_names[HAIR_VARIATION_REFLECTIVITY] = pair("", "variation_reflectivity");
	m_ids_to_names[HAIR_VARIATION_REFLECTIVITY_SHADER] = pair("outColor[0]", "variation_reflectivity");

	m_ids_to_names[HAIR_BOOST_GLOSSINESS] = pair("", "boost_glossiness");
	m_ids_to_names[HAIR_BOOST_GLOSSINESS_SHADER] = pair("outColor[0]", "boost_glossiness");
	m_ids_to_names[HAIR_BOOST_REFLECTION] = pair("", "boost_reflection");
	m_ids_to_names[HAIR_BOOST_REFLECTION_SHADER] = pair("outColor[0]", "boost_reflection");
	m_ids_to_names[HAIR_BOOST_TRANSMISSION] = pair("", "boost_transmission");
	m_ids_to_names[HAIR_BOOST_TRANSMISSION_SHADER] = pair("outColor[0]", "boost_transmission");


}

