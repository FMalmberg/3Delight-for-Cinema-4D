
#include "c4d.h"
#include "c4d_symbols.h"
#include "xstandardshader.h"
#include "IDs.h"

class TextureShader : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(TextureShader); }
};

Bool TextureShader::Init(GeListNode* node)
{
	//BaseContainer* data = ((BaseShader*)node)->GetDataInstance();

	return true;
}


Vector TextureShader::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterTextureShader(void)
{
	return RegisterShaderPlugin(ID_TEXTURESHADER, "DL_Texture"_s, 0, TextureShader::Alloc, "Xtextureshader"_s, 0);
}
