#include "c4d.h"
#include "c4d_symbols.h"
#include "dl_color_correction.h"
#include "IDs.h"

class DLColorCorrection : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(DLColorCorrection); }
};

Bool DLColorCorrection::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();
	data->SetVector(INPUT_COLOR, HSVToRGB(Vector(1, 0, 1)));
	data->SetFloat(INPUT_MASK, 1.0);
	data->SetFloat(CORRECTION_GAMMA, 1.0);
	data->SetFloat(CORRECTION_HUE_SHIFT, 0);
	data->SetFloat(CORRECTION_SATURATION, 1.0);
	data->SetFloat(CORRECTION_VIBRANCE, 1.0);
	data->SetFloat(CORRECTION_CONSTRAST, 1.0);
	data->SetFloat(CORRECTION_CONSTRAST_PIVOT, 0.180);
	data->SetFloat(CORRECTION_EXPOSURE, 0);
	data->SetVector(CORRECTION_GAIN, HSVToRGB(Vector(1, 0, 1)));
	data->SetVector(CORRECTION_OFFSET, HSVToRGB(Vector(1, 0, 0)));
	data->SetBool(CORRECTION_INVERT, false);

	return true;
}


Vector DLColorCorrection::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterColorCorrectionTexture(void)
{
	return RegisterShaderPlugin(DL_COLORCORRECTION, "Color Correction"_s, 0, DLColorCorrection::Alloc, "dl_color_correction"_s, 0);
}
