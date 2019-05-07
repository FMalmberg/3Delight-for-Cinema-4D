
#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLCarPaint_Translator.h"
#include "dl_car_paint.h"
#include <assert.h>
#include <utility>

Delight_CarPaint::Delight_CarPaint()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlCarPaint.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	m_ids_to_names[Shader_Path] = make_pair("", &c_shaderpath[0]);
	m_ids_to_names[COATING_LAYER_THICKNESS] = make_pair("", "coating_thickness");
	m_ids_to_names[COATING_LAYER_THICKNESS_SHADER] = make_pair("outColor[0]", "coating_thickness");
	m_ids_to_names[COATING_LAYER_COLOR] = make_pair("", "coating_color");
	m_ids_to_names[COATING_LAYER_COLOR_SHADER] = make_pair("outColor", "coating_color");
	m_ids_to_names[COATING_LAYER_ROUGHNESS] = make_pair("", "coating_roughness");
	m_ids_to_names[COATING_LAYER_ROUGHNESS_SHADER] = make_pair("outColor[0]", "coating_roughness");
	m_ids_to_names[COATING_LAYER_SPECULAR_LEVEL] = make_pair("", "coating_specular_level");
	m_ids_to_names[COATING_LAYER_SPECULAR_LEVEL_SHADER] = make_pair("outColor[0]", "coating_specular_level");

	m_ids_to_names[FLAKES_DENSITY] = make_pair("", "flake_density");
	m_ids_to_names[FLAKES_DENSITY_SHADER] = make_pair("outColor[0]", "flake_density");
	m_ids_to_names[FLAKES_COLOR] = make_pair("", "flake_color");
	m_ids_to_names[FLAKES_COLOR_SHADER] = make_pair("outColor", "flake_color");
	m_ids_to_names[FLAKES_ROUGHNESS] = make_pair("", "flake_roughness");
	m_ids_to_names[FLAKES_ROUGHNESS_SHADER] = make_pair("outColor[0]", "flake_roughness");
	m_ids_to_names[FLAKES_SCALE] = make_pair("", "flake_scale");
	m_ids_to_names[FLAKES_SCALE_SHADER] = make_pair("outColor[0]", "flake_scale");
	m_ids_to_names[FLAKES_RANDOMNESS] = make_pair("", "flake_randomness");
	m_ids_to_names[FLAKES_RANDOMNESS_SHADER] = make_pair("outColor[0]", "flake_randomness");

	m_ids_to_names[BASE_LAYER_COLOR] = make_pair("", "i_color");
	m_ids_to_names[BASE_LAYER_COLOR_SHADER] = make_pair("outColor", "i_color");
	m_ids_to_names[BASE_LAYER_ROUGHNESS] = make_pair("", "roughness");
	m_ids_to_names[BASE_LAYER_ROUGHNESS_SHADER] = make_pair("outColor", "roughness");
	m_ids_to_names[BASE_LAYER_SPECULAR_LEVEL] = make_pair("", "specular_level");
	m_ids_to_names[BASE_LAYER_SPECULAR_LEVEL_SHADER] = make_pair("outColor[0]", "specular_level");
	m_ids_to_names[BASE_LAYER_METALLIC] = make_pair("", "metallic");
	m_ids_to_names[BASE_LAYER_METALLIC_SHADER] = make_pair("outColor[0]", "metallic");

	m_ids_to_names[BUMP_TYPE] = make_pair("", "disp_normal_bump_type");
	m_ids_to_names[BUMP_VALUE] = make_pair("outColor", "disp_normal_bump_value");
	m_ids_to_names[BUMP_INTENSITY] = make_pair("", "disp_normal_bump_intensity");
	m_ids_to_names[BUMP_LAYERS_AFFECTED] = make_pair("", "normal_bump_affect_layer");

}

