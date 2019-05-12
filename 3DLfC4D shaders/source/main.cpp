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
#include "DLWorleyNoise_Translator.h";
#include "DLFlakes_translator.h";
#include "EnvironmentLightTranslator.h";
#include "oenvironment.h";
#include "DLColorBlend_Translator.h"
#include "DLColorCorrection_Translator.h"



#include "IDs.h"
/*
	This function is used to dynamically change the structure of Create Material UI.
	We create a SubTitle with the name 3Delight when create material is pressed.
	Then under this Subtitle we put all our command plugins as below.
*/
void CreateMaterialUI(BaseContainer* bc)
{
	if (!bc) return;
	BrowseContainer browse(bc);
	Int32 id = 0;
	GeData * dat = nullptr;

	while (browse.GetNext(&id, &dat))
	{
		if (id == MENURESOURCE_SUBMENU || id == MENURESOURCE_STRING)
		{
			CreateMaterialUI(dat->GetContainer());
		}
		else if (id == MENURESOURCE_COMMAND)
		{
			if (dat->GetString() == String("IDM_MNEU"))
			{
				BaseContainer sc;
				sc.InsData(MENURESOURCE_SUBTITLE, String("3Delight"));
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052718")); //DL_Principled
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052717")); //DL_Glass
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052719")); //DL_HAIRANDFUR
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052720")); //DL_METAL
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052721")); //DL_SKIN
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052722")); //DL_SUBSTANCE
				sc.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052724")); //DL_CARPAINT
				bc->InsDataAfter(MENURESOURCE_STRING, sc, dat);
			}
		}
	}
}

//3Delight Materials Object
Bool Register_DlPrincipled_Object(void);
Bool Register_DlGlass_Object(void);
Bool Register_HairAndFur_Object(void);
Bool Register_Metal_Object(void);
Bool Register_Skin_Object(void);
Bool Register_Substance_Object(void);
Bool Register_CarPaint_Object(void);

//3Delight Materials
Bool RegisterDLPrincipled(void);
Bool RegisterDLGlass(void);
Bool RegisterDLSkin(void);
Bool RegisterDLHairAndFur(void);
Bool RegisterDLMetal(void);
Bool RegisterDLSubstance(void);
Bool RegisterDLCarPaint(void);

//3Delight Shaders
Bool RegisterSkyTexture(void);
Bool RegisterWorleyNoiseTexture(void);
Bool RegisterFlakesTexture(void);
Bool RegisterEnvironmentLight(void);
Bool RegisterColorBlendTexture(void);
Bool RegisterColorCorrectionTexture(void);
Bool RegisterColorVariationTexture(void);
Bool RegisterFacingRatioTexture(void);

Bool PluginStart(void)
{
	//3Delight Materials Object
	if (!Register_DlPrincipled_Object()) return FALSE;
	if (!Register_DlGlass_Object()) return FALSE;
	if (!Register_HairAndFur_Object()) return FALSE;
	if (!Register_Metal_Object()) return FALSE;
	if (!Register_Skin_Object()) return FALSE;
	if (!Register_Substance_Object()) return FALSE;
	if (!Register_CarPaint_Object()) return FALSE;

	//3Delight Materials
	if (!RegisterDLPrincipled()) return FALSE;
	if (!RegisterDLGlass()) return FALSE;
	if (!RegisterDLSkin()) return FALSE;
	if (!RegisterDLHairAndFur()) return FALSE;
	if (!RegisterDLMetal()) return FALSE;
	if (!RegisterDLSubstance()) return FALSE;
	if (!RegisterDLCarPaint()) return FALSE;

	//3Delight Shaders
	if (!RegisterEnvironmentLight()) return FALSE;
	if (!RegisterSkyTexture()) return FALSE;
	if (!RegisterFlakesTexture()) return FALSE;
	if (!RegisterWorleyNoiseTexture()) return FALSE;
	if (!RegisterColorBlendTexture()) return FALSE;
	if (!RegisterColorCorrectionTexture()) return FALSE;
	if (!RegisterColorVariationTexture()) return FALSE;
	if (!RegisterFacingRatioTexture()) return FALSE;

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
			/*
				On the translator we have only allocated the Materials and Shaders that we
				support in our plugin. There will be others to add as we are working on it.
			*/

			//C4D Material
			pm->RegisterTranslator(Mmaterial, AllocateTranslator<NSI_Export_Material>);

			//3Delight Materials
			pm->RegisterTranslator(DL_PRINCIPLED, AllocateTranslator<Delight_Principled>);
			pm->RegisterTranslator(DL_GLASS, AllocateTranslator<Delight_Glass>);
			pm->RegisterTranslator(DL_SKIN, AllocateTranslator<Delight_Skin>);
			pm->RegisterTranslator(DL_HAIRANDFUR, AllocateTranslator<Delight_HairAndFur>);
			pm->RegisterTranslator(DL_METAL, AllocateTranslator<Delight_Metal>);
			pm->RegisterTranslator(DL_SUBSTANCE, AllocateTranslator<Delight_Substance>);
			pm->RegisterTranslator(DL_CARPAINT, AllocateTranslator<Delight_CarPaint>);

			//C4D Shaders
			pm->RegisterTranslator(Osky, AllocateTranslator<EnvironmentLightTranslator>);
			pm->RegisterTranslator(Xcheckerboard, AllocateTranslator<NSI_Export_Shader>);
			pm->RegisterTranslator(Xbitmap, AllocateTranslator<NSI_Export_Bitmap>);
			pm->RegisterTranslator(Ttexture, AllocateTranslator<TextureTagTranslator>);

			//3Delight Shaders
			pm->RegisterTranslator(DL_SKY, AllocateTranslator<Delight_Sky>);
			pm->RegisterTranslator(DL_WorleyNoise, AllocateTranslator<Delight_WorleyNoise>);
			pm->RegisterTranslator(DL_Flakes, AllocateTranslator<Delight_Flakes>);
			pm->RegisterTranslator(ID_ENVIRONMENTLIGHT, AllocateTranslator<EnvironmentLightTranslator>);
			pm->RegisterTranslator(ID_TEXTURESHADER, AllocateTranslator<TextureShaderTranslator>);
			
			pm->RegisterTranslator(DL_COLORBLEND, AllocateTranslator<Delight_ColorBlend>);
			pm->RegisterTranslator(DL_COLORCORRECTION, AllocateTranslator<Delight_ColorCorrection>);
			pm->RegisterTranslator(DL_COLORVARIATION, AllocateTranslator<Delight_ColorVariation>);
			pm->RegisterTranslator(DL_FACINGRATIO, AllocateTranslator<Delight_FacingRatio>);
			break;

		}

		case C4DPL_BUILDMENU:
		{
			BaseContainer* bc = GetMenuResource("M_MATERIAL_MANAGER"_s);
			if (!bc) return FALSE;
			CreateMaterialUI(bc);
			break;
		}
	}
		return FALSE;
}
