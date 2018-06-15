#include "c4d.h"
#include <string.h>
#include "DL_API.h"
#include "PolygonObjectTranslator.h"


Bool PluginStart(void)
{

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

		pm->RegisterTranslator(Opolygon, AllocateTranslator<PolygonObjectTranslator>);
		//pm->RegisterObjectTranslator(Osphere,AllocateObjectTranslator<SphereTranslator>);
		//pm->RegisterObjectTranslator(Osds,AllocateObjectTranslator<SDSTranslator>);
		//pm->RegisterObjectTranslator(Opolygon,AllocateObjectTranslator<PolygonObjectTranslator>);
		break;

	}
	return FALSE;
}
/*
Bool PluginMessage(Int32 id, void* data)
{
	switch (id)
	{
		case C4DPL_INIT_SYS:
			if (!resource.Init())
				return false;		// don't start plugin without resource

			return true;

		case DL_LOAD_PLUGINS:
			DL_PluginManager* pm = (DL_PluginManager*)data;

			pm->RegisterTranslator(Opolygon, AllocateTranslator<PolygonObjectTranslator>);
			break;

		case C4DMSG_PRIORITY:
			return true;

		case C4DPL_BUILDMENU:
			break;

		case C4DPL_COMMANDLINEARGS:
			break;

		case C4DPL_EDITIMAGE:
			return false;
	}

	return false;
}*/
