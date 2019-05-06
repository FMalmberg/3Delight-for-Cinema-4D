// example for a complex channel shader with custom areas
// and animated preview
#include "c4d.h"
#include "c4d_symbols.h"
#include "dl_worley_noise.h"
#include "IDs.h"

class DLWorleyNoise : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(DLWorleyNoise); }
};

Bool DLWorleyNoise::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();
	data->SetInt32(WORLEY_NOISE_OUTPUT_TYPE, WORLEY_NOISE_OUTPUT_TYPE_CELL_VALUE);
	data->SetInt32(WORLEY_NOISE_DISTANCE_TYPE, WORLEY_NOISE_DISTANCE_TYPE_EUCLIDEAN);
	data->SetFloat(WORLEY_NOISE_MINKOWSKI_NUMBER, 3.0);

	data->SetVector(WORLEY_NOISE_COLOR, HSVToRGB(Vector(0,0,1)));
	data->SetFloat(WORLEY_NOISE_SCALE, 1.0);
	data->SetFloat(WORLEY_NOISE_DENSITY, 1.0);

	data->SetFloat(WORLEY_NOISE_RANDOM_POSITION, 1.0);
	data->SetFloat(WORLEY_NOISE_RANDOM_BRIGHTNESS, 0.0);

	data->SetFloat(WORLEY_NOISE_TIME, 0.0);
	data->SetInt32(WORLEY_NOISE_SPACE, WORLEY_NOISE_SPACE_WORLD);

	data->SetInt32(WORLEY_NOISE_LAYERS, 1);
	data->SetInt32(WORLEY_NOISE_BLEND_MODE, WORLEY_NOISE_BLEND_MODE_OVER);
	data->SetFloat(WORLEY_NOISE_PERSISTENCE, 0.7);
	data->SetFloat(WORLEY_NOISE_LAYERS_SCALE, 0.45);

	data->SetFloat(WORLEY_NOISE_ADJUST_AMPLITUDE, 1.0);
	data->SetFloat(WORLEY_NOISE_ADJUST_CONTRAST, 1.0);
	data->SetBool(WORLEY_NOISE_ADJUST_INVERT, false);

	return true;
}


Vector DLWorleyNoise::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterWorleyNoiseTexture(void)
{
	return RegisterShaderPlugin(DL_WorleyNoise, "Worley Noise"_s, 0, DLWorleyNoise::Alloc, "dl_worley_noise"_s, 0);
}
