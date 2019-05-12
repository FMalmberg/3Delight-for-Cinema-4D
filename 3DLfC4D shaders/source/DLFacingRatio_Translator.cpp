#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLFacingRatio_Translator.h"
#include "dl_facing_ratio.h"
#include <assert.h>
#include <utility>

Delight_FacingRatio::Delight_FacingRatio()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlFacingRatio.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	m_ids_to_names[Shader_Path] = make_pair("", &c_shaderpath[0]);
	m_ids_to_names[FACING_RATIO_COLOR_EDGE] = make_pair("", "color_edge");
	m_ids_to_names[FACING_RATIO_COLOR_EDGE_SHADER] = make_pair("outColor", "color_edge");
	m_ids_to_names[FACING_RATIO_COLOR_CENTER] = make_pair("", "color_center");
	m_ids_to_names[FACING_RATIO_COLOR_CENTER_SHADER] = make_pair("outColor", "color_center");
	m_ids_to_names[FACING_RATIO_BIAS] = make_pair("", "bias");
	m_ids_to_names[FACING_RATIO_BIAS_SHADER] = make_pair("outColor[0]", "bias");
	m_ids_to_names[FACING_RATIO_CONTRAST] = make_pair("", "contrast");
	m_ids_to_names[FACING_RATIO_CONTRAST_SHADER] = make_pair("outColor[0]", "contrast");

	m_ids_to_names[BUMP_TYPE] = make_pair("", "disp_normal_bump_type");
	m_ids_to_names[BUMP_INTENSITY] = make_pair("", "disp_normal_bump_intensity");
	m_ids_to_names[BUMP_VALUE] = make_pair("outColor", "disp_normal_bump_value");

}

