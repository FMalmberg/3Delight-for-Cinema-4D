#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLFlakes_translator.h"
#include "dl_flakes.h"
#include <assert.h>
#include <utility>

Delight_Flakes::Delight_Flakes()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlFlakes.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	m_ids_to_names[Shader_Path] = pair("", &c_shaderpath[0]);
	m_ids_to_names[FLAKES_DENSITY] = pair("", "density");
	m_ids_to_names[FLAKES_DENSITY_SHADER] = pair("outColor[0]", "density");
	m_ids_to_names[FLAKES_SCALE] = pair("", "scale");
	m_ids_to_names[FLAKES_SCALE_SHADER] = pair("outColor[0]", "scale");
	m_ids_to_names[FLAKES_RANDOMNESS] = pair("", "randomness");
	m_ids_to_names[FLAKES_RANDOMNESS_SHADER] = pair("outColor[0]", "randomness");
	m_ids_to_names[FLAKES_LAYERS] = pair("", "layers");
}

