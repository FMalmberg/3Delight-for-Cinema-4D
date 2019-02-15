// example for a complex channel shader with custom areas
// and animated preview
#include "c4d.h"
#include "c4d_symbols.h"
#include "xrange.h"
#include "IDs.h"


class RangeShader : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(RangeShader); }
};

Bool RangeShader::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();

	data->SetFloat(IN_MIN, 0);
	data->SetFloat(IN_MAX, 1);
	data->SetFloat(OUT_MIN, 0);
	data->SetFloat(OUT_MAX, 1);

	return true;
}


Vector RangeShader::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterRangeShader(void)
{
	return RegisterShaderPlugin(ID_RANGESHADER, "DL_RangeShader"_s, 0, RangeShader::Alloc, "Xrange"_s, 0);
}
