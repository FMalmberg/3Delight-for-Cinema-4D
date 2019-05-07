#include "c4d.h"
#include <string.h>
#include "DL_API.h"
#include "PolygonObjectTranslator.h"
#include "BaseObjectTranslator.h"

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
		if (!g_resource.Init()) return FALSE; // don't start plugin without resource
		return TRUE;
		break;

	case DL_LOAD_PLUGINS:
		DL_PluginManager* pm = (DL_PluginManager*)data;

		pm->RegisterTranslator(Opolygon, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Oalembicgenerator, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Ocube, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Ocylinder, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Osphere, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Odisc, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Oextrude, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Oplatonic, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Ocone, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Otorus, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Otube, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Ofigure, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Opyramid, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Oplane, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Ofractal, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Ocapsule, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Ooiltank, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Orelief, AllocateTranslator<PolygonObjectTranslator>);
		pm->RegisterTranslator(Osinglepoly, AllocateTranslator<PolygonObjectTranslator>);

		break;

	}
	return FALSE;
}
