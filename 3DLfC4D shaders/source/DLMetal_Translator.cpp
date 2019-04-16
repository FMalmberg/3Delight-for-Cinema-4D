#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLMetal_Translator.h"
#include "dl_metal.h"
#include <assert.h>
#include <utility>

Delight_Metal::Delight_Metal()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlMetal.oso"));
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

	m_ids_to_names[METAL_LAYER_COLOR] = pair("Cout", "i_color");
	m_ids_to_names[METAL_LAYER_COLOR_SHADER] = pair("Cout", "i_color");
	m_ids_to_names[METAL_LAYER_EDGE_COLOR] = pair("Cout", "edge_color");
	m_ids_to_names[METAL_LAYER_EDGE_COLOR_SHADER] = pair("Cout", "edge_color");
	m_ids_to_names[METAL_LAYER_ROUGHNESS] = pair("Cout[0]", "roughness");
	m_ids_to_names[METAL_LAYER_ROUGHNESS_SHADER] = pair("Cout", "roughness");
	m_ids_to_names[METAL_LAYER_ANISOTROPY] = pair("Cout[0]", "anisotropy");
	m_ids_to_names[METAL_LAYER_ANISOTROPY_SHADER] = pair("Cout[0]", "anisotropy");
	m_ids_to_names[METAL_LAYER_ANISOTROPY_DIRECTION] = pair("Cout", "anisotropy_direction");
	m_ids_to_names[METAL_LAYER_ANISOTROPY_DIRECTION_SHADER] = pair("Cout", "anisotropy_direction");
	m_ids_to_names[METAL_LAYER_OPACITY] = pair("Cout[0]", "opacity");
	m_ids_to_names[METAL_LAYER_OPACITY_SHADER] = pair("Cout[0]", "opacity");
	m_ids_to_names[METAL_LAYER_TEMPERED_METAL] = pair("", "tempered_metal_on");
	m_ids_to_names[METAL_LAYER_OXIDE_THICKNESS] = pair("Cout[0]", "oxide_thickness");
	m_ids_to_names[METAL_LAYER_OXIDE_THICKNESS_SHADER] = pair("Cout[0]", "oxide_thickness");
	m_ids_to_names[METAL_LAYER_OXIDE_IOR] = pair("Cout[0]", "oxide_ior");
	m_ids_to_names[METAL_LAYER_OXIDE_IOR_SHADER] = pair("Cout[0]", "oxide_ior");

	m_ids_to_names[BUMP_TYPE] = pair("", "disp_normal_bump_type");
	m_ids_to_names[BUMP_INTENSITY] = pair("", "disp_normal_bump_intensity");
	m_ids_to_names[BUMP_LAYERS_AFFECTED] = pair("", "normal_bump_affect_layer");
}

