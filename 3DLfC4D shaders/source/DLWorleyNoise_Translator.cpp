#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLWorleyNoise_Translator.h"
#include "dl_worley_noise.h"
#include <assert.h>
#include <utility>

Delight_WorleyNoise::Delight_WorleyNoise()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlWorleyNoise.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	m_ids_to_names[Shader_Path] = pair("", &c_shaderpath[0]);
	m_ids_to_names[WORLEY_NOISE_SCALE] = pair("", "scale");
	m_ids_to_names[WORLEY_NOISE_SCALE_SHADER] = pair("outColor[0]", "scale");
	m_ids_to_names[WORLEY_NOISE_DISTANCE_TYPE] = pair("", "distance_type");
	m_ids_to_names[WORLEY_NOISE_BLEND_MODE] = pair("", "blend_type");
	m_ids_to_names[WORLEY_NOISE_OUTPUT_TYPE] = pair("", "output_type");
	m_ids_to_names[WORLEY_NOISE_MINKOWSKI_NUMBER] = pair("", "minkowski_k");
	m_ids_to_names[WORLEY_NOISE_MINKOWSKI_NUMBER_SHADER] = pair("outColor[0]", "minkowski_k");
	m_ids_to_names[WORLEY_NOISE_COLOR] = pair("", "i_color");
	m_ids_to_names[WORLEY_NOISE_COLOR_SHADER] = pair("outColor", "i_color");
	m_ids_to_names[WORLEY_NOISE_LAYERS] = pair("", "layers");
	m_ids_to_names[WORLEY_NOISE_PERSISTENCE] = pair("", "layer_persistence");
	m_ids_to_names[WORLEY_NOISE_PERSISTENCE_SHADER] = pair("outColor[0]", "layer_persistence");
	m_ids_to_names[WORLEY_NOISE_LAYERS_SCALE] = pair("", "layer_scale");
	m_ids_to_names[WORLEY_NOISE_LAYERS_SCALE_SHADER] = pair("outColor[0]", "layer_scale");
	m_ids_to_names[WORLEY_NOISE_DENSITY] = pair("", "density");
	m_ids_to_names[WORLEY_NOISE_DENSITY_SHADER] = pair("outColor[0]", "density");
	m_ids_to_names[WORLEY_NOISE_TIME] = pair("", "i_time");
	m_ids_to_names[WORLEY_NOISE_TIME_SHADER] = pair("outColor[0]", "i_time");

	m_ids_to_names[WORLEY_NOISE_RANDOM_POSITION] = pair("", "random_pos");
	m_ids_to_names[WORLEY_NOISE_RANDOM_POSITION_SHADER] = pair("outColor[0]", "random_pos");
	m_ids_to_names[WORLEY_NOISE_RANDOM_BRIGHTNESS] = pair("", "random_bright");
	m_ids_to_names[WORLEY_NOISE_RANDOM_BRIGHTNESS_SHADER] = pair("outColor[0]", "random_bright");
	m_ids_to_names[WORLEY_NOISE_ADJUST_AMPLITUDE] = pair("", "amplitude");
	m_ids_to_names[WORLEY_NOISE_ADJUST_AMPLITUDE_SHADER] = pair("outColor[0]", "amplitude");
	m_ids_to_names[WORLEY_NOISE_ADJUST_CONTRAST] = pair("", "contrast");
	m_ids_to_names[WORLEY_NOISE_ADJUST_CONTRAST_SHADER] = pair("outColor[0]", "contrast");
	m_ids_to_names[WORLEY_NOISE_ADJUST_INVERT] = pair("", "invert");
	m_ids_to_names[WORLEY_NOISE_SPACE] = pair("", "space");
}

