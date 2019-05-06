// example for a complex channel shader with custom areas
// and animated preview
#include "c4d.h"
#include "c4d_symbols.h"
#include "xglassshader.h"
#include "IDs.h"

class GlassShader : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(GlassShader); }
};

Bool GlassShader::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();

	data->SetVector(REFLECTION_COLOR, Vector(1, 1, 1));
	data->SetVector(REFRACTION_COLOR, Vector(1, 1, 1));
	data->SetVector(TINT, Vector(1, 1, 1));
	data->SetFloat(ROUGHNESS, 0.05);
	data->SetFloat(IOR, 1.5);
	data->SetInt32(MODE, MODE_REFRACTION);

	return true;
}


Vector GlassShader::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterGlassShader(void)
{
	return RegisterShaderPlugin(ID_GLASSSHADER, "DL_GlassSurface"_s, 0, GlassShader::Alloc, "Xglassshader"_s, 0);
}
