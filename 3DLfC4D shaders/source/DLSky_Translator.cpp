#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLSky_Translator.h"
#include "dl_sky.h"
#include <assert.h>
#include <utility>

Delight_Sky::Delight_Sky()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlSky.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	m_ids_to_names[Shader_Path] = pair("", &c_shaderpath[0]);
	m_ids_to_names[SKY_INTENSITY] = pair("", "intensity");
	m_ids_to_names[SKY_INTENSITY_SHADER] = pair("outColor[0]", "intensity");
	m_ids_to_names[SKY_TURBIDITY] = pair("", "turbidity");
	m_ids_to_names[SKY_GROUND_COLOR] = pair("", "ground_albedo");
	m_ids_to_names[SKY_ELEVATION] = pair("", "elevation");
	m_ids_to_names[SKY_AZIMUTH] = pair("", "azimuth");
	m_ids_to_names[SKY_TINT] = pair("", "sky_tint");
	m_ids_to_names[SKY_SUN_TINT] = pair("", "sun_tint");
	m_ids_to_names[SKY_SUN_SIZE] = pair("", "sun_size");
	m_ids_to_names[SKY_DRAW_SUN_DISK] = pair("", "sun_enable");
	m_ids_to_names[SKY_DRAW_GROUND] = pair("", "ground_enable");
}

