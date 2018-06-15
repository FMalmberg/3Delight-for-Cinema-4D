// Empty template for C4d plugins, R17

#include "c4d.h"
#include <string.h>

#include "DL_API.h"
#include "LightCardTranslator.h"
#include "EnvironmentLightTranslator.h"
#include "PointLightTranslator.h"
#include "IDs.h"

Bool RegisterLightCard(void);
Bool RegisterEnvironmentLight(void);
Bool RegisterPointLight(void);

Bool PluginStart(void)
{
	//...do or register something...
	if (!RegisterLightCard()) return FALSE;
	if (!RegisterPointLight()) return FALSE;
	if (!RegisterEnvironmentLight()) return FALSE;

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
		pm->RegisterTranslator(ID_LIGHTCARD, AllocateTranslator<LightCardTranslator>);
		pm->RegisterTranslator(ID_POINTLIGHT, AllocateTranslator<PointLightTranslator>);
		pm->RegisterTranslator(ID_ENVIRONMENTLIGHT, AllocateTranslator<EnvironmentLightTranslator>);
		//pm->RegisterTranslator(Opolygon, AllocateTranslator<PolygonObjectTranslator>);
		break;

	}
	return FALSE;
}
