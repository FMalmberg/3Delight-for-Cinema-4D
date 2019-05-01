// example for a complex channel shader with custom areas
// and animated preview
#include "c4d.h"
#include "c4d_symbols.h"
#include "dl_sky.h"
#include "IDs.h"

class DLPlace2DTexture : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(DLPlace2DTexture); }
};

Bool DLPlace2DTexture::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();
	data->SetFloat(SKY_INTENSITY, 1.0);
	data->SetFloat(SKY_TURBIDITY, 3.0);
	data->SetVector(SKY_GROUND_COLOR, HSVToRGB(Vector(0,0,0.1)));
	data->SetFloat(SKY_ELEVATION, 45.0);
	data->SetFloat(SKY_AZIMUTH, 90.0);
	data->SetVector(SKY_TINT, HSVToRGB(Vector(0, 0, 1)));
	data->SetVector(SKY_SUN_TINT, HSVToRGB(Vector(0, 0, 1)));
	data->SetFloat(SKY_SUN_SIZE, 0.51);

	data->SetBool(SKY_DRAW_SUN_DISK, true);
	data->SetBool(SKY_DRAW_GROUND, false);
	return true;
}


Vector DLPlace2DTexture::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterSkyTexture(void)
{
	//GePrint("Register standard shader");
	// be sure to use a unique ID obtained from www.plugincafe.com
	return RegisterShaderPlugin(DL_SKY, "Sky"_s, 0, DLPlace2DTexture::Alloc, "dl_sky"_s, 0);
}
