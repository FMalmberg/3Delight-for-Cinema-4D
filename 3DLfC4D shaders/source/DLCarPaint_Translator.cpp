
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
	m_ids_to_names[Shader_Path] = pair("", &c_shaderpath[0]);
	m_ids_to_names[COATING_LAYER_THICKNESS] = pair("Cout[0]", "coating_thickness");
	m_ids_to_names[COATING_LAYER_THICKNESS_SHADER] = pair("Cout[0]", "coating_thickness");
	m_ids_to_names[COATING_LAYER_COLOR] = pair("Cout", "coating_color");
	m_ids_to_names[COATING_LAYER_COLOR_SHADER] = pair("Cout", "coating_color");
	m_ids_to_names[COATING_LAYER_ROUGHNESS] = pair("Cout[0]", "coating_roughness");
	m_ids_to_names[COATING_LAYER_ROUGHNESS_SHADER] = pair("Cout[0]", "coating_roughness");
	m_ids_to_names[COATING_LAYER_SPECULAR_LEVEL] = pair("Cout[0]", "coating_specular_level");
	m_ids_to_names[COATING_LAYER_SPECULAR_LEVEL_SHADER] = pair("Cout[0]", "coating_specular_level");

	m_ids_to_names[FLAKES_DENSITY] = pair("Cout[0]", "flake_density");
	m_ids_to_names[FLAKES_DENSITY_SHADER] = pair("Cout[0]", "flake_density");
	m_ids_to_names[FLAKES_COLOR] = pair("Cout", "flake_color");
	m_ids_to_names[FLAKES_COLOR_SHADER] = pair("Cout", "flake_color");
	m_ids_to_names[FLAKES_ROUGHNESS] = pair("Cout[0]", "flake_roughness");
	m_ids_to_names[FLAKES_ROUGHNESS_SHADER] = pair("Cout[0]", "flake_roughness");
	m_ids_to_names[FLAKES_SCALE] = pair("Cout[0]", "flake_scale");
	m_ids_to_names[FLAKES_SCALE_SHADER] = pair("Cout[0]", "flake_scale");
	m_ids_to_names[FLAKES_RANDOMNESS] = pair("Cout", "flake_randomness");
	m_ids_to_names[FLAKES_RANDOMNESS_SHADER] = pair("Cout", "flake_randomness");

	m_ids_to_names[BASE_LAYER_COLOR] = pair("Cout", "i_color");
	m_ids_to_names[BASE_LAYER_COLOR_SHADER] = pair("Cout", "i_color");
	m_ids_to_names[BASE_LAYER_ROUGHNESS] = pair("Cout[0]", "roughness");
	m_ids_to_names[BASE_LAYER_ROUGHNESS_SHADER] = pair("Cout", "roughness");
	m_ids_to_names[BASE_LAYER_SPECULAR_LEVEL] = pair("Cout[0]", "specular_level");
	m_ids_to_names[BASE_LAYER_SPECULAR_LEVEL_SHADER] = pair("Cout[0]", "specular_level");
	m_ids_to_names[BASE_LAYER_METALLIC] = pair("Cout[0]", "metallic");
	m_ids_to_names[BASE_LAYER_METALLIC_SHADER] = pair("Cout[0]", "metallic");

	m_ids_to_names[BUMP_TYPE] = pair("", "disp_normal_bump_type");
	m_ids_to_names[BUMP_VALUE] = pair("Cout", "disp_normal_bump_value");
	m_ids_to_names[BUMP_INTENSITY] = pair("", "disp_normal_bump_intensity");
	m_ids_to_names[BUMP_VALUE] = pair("Cout", "disp_normal_bump_value");
	m_ids_to_names[BUMP_LAYERS_AFFECTED] = pair("", "normal_bump_affect_layer");

}

