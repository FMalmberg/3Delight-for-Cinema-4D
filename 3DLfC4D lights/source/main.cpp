// Empty template for C4d plugins, R17

#include "c4d.h"
#include <string.h>

#include "DL_API.h"
#include "LightCardTranslator.h"
#include "EnvironmentLightTranslator.h"
#include "PointLightTranslator.h"
#include "IDs.h"
#include "c4dLightTranslator.h"
#include "../../3DLfC4D shaders/source/DLSky_Translator.h"
#include "../../3DLfC4D shaders/source/NSIExportShader.h"
#include "../../3DLfC4D shaders/res/description/dl_sky.h"

//Forward declaration
//bool RegisterLightCard(void);

Bool PluginStart(void)
{
	//if (!RegisterLightCard()) { return false; }
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
		pm->RegisterTranslator(Olight, AllocateTranslator<c4dLightTranslator>);
		//pm->RegisterTranslator(ID_LIGHTCARD, AllocateTranslator<LightCardTranslator>);
		break;

	}
	return FALSE;
}
