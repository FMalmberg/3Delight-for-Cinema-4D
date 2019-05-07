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

	m_ids_to_names[Shader_Path] = make_pair("", &c_shaderpath[0]);
	m_ids_to_names[HAIR_COLOR_MELANIN] = make_pair("", "eumelanine");
	m_ids_to_names[HAIR_COLOR_MELANIN_SHADER] = make_pair("outColor[0]", "eumelanine");
	m_ids_to_names[HAIR_COLOR_MELANIN_RED] = make_pair("", "phenomelanine");
	m_ids_to_names[HAIR_COLOR_MELANIN_RED_SHADER] = make_pair("outColor[0]", "phenomelanine");
	m_ids_to_names[HAIR_COLOR_DYE] = make_pair("", "i_color");
	m_ids_to_names[HAIR_COLOR_DYE_SHADER] = make_pair("outColor", "i_color");
	m_ids_to_names[HAIR_COLOR_DYE_WEIGHT] = make_pair("", "dye_weight");
	m_ids_to_names[HAIR_COLOR_DYE_WEIGHT_SHADER] = make_pair("outColor[0]", "dye_weight");

	m_ids_to_names[HAIR_LOOK_REFLECTIVITY] = make_pair("", "reflectivity");
	m_ids_to_names[HAIR_LOOK_REFLECTIVITY_SHADER] = make_pair("outColor[0]", "reflectivity");
	m_ids_to_names[HAIR_LOOK_ROUGHNESS_ALONG] = make_pair("", "longitudinal_roughness");
	m_ids_to_names[HAIR_LOOK_ROUGHNESS_ALONG_SHADER] = make_pair("outColor[0]", "longitudinal_roughness");
	m_ids_to_names[HAIR_LOOK_ROUGHNESS_AROUND] = make_pair("", "azimuthal_roughness");
	m_ids_to_names[HAIR_LOOK_ROUGHNESS_AROUND_SHADER] = make_pair("outColor[0]", "azimuthal_roughness");
	m_ids_to_names[HAIR_LOOK_SYNTHETIC_FIBER] = make_pair("", "synthetic");
	m_ids_to_names[HAIR_LOOK_SYNTHETIC_FIBER_SHADER] = make_pair("outColor[0]", "synthetic");

	m_ids_to_names[HAIR_VARIATION_MELANIN] = make_pair("", "variation_melanin");
	m_ids_to_names[HAIR_VARIATION_MELANIN_SHADER] = make_pair("outColor[0]", "variation_melanin");
	m_ids_to_names[HAIR_VARIATION_MELANIN_RED] = make_pair("", "variation_melanin_red");
	m_ids_to_names[HAIR_VARIATION_MELANIN_RED_SHADER] = make_pair("outColor[0]", "variation_melanin_red");
	m_ids_to_names[HAIR_VARIATION_WHITE_HAIR] = make_pair("", "variation_white_hair");
	m_ids_to_names[HAIR_VARIATION_WHITE_HAIR_SHADER] = make_pair("outColor[0]", "variation_white_hair");
	m_ids_to_names[HAIR_VARIATION_DYE_HUE] = make_pair("", "variation_dye_hue");
	m_ids_to_names[HAIR_VARIATION_DYE_HUE_SHADER] = make_pair("outColor[0]", "variation_dye_hue");
	m_ids_to_names[HAIR_VARIATION_DYE_SATURATION] = make_pair("", "variation_dye_saturation");
	m_ids_to_names[HAIR_VARIATION_DYE_SATURATION_SHADER] = make_pair("outColor[0]", "variation_dye_saturation");
	m_ids_to_names[HAIR_VARIATION_DYE_VALUE] = make_pair("", "variation_dye_value");
	m_ids_to_names[HAIR_VARIATION_DYE_VALUE_SHADER] = make_pair("outColor[0]", "variation_dye_value");
	m_ids_to_names[HAIR_VARIATION_ROUGHNESS] = make_pair("", "variation_roughness");
	m_ids_to_names[HAIR_VARIATION_ROUGHNESS_SHADER] = make_pair("outColor[0]", "variation_roughness");
	m_ids_to_names[HAIR_VARIATION_REFLECTIVITY] = make_pair("", "variation_reflectivity");
	m_ids_to_names[HAIR_VARIATION_REFLECTIVITY_SHADER] = make_pair("outColor[0]", "variation_reflectivity");

	m_ids_to_names[HAIR_BOOST_GLOSSINESS] = make_pair("", "boost_glossiness");
	m_ids_to_names[HAIR_BOOST_GLOSSINESS_SHADER] = make_pair("outColor[0]", "boost_glossiness");
	m_ids_to_names[HAIR_BOOST_REFLECTION] = make_pair("", "boost_reflection");
	m_ids_to_names[HAIR_BOOST_REFLECTION_SHADER] = make_pair("outColor[0]", "boost_reflection");
	m_ids_to_names[HAIR_BOOST_TRANSMISSION] = make_pair("", "boost_transmission");
	m_ids_to_names[HAIR_BOOST_TRANSMISSION_SHADER] = make_pair("outColor[0]", "boost_transmission");


}

