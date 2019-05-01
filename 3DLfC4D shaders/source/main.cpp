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
#include "NSIExportShader.h";
#include "NSIExportMaterial.h";
#include "NSIExportBitmap.h";
#include "DlPrincipled_translator.h";
#include "DLGlass_Translator.h";
#include "DLSkin_Translator.h";
#include "DLHairAndFur_Translator.h";
#include "DLMetal_Translator.h";
#include "DLSubstance_Translator.h";
#include "DLCarPaint_Translator.h";
#include "DLSky_Translator.h";
#include "EnvironmentLightTranslator.h";
#include "oenvironment.h";


#include "IDs.h"
void MySearchMenuResource(BaseContainer* bc)
{
	if (!bc) return;
	BrowseContainer browse(bc);
	Int32 id = 0;
	GeData * dat = nullptr;

	while (browse.GetNext(&id, &dat))
	{
		if (id == MENURESOURCE_SUBMENU || id == MENURESOURCE_STRING)
		{
			MySearchMenuResource(dat->GetContainer());
		}
		else if (id == MENURESOURCE_COMMAND)
		{
			if (dat->GetString() == String("IDM_MNEU"))
			{
				BaseContainer sc;
				sc.InsData(MENURESOURCE_SUBTITLE, String("3Delight"));
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052718"));
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052717"));
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052719"));
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052720"));
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052721"));
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052722"));
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052724"));

				bc->InsDataAfter(MENURESOURCE_STRING, sc, dat);
			}
		}
	}
}


Bool RegisterDLPrincipled(void);
Bool RegisterDLGlass(void);
Bool RegisterDLSkin(void);
Bool RegisterDLHairAndFur(void);
Bool RegisterDLMetal(void);
Bool RegisterDLSubstance(void);
Bool RegisterDLCarPaint(void);

Bool Register_DlPrincipled_Object(void);
Bool Register_DlGlass_Object(void);
Bool Register_HairAndFur_Object(void);
Bool Register_Metal_Object(void);
Bool Register_Skin_Object(void);
Bool Register_Substance_Object(void);
Bool Register_CarPaint_Object(void);
Bool RegisterSkyTexture(void);
Bool RegisterEnvironmentLight(void);
//Bool RegisterTextureShader(void);

Bool PluginStart(void)
{
	if (!Register_DlPrincipled_Object()) return FALSE;
	if (!Register_DlGlass_Object()) return FALSE;
	if (!Register_HairAndFur_Object()) return FALSE;
	if (!Register_Metal_Object()) return FALSE;
	if (!Register_Skin_Object()) return FALSE;
	if (!Register_Substance_Object()) return FALSE;
	if (!Register_CarPaint_Object()) return FALSE;
	if (!RegisterEnvironmentLight()) return FALSE;


	if (!RegisterDLPrincipled()) return FALSE;
	if (!RegisterDLGlass()) return FALSE;
	if (!RegisterDLSkin()) return FALSE;
	if (!RegisterDLHairAndFur()) return FALSE;
	if (!RegisterDLMetal()) return FALSE;
	if (!RegisterDLSubstance()) return FALSE;
	if (!RegisterDLCarPaint()) return FALSE;
	if (!RegisterSkyTexture()) return FALSE;

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
	{
		DL_PluginManager* pm = (DL_PluginManager*)data;
		pm->RegisterHook(AllocateHook<ShaderSettingsHook>);
		pm->RegisterTranslator(ID_TEXTURESHADER, AllocateTranslator<TextureShaderTranslator>);
		pm->RegisterTranslator(Mmaterial, AllocateTranslator<NSI_Export_Material>);
		pm->RegisterTranslator(ID_ENVIRONMENTLIGHT, AllocateTranslator<EnvironmentLightTranslator>);

		//3Delight Materials
		pm->RegisterTranslator(DL_PRINCIPLED, AllocateTranslator<Delight_Principled>);
		pm->RegisterTranslator(DL_GLASS, AllocateTranslator<Delight_Glass>);
		pm->RegisterTranslator(DL_SKIN, AllocateTranslator<Delight_Skin>);
		pm->RegisterTranslator(DL_HAIRANDFUR, AllocateTranslator<Delight_HairAndFur>);
		pm->RegisterTranslator(DL_METAL, AllocateTranslator<Delight_Metal>);
		pm->RegisterTranslator(DL_SUBSTANCE, AllocateTranslator<Delight_Substance>);
		pm->RegisterTranslator(DL_CARPAINT, AllocateTranslator<Delight_CarPaint>);

		pm->RegisterTranslator(DL_SKY, AllocateTranslator<Delight_Sky>);
		pm->RegisterTranslator(Osky, AllocateTranslator<EnvironmentLightTranslator>);

		pm->RegisterTranslator(Xcheckerboard, AllocateTranslator<NSI_Export_Shader>);
		/*pm->RegisterTranslator(Xtiles, AllocateTranslator<NSI_Export_Shader>);
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
		pm->RegisterTranslator(Xthinfilm, AllocateTranslator<NSI_Export_Shader>);*/

		//pm->RegisterTranslator(ID_TEXTURESHADER, AllocateTranslator<TextureShaderTranslator>);
		pm->RegisterTranslator(Xbitmap, AllocateTranslator<NSI_Export_Bitmap>);
		pm->RegisterTranslator(Ttexture, AllocateTranslator<TextureTagTranslator>);
		break;

	}

	case C4DPL_BUILDMENU:
	{
		BaseContainer* bc = GetMenuResource("M_MATERIAL_MANAGER"_s);
		if (!bc) return FALSE;
		MySearchMenuResource(bc);
		break;
	}
	}
	return FALSE;
}
