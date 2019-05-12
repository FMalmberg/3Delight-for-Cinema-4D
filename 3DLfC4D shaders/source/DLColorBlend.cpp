// example for a complex channel shader with custom areas
// and animated preview
#include "c4d.h"
#include "c4d_symbols.h"
#include "dl_color_blend.h"
#include "IDs.h"

class DLColorBlend : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(DLColorBlend); }
};

Bool DLColorBlend::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();
	data->SetInt32(BLEND_MODE, BLEND_MODE_OVER);
	data->SetFloat(BLEND_FACTOR, 1.0);
	data->SetVector(BLEND_FOREGROUND, HSVToRGB(Vector(0,0,1)));
	data->SetVector(BLEND_BACKGROUND, HSVToRGB(Vector(0, 0, 0)));

	return true;
}


Vector DLColorBlend::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterColorBlendTexture(void)
{
	return RegisterShaderPlugin(DL_COLORBLEND, "Color Blend"_s, 0, DLColorBlend::Alloc, "dl_color_blend"_s, 0);
}
