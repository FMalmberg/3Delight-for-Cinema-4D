// Empty template for C4d plugins, R17

#include "c4d.h"
#include <string.h>

#include "DL_API.h"

#include "ShaderSettingsHook.h"
#include "TextureTranslator.h"
#include "DL_material_translator.h"
#include "StandardShaderTranslator.h"
#include "TextureShaderTranslator.h"
#include "GlassShaderTranslator.h"

#include "IDs.h"


Bool RegisterDLMaterial(void);
Bool RegisterStandardShader(void);
Bool RegisterGlassShader(void);

Bool PluginStart(void)
{
	if (!RegisterDLMaterial()) return FALSE;
	if (!RegisterStandardShader()) return FALSE;
	if(!RegisterGlassShader()) return FALSE;
	return true;
}

void PluginEnd(void)
{

}

Bool PluginMessage(Int32 id, void *data)
{
	switch (id)
	{
	case C4DPL_INIT_SYS:
		if (!resource.Init()) return FALSE; // don't start plugin without resource
		return TRUE;
		break;

	case DL_LOAD_PLUGINS:
		DL_PluginManager* pm = (DL_PluginManager*)data;
		pm->RegisterHook(AllocateHook<ShaderSettingsHook>);
		pm->RegisterTranslator(ID_DELIGHTMATERIAL, AllocateTranslator<DL_material_translator>);
		pm->RegisterTranslator(ID_STANDARDSHADER, AllocateTranslator<StandardShaderTranslator>);
		pm->RegisterTranslator(ID_TEXTURESHADER, AllocateTranslator<TextureShaderTranslator>);
		pm->RegisterTranslator(ID_GLASSSHADER, AllocateTranslator<GlassShaderTranslator>);
		pm->RegisterTranslator(Xbitmap, AllocateTranslator<TextureTranslator>);
		
		break;

	}
	return FALSE;
}
