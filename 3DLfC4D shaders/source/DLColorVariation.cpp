// example for a complex channel shader with custom areas
// and animated preview
#include "c4d.h"
#include "c4d_symbols.h"
#include "dl_color_variation.h"
#include "IDs.h"

class DLColorVariation : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(DLColorVariation); }
};

Bool DLColorVariation::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();
	data->SetVector(INPUT_COLOR, HSVToRGB(Vector(1, 0, 240.0/360.0)));
	data->SetFloat(VARIATION_HUE, 0);
	data->SetFloat(VARIATION_SATURATION, 0);
	data->SetFloat(VARIATION_BRIGHTNESS, 0);
	data->SetFloat(RANDOMNESS_TYPE, RANDOMNESS_UNIFORM);
	data->SetFloat(SOURCE_TYPE, SOURCE_TYPE_OBJECT);
	data->SetInt32(SEED, 0);
	return true;
}


Vector DLColorVariation::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterColorVariationTexture(void)
{
	return RegisterShaderPlugin(DL_COLORVARIATION, "Color Variation"_s, 0, DLColorVariation::Alloc, "dl_color_variation"_s, 0);
}
