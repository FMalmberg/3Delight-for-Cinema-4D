// example for a complex channel shader with custom areas
// and animated preview
#include "c4d.h"
#include "c4d_symbols.h"
#include "dl_facing_ratio.h"
#include "IDs.h"

class DLFacingRatio : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(DLFacingRatio); }
};

Bool DLFacingRatio::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();
	data->SetVector(FACING_RATIO_COLOR_EDGE, HSVToRGB(Vector(288.0/360.0, 0, 1)));
	data->SetVector(FACING_RATIO_COLOR_CENTER, HSVToRGB(Vector(288.0/360.0, 0, 0)));
	data->SetFloat(FACING_RATIO_BIAS, 0.5);
	data->SetFloat(FACING_RATIO_CONTRAST, 0.5);
	data->SetInt32(BUMP_TYPE, BUMP_MAP);
	data->SetFloat(BUMP_INTENSITY, 1);
	return true;
}


Vector DLFacingRatio::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterFacingRatioTexture(void)
{
	return RegisterShaderPlugin(DL_FACINGRATIO, "Facing Ratio"_s, 0, DLFacingRatio::Alloc, "dl_facing_ratio"_s, 0);
}
