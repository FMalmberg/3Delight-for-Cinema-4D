// example for a complex channel shader with custom areas
// and animated preview
#include "c4d.h"
#include "c4d_symbols.h"
#include "dl_random_color.h"
#include "IDs.h"

class DLRandomColor : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(DLRandomColor); }
};

Bool DLRandomColor::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();
	data->SetVector(INPUT_COLOR1, Vector(HSVToRGB(Vector(0,0,0))));
	data->SetFloat(INPUT_IMPORTANCE1, 1.0);
	data->SetVector(INPUT_COLOR2, Vector(HSVToRGB(Vector(0,0,0))));
	data->SetFloat(INPUT_IMPORTANCE2, 1.0);
	data->SetVector(INPUT_COLOR3, Vector(HSVToRGB(Vector(0,0,0))));
	data->SetFloat(INPUT_IMPORTANCE3, 1.0);
	data->SetVector(INPUT_COLOR4, Vector(HSVToRGB(Vector(0,0,0))));
	data->SetFloat(INPUT_IMPORTANCE4, 1.0);
	data->SetVector(INPUT_COLOR5, Vector(HSVToRGB(Vector(0,0,0))));
	data->SetFloat(INPUT_IMPORTANCE5, 1.0);
	
	return true;
}


Vector DLRandomColor::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterRandomColorTexture(void)
{
	return RegisterShaderPlugin(DL_RANDOMCOLOR, "Random Color"_s, 0, DLRandomColor::Alloc, "dl_random_color"_s, 0);
}
