// example for a complex channel shader with custom areas
// and animated preview
#include "c4d.h"
#include "c4d_symbols.h"
#include "dl_flakes.h"
#include "IDs.h"

class DLFlakes : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(DLFlakes); }
};

Bool DLFlakes::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();
	data->SetInt32(FLAKES_DENSITY, 0.5);
	data->SetInt32(FLAKES_SCALE, 1.0);
	data->SetFloat(FLAKES_RANDOMNESS, 0.5);
	data->SetInt32(FLAKES_LAYERS, 1);
	return true;
}


Vector DLFlakes::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterFlakesTexture(void)
{
	return RegisterShaderPlugin(DL_Flakes, "Flakes"_s, 0, DLFlakes::Alloc, "dl_flakes"_s, 0);
}
