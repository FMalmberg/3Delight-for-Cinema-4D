#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLColorBlend_Translator.h"
#include "dl_color_blend.h"
#include <assert.h>
#include <utility>

Delight_ColorBlend::Delight_ColorBlend()
{
	
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlColorBlend.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	m_ids_to_names[Shader_Path] = make_pair("", &c_shaderpath[0]);
	m_ids_to_names[BLEND_MODE] = make_pair("", "blendMode");
	m_ids_to_names[BLEND_FACTOR] = make_pair("", "blend");
	m_ids_to_names[BLEND_FACTOR_SHADER] = make_pair("outColor[0]", "blend");
	m_ids_to_names[BLEND_FOREGROUND] = make_pair("", "fg");
	m_ids_to_names[BLEND_FOREGROUND_SHADER] = make_pair("outColor", "fg");
	m_ids_to_names[BLEND_BACKGROUND] = make_pair("", "bg");
	m_ids_to_names[BLEND_BACKGROUND_SHADER] = make_pair("outColor", "bg");

}

