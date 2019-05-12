#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLColorCorrection_Translator.h"
#include "dl_color_correction.h"
#include <assert.h>
#include <utility>

Delight_ColorCorrection::Delight_ColorCorrection()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlColorCorrection.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	m_ids_to_names[Shader_Path] = make_pair("", &c_shaderpath[0]);
	m_ids_to_names[INPUT_COLOR] = make_pair("", "input");
	m_ids_to_names[INPUT_COLOR_SHADER] = make_pair("outColor", "input");
	m_ids_to_names[INPUT_MASK] = make_pair("", "mask");
	m_ids_to_names[INPUT_MASK_SHADER] = make_pair("outColor[0]", "mask");

	m_ids_to_names[CORRECTION_GAMMA] = make_pair("", "gamma");
	m_ids_to_names[CORRECTION_GAMMA_SHADER] = make_pair("outColor[0]", "gamma");
	m_ids_to_names[CORRECTION_HUE_SHIFT] = make_pair("", "hueShift");
	m_ids_to_names[CORRECTION_HUE_SHIFT_SHADER] = make_pair("outColor[0]", "hueShift");
	m_ids_to_names[CORRECTION_SATURATION] = make_pair("", "saturation");
	m_ids_to_names[CORRECTION_SATURATION_SHADER] = make_pair("outColor[0]", "saturation");
	m_ids_to_names[CORRECTION_VIBRANCE] = make_pair("", "vibrance");
	m_ids_to_names[CORRECTION_VIBRANCE_SHADER] = make_pair("outColor[0]", "vibrance");
	m_ids_to_names[CORRECTION_CONSTRAST] = make_pair("", "contrast");
	m_ids_to_names[CORRECTION_CONSTRAST_SHADER] = make_pair("outColor[0]", "contrast");
	m_ids_to_names[CORRECTION_CONSTRAST_PIVOT] = make_pair("", "contrastPivot");
	m_ids_to_names[CORRECTION_CONSTRAST_PIVOT_SHADER] = make_pair("outColor[0]", "contrastPivot");
	m_ids_to_names[CORRECTION_EXPOSURE] = make_pair("", "exposure");
	m_ids_to_names[CORRECTION_EXPOSURE_SHADER] = make_pair("outColor[0]", "exposure");
	m_ids_to_names[CORRECTION_GAIN] = make_pair("", "gain");
	m_ids_to_names[CORRECTION_GAIN_SHADER] = make_pair("outColor", "gain");
	m_ids_to_names[CORRECTION_OFFSET] = make_pair("", "offset");
	m_ids_to_names[CORRECTION_OFFSET_SHADER] = make_pair("outColor", "offset");
	m_ids_to_names[CORRECTION_INVERT] = make_pair("", "invert");
}

