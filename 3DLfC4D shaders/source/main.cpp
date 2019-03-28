// Empty template for C4d plugins, R17

#include "c4d.h"
#include <string.h>

#include "DL_API.h"
#include "ShaderSettingsHook.h"
#include "TextureTagTranslator.h"
#include "TextureTranslator.h"
#include "DL_material_translator.h"
#include "StandardShaderTranslator.h"
#include "TextureShaderTranslator.h"
#include "GlassShaderTranslator.h"
#include "NormalDisplacementTranslator.h"
#include "RangeTranslator.h"
#include "../../3DLfC4D shaders/project/NSIExportShader.h";
#include "../../3DLfC4D shaders/project/NSIExportMaterial.h";


#include "IDs.h"


Bool RegisterDLMaterial(void);
Bool RegisterStandardShader(void);
Bool RegisterGlassShader(void);
Bool RegisterNormalDisplacementShader(void);
Bool RegisterRangeShader(void);
//Bool RegisterTextureShader(void);

Bool PluginStart(void)
{
	if (!RegisterDLMaterial()) return FALSE;
	if (!RegisterStandardShader()) return FALSE;
	if(!RegisterGlassShader()) return FALSE;
	if (!RegisterNormalDisplacementShader()) return FALSE;
	if (!RegisterRangeShader()) return FALSE;
	//if (!RegisterTextureShader()) return FALSE;
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
		if (!g_resource.Init()) return FALSE; // don't start plugin without resource
		return TRUE;
		break;

	case DL_LOAD_PLUGINS:
		DL_PluginManager* pm = (DL_PluginManager*)data;
		pm->RegisterHook(AllocateHook<ShaderSettingsHook>);
		pm->RegisterTranslator(ID_STANDARDSHADER, AllocateTranslator<StandardShaderTranslator>);
		pm->RegisterTranslator(ID_TEXTURESHADER, AllocateTranslator<TextureShaderTranslator>);
		pm->RegisterTranslator(ID_GLASSSHADER, AllocateTranslator<GlassShaderTranslator>);
		pm->RegisterTranslator(ID_NORMALDISPLACEMENTSHADER, AllocateTranslator<NormalDisplacementTranslator>);
		pm->RegisterTranslator(ID_RANGESHADER, AllocateTranslator<RangeTranslator>);
		pm->RegisterTranslator(Mmaterial, AllocateTranslator<NSI_Export_Material>);
		pm->RegisterTranslator(Xcheckerboard, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xtiles, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xstar, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xbrick, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xcloud, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xcolorstripes, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xcyclone, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xearth, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xfire, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xflame, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xgalaxy, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xmetal, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xsimplenoise, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xrust, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xstarfield, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xsunburst, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xsimpleturbulence, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xvenus, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xwater, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xwood, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xplanet, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xmarble, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xspectral, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xgradient, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xfresnel, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xlumas, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xproximal, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xnormaldirection, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xtranslucency, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xfusion, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xposterizer, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xcolorizer, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xdistorter, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xprojector, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xnoise, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xlayer, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xspline, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xfilter, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xripple, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xvertexmap, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xsss, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xambientocclusion, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xchanlum, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xmosaic, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xrainsampler, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xnormalizer, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xterrainmask, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xobjectcolor, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xformula, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xvariation, AllocateTranslator<NSI_Export_Shader>);
		pm->RegisterTranslator(Xthinfilm, AllocateTranslator<NSI_Export_Shader>);

		//pm->RegisterTranslator(ID_TEXTURESHADER, AllocateTranslator<TextureShaderTranslator>);
		pm->RegisterTranslator(Xbitmap, AllocateTranslator<TextureTranslator>);
		pm->RegisterTranslator(Ttexture, AllocateTranslator<TextureTagTranslator>);
		pm->RegisterTranslator(ID_DELIGHTMATERIAL, AllocateTranslator<DL_material_translator>);


		break;

	}
	return FALSE;
}
