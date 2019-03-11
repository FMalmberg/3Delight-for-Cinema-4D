// example for a complex channel shader with custom areas
// and animated preview
#include "c4d.h"
#include "c4d_symbols.h"
#include "xnormaldisplacementshader.h"
#include "IDs.h"


class NormalDisplacementShader : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(NormalDisplacementShader); }
};

Bool NormalDisplacementShader::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();

	data->SetFloat(DISPLACEMENT_HEIGHT, 0.01);
	data->SetFloat(DISPLACEMENT_STRENGTH, 1.0);
	//data->SetBool(DISPLACEMENT_CENTERED, true);

	return true;
}


Vector NormalDisplacementShader::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterNormalDisplacementShader(void)
{
	return RegisterShaderPlugin(ID_NORMALDISPLACEMENTSHADER, "DL_NormalDisplacement"_s, 0, NormalDisplacementShader::Alloc, "Xnormaldisplacementshader"_s, 0);
}
