// Empty template for C4d plugins, R17

#include "c4d.h"
#include <string.h>

#include "DL_API.h"
#include "LightCardTranslator.h"
#include "EnvironmentLightTranslator.h"
#include "PointLightTranslator.h"
#include "IDs.h"
#include "c4dLightTranslator.h"
#include "DirectionalLightTranslator.h"
#include "IncandescenceLightTranslator.h"
#include "../../3DLfC4D shaders/source/DLSky_Translator.h"
#include "../../3DLfC4D shaders/source/NSIExportShader.h"
#include "../../3DLfC4D shaders/res/description/dl_sky.h"

//Forward declaration
bool RegisterLightCard(void);
bool RegisterDirectionalLight(void);
bool RegisterPointLight(void);
bool RegisterIncandescenceLight(void);

Bool PluginStart(void)
{
	if (!RegisterLightCard()) { return false; }
	if (!RegisterDirectionalLight()) { return false; }
	//if (!RegisterPointLight()) { return false; }
	if (!RegisterIncandescenceLight()) { return false; }
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
		pm->RegisterTranslator(Olight, AllocateTranslator<c4dLightTranslator>,true);
		pm->RegisterTranslator(ID_LIGHTCARD, AllocateTranslator<LightCardTranslator>,true);
		//pm->RegisterTranslator(ID_POINTLIGHT, AllocateTranslator<PointLightTranslator>, true);
		pm->RegisterTranslator(ID_DIRECTIONAL_LIGHT, AllocateTranslator<DirectionalLightTranslator>, true);
		pm->RegisterTranslator(ID_INCANDESCENCELIGHT, AllocateTranslator<IncandescenceLightTranslator>, true);
		break;
		
	}
	return FALSE;
}
