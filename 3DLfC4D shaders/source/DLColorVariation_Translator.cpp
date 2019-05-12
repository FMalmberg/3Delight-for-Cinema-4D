#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLColorVariation_Translator.h"
#include "dl_color_variation.h"
#include <assert.h>
#include <utility>

Delight_ColorVariation::Delight_ColorVariation()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlColorVariation.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	m_ids_to_names[Shader_Path] = make_pair("", &c_shaderpath[0]);
	m_ids_to_names[INPUT_COLOR] = make_pair("", "_color");
	m_ids_to_names[INPUT_COLOR_SHADER] = make_pair("outColor", "_color");
	m_ids_to_names[VARIATION_HUE] = make_pair("", "hueVariation");
	m_ids_to_names[VARIATION_HUE_SHADER] = make_pair("outColor[0]", "hueVariation");
	m_ids_to_names[VARIATION_SATURATION] = make_pair("", "saturationVariation");
	m_ids_to_names[VARIATION_SATURATION_SHADER] = make_pair("outColor[0]", "saturationVariation");
    m_ids_to_names[VARIATION_BRIGHTNESS] = make_pair("", "brightnessVariation");
	m_ids_to_names[VARIATION_BRIGHTNESS_SHADER] = make_pair("outColor[0]", "brightnessVariation");

	m_ids_to_names[RANDOMNESS_TYPE] = make_pair("", "randomness");
	m_ids_to_names[SOURCE_TYPE] = make_pair("outColor[0]", "randomSource");
	m_ids_to_names[SEED] = make_pair("", "seed");
}

