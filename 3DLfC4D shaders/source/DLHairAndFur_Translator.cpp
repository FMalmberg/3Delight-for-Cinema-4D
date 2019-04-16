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
	m_ids_to_names[HAIR_COLOR_MELANIN] = pair("Cout[0]", "eumelanine");
	m_ids_to_names[HAIR_COLOR_MELANIN_SHADER] = pair("Cout[0]", "eumelanine");
	m_ids_to_names[HAIR_COLOR_MELANIN_RED] = pair("Cout[0]", "phenomelanine");
	m_ids_to_names[HAIR_COLOR_MELANIN_RED_SHADER] = pair("Cout[0]", "phenomelanine");
	m_ids_to_names[HAIR_COLOR_DYE] = pair("Cout", "i_color");
	m_ids_to_names[HAIR_COLOR_DYE_SHADER] = pair("Cout", "i_color");
	m_ids_to_names[HAIR_COLOR_DYE_WEIGHT] = pair("Cout[0]", "dye_weight");
	m_ids_to_names[HAIR_COLOR_DYE_WEIGHT_SHADER] = pair("Cout[0]", "dye_weight");

	m_ids_to_names[HAIR_LOOK_REFLECTIVITY] = pair("Cout[0]", "reflectivity");
	m_ids_to_names[HAIR_LOOK_REFLECTIVITY_SHADER] = pair("Cout[0]", "reflectivity");
	m_ids_to_names[HAIR_LOOK_ROUGHNESS_ALONG] = pair("Cout[0]", "longitudinal_roughness");
	m_ids_to_names[HAIR_LOOK_ROUGHNESS_ALONG_SHADER] = pair("Cout[0]", "longitudinal_roughness");
	m_ids_to_names[HAIR_LOOK_ROUGHNESS_AROUND] = pair("Cout[0]", "azimuthal_roughness");
	m_ids_to_names[HAIR_LOOK_ROUGHNESS_AROUND_SHADER] = pair("Cout[0]", "azimuthal_roughness");
	m_ids_to_names[HAIR_LOOK_SYNTHETIC_FIBER] = pair("Cout[0]", "synthetic");
	m_ids_to_names[HAIR_LOOK_SYNTHETIC_FIBER_SHADER] = pair("Cout[0]", "synthetic");

	m_ids_to_names[HAIR_VARIATION_MELANIN] = pair("Cout[0]", "variation_melanin");
	m_ids_to_names[HAIR_VARIATION_MELANIN_SHADER] = pair("Cout[0]", "variation_melanin");
	m_ids_to_names[HAIR_VARIATION_MELANIN_RED] = pair("Cout[0]", "variation_melanin_red");
	m_ids_to_names[HAIR_VARIATION_MELANIN_RED_SHADER] = pair("Cout[0]", "variation_melanin_red");
	m_ids_to_names[HAIR_VARIATION_WHITE_HAIR] = pair("Cout[0]", "variation_white_hair");
	m_ids_to_names[HAIR_VARIATION_WHITE_HAIR_SHADER] = pair("Cout[0]", "variation_white_hair");
	m_ids_to_names[HAIR_VARIATION_DYE_HUE] = pair("Cout[0]", "variation_dye_hue");
	m_ids_to_names[HAIR_VARIATION_DYE_HUE_SHADER] = pair("Cout[0]", "variation_dye_hue");
	m_ids_to_names[HAIR_VARIATION_DYE_SATURATION] = pair("Cout[0]", "variation_dye_saturation");
	m_ids_to_names[HAIR_VARIATION_DYE_SATURATION_SHADER] = pair("Cout[0]", "variation_dye_saturation");
	m_ids_to_names[HAIR_VARIATION_DYE_VALUE] = pair("Cout[0]", "variation_dye_value");
	m_ids_to_names[HAIR_VARIATION_DYE_VALUE_SHADER] = pair("Cout[0]", "variation_dye_value");
	m_ids_to_names[HAIR_VARIATION_ROUGHNESS] = pair("Cout[0]", "variation_roughness");
	m_ids_to_names[HAIR_VARIATION_ROUGHNESS_SHADER] = pair("Cout[0]", "variation_roughness");
	m_ids_to_names[HAIR_VARIATION_REFLECTIVITY] = pair("Cout[0]", "variation_reflectivity");
	m_ids_to_names[HAIR_VARIATION_REFLECTIVITY_SHADER] = pair("Cout[0]", "variation_reflectivity");

	m_ids_to_names[HAIR_BOOST_GLOSSINESS] = pair("Cout[0]", "boost_glossiness");
	m_ids_to_names[HAIR_BOOST_GLOSSINESS_SHADER] = pair("Cout[0]", "boost_glossiness");
	m_ids_to_names[HAIR_BOOST_REFLECTION] = pair("Cout[0]", "boost_reflection");
	m_ids_to_names[HAIR_BOOST_REFLECTION_SHADER] = pair("Cout[0]", "boost_reflection");
	m_ids_to_names[HAIR_BOOST_TRANSMISSION] = pair("Cout[0]", "boost_transmission");
	m_ids_to_names[HAIR_BOOST_TRANSMISSION_SHADER] = pair("Cout[0]", "boost_transmission");


}

