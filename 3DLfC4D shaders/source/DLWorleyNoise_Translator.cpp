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
	m_ids_to_names[Shader_Path] = make_pair("", &c_shaderpath[0]);
	m_ids_to_names[WORLEY_NOISE_SCALE] = make_pair("", "scale");
	m_ids_to_names[WORLEY_NOISE_SCALE_SHADER] = make_pair("outColor[0]", "scale");
	m_ids_to_names[WORLEY_NOISE_DISTANCE_TYPE] = make_pair("", "distance_type");
	m_ids_to_names[WORLEY_NOISE_BLEND_MODE] = make_pair("", "blend_type");
	m_ids_to_names[WORLEY_NOISE_OUTPUT_TYPE] = make_pair("", "output_type");
	m_ids_to_names[WORLEY_NOISE_MINKOWSKI_NUMBER] = make_pair("", "minkowski_k");
	m_ids_to_names[WORLEY_NOISE_MINKOWSKI_NUMBER_SHADER] = make_pair("outColor[0]", "minkowski_k");
	m_ids_to_names[WORLEY_NOISE_COLOR] = make_pair("", "i_color");
	m_ids_to_names[WORLEY_NOISE_COLOR_SHADER] = make_pair("outColor", "i_color");
	m_ids_to_names[WORLEY_NOISE_LAYERS] = make_pair("", "layers");
	m_ids_to_names[WORLEY_NOISE_PERSISTENCE] = make_pair("", "layer_persistence");
	m_ids_to_names[WORLEY_NOISE_PERSISTENCE_SHADER] = make_pair("outColor[0]", "layer_persistence");
	m_ids_to_names[WORLEY_NOISE_LAYERS_SCALE] = make_pair("", "layer_scale");
	m_ids_to_names[WORLEY_NOISE_LAYERS_SCALE_SHADER] = make_pair("outColor[0]", "layer_scale");
	m_ids_to_names[WORLEY_NOISE_DENSITY] = make_pair("", "density");
	m_ids_to_names[WORLEY_NOISE_DENSITY_SHADER] = make_pair("outColor[0]", "density");
	m_ids_to_names[WORLEY_NOISE_TIME] = make_pair("", "i_time");
	m_ids_to_names[WORLEY_NOISE_TIME_SHADER] = make_pair("outColor[0]", "i_time");

	m_ids_to_names[WORLEY_NOISE_RANDOM_POSITION] = make_pair("", "random_pos");
	m_ids_to_names[WORLEY_NOISE_RANDOM_POSITION_SHADER] = make_pair("outColor[0]", "random_pos");
	m_ids_to_names[WORLEY_NOISE_RANDOM_BRIGHTNESS] = make_pair("", "random_bright");
	m_ids_to_names[WORLEY_NOISE_RANDOM_BRIGHTNESS_SHADER] = make_pair("outColor[0]", "random_bright");
	m_ids_to_names[WORLEY_NOISE_ADJUST_AMPLITUDE] = make_pair("", "amplitude");
	m_ids_to_names[WORLEY_NOISE_ADJUST_AMPLITUDE_SHADER] = make_pair("outColor[0]", "amplitude");
	m_ids_to_names[WORLEY_NOISE_ADJUST_CONTRAST] = make_pair("", "contrast");
	m_ids_to_names[WORLEY_NOISE_ADJUST_CONTRAST_SHADER] = make_pair("outColor[0]", "contrast");
	m_ids_to_names[WORLEY_NOISE_ADJUST_INVERT] = make_pair("", "invert");
	m_ids_to_names[WORLEY_NOISE_SPACE] = make_pair("", "space");
}

