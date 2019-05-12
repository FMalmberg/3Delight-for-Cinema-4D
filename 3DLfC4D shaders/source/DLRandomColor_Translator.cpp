#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLRandomColorTranslator.h"
#include "dl_random_color.h"
#include <assert.h>
#include <utility>

Delight_RandomColor::Delight_RandomColor()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlRandomInputColor.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	m_ids_to_names[Shader_Path] = make_pair("", &c_shaderpath[0]);
	m_ids_to_names[INPUT_COLOR1] = make_pair("", "color_1");
	m_ids_to_names[INPUT_COLOR1_SHADER] = make_pair("outColor", "color_1");
	m_ids_to_names[INPUT_IMPORTANCE1] = make_pair("", "importance_1");
	m_ids_to_names[INPUT_IMPORTANCE1_SHADER] = make_pair("outColor[0]", "importance_1");

	m_ids_to_names[INPUT_COLOR2] = make_pair("", "color_2");
	m_ids_to_names[INPUT_COLOR2_SHADER] = make_pair("outColor", "color_2");
	m_ids_to_names[INPUT_IMPORTANCE2] = make_pair("", "importance_2");
	m_ids_to_names[INPUT_IMPORTANCE2_SHADER] = make_pair("outColor[0]", "importance_2");

	m_ids_to_names[INPUT_COLOR3] = make_pair("", "color_3");
	m_ids_to_names[INPUT_COLOR3_SHADER] = make_pair("outColor", "color_3");
	m_ids_to_names[INPUT_IMPORTANCE3] = make_pair("", "importance_3");
	m_ids_to_names[INPUT_IMPORTANCE3_SHADER] = make_pair("outColor[0]", "importance_3");

	m_ids_to_names[INPUT_COLOR4] = make_pair("", "color_4");
	m_ids_to_names[INPUT_COLOR4_SHADER] = make_pair("outColor", "color_4");
	m_ids_to_names[INPUT_IMPORTANCE4] = make_pair("", "importance_4");
	m_ids_to_names[INPUT_IMPORTANCE4_SHADER] = make_pair("outColor[0]", "importance_4");

	m_ids_to_names[INPUT_COLOR5] = make_pair("", "color_5");
	m_ids_to_names[INPUT_COLOR5_SHADER] = make_pair("outColor", "color_5");
	m_ids_to_names[INPUT_IMPORTANCE5] = make_pair("", "importance_5");
	m_ids_to_names[INPUT_IMPORTANCE5_SHADER] = make_pair("outColor[0]", "importance_5");
	
}

