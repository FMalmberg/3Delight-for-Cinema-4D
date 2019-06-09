// Empty template for C4d plugins, R17

#include "c4d.h"
#include <string.h>
#include "c4d_gui.h"
#include "PluginManager.h"
#include "BitmapDisplayDriver.h"
#include "CameraHook.h"
#include "RenderOptionsHook.h"
#include "DisplayTranslator.h"
#include "VisibilityTagTranslator.h"
#include "IDs.h"
#include <windows.h>


//Global plugin manager, 
//Stores pointers to allocators for translator plugins	

PluginManager PM;

Bool RegisterRenderFrame(void);
Bool RegisterRenderSequence(void);
//Bool RegisterInteractiveRenderManager(void);
Bool RegisterDisplay(void);
//Bool RegisterImageLayer(void);
Bool RegisterDL_CameraTag(void);
Bool RegisterDL_MotionBlurTag(void);
Bool RegisterDL_VisibilityTag(void);
Bool Register3DelightPlugin(void);
Bool RegisterCustomListView(void);
Bool RegisterCustomMultiLight(void);
Bool RegisterInteractiveRenderingStart(void);
Bool RegisterInteractiveRenderingStop(void);
Bool RegisterInteractiveRenderManager(void);
Bool Register3DelightCommand(void);

Bool PluginStart(void)
{
	const char *env = getenv("DELIGHT");
	if (env != nullptr)
	{
		std::string _3delight_path(env);
		_3delight_path += "/bin/3Delight.DLL";
		LoadLibraryA(_3delight_path.c_str());
	}
	else
	{
		MessageDialog("Could not load 3Delight Library"_s);
	}
	//if (!RegisterCustomGUITest()) return FALSE;
	if (!RegisterRenderFrame()) return FALSE;
	if (!RegisterRenderSequence()) return FALSE;
	if (!RegisterCustomListView()) return FALSE;
	if (!RegisterCustomMultiLight()) return FALSE;
	if (!RegisterInteractiveRenderingStart()) return FALSE;
	if (!RegisterInteractiveRenderingStop()) return FALSE;
	if (!Register3DelightPlugin()) return FALSE;
	if (!RegisterInteractiveRenderManager()) return FALSE;
	if (!Register3DelightCommand()) return FALSE;

	
	if (!RegisterDisplay()) return FALSE;
	//if (!RegisterDL_CameraTag()) return FALSE;
	//if (!RegisterDL_MotionBlurTag()) return FALSE;
	if (!RegisterDL_VisibilityTag()) return FALSE;
	if (!RegisterCustomListView()) return FALSE;
	if (!RegisterCustomMultiLight()) return FALSE;

	//Register 3Delight display driver for rendering to C4D bitmaps
	PtDspyDriverFunctionTable table;
	memset(&table, 0, sizeof(table));
	table.Version = 1;
	table.pOpen = &BmpDspyImageOpen;
	table.pQuery = &BmpDspyImageQuery;
	table.pWrite = &BmpDspyImageData;
	table.pClose = &BmpDspyImageClose;
	
	PtDspyError err = DspyRegisterDriverTable("C4D_bitmap", &table); 
	
	return true;
}

void PluginEnd(void)
{

}

Bool PluginMessage(Int32 id, void* data)
{
	switch (id)
	{
	case C4DPL_INIT_SYS:
		if (!g_resource.Init())
			return false;		// don't start plugin without resource
		return true;

	case C4DPL_STARTACTIVITY:
	{
		//Send message to load translators from other plugins
		GePluginMessage(DL_LOAD_PLUGINS, (void*)&PM);
		break;
	}
	case DL_LOAD_PLUGINS:
		DL_PluginManager* pm = (DL_PluginManager*)data;
		pm->RegisterHook(AllocateHook<CameraHook>);
		pm->RegisterHook(AllocateHook<RenderOptionsHook>);
		pm->RegisterTranslator(ID_DISPLAY, AllocateTranslator<DisplayTranslator>);
		pm->RegisterTranslator(ID_DL_VISIBILITYTAG, AllocateTranslator<VisibilityTagTranslator>);
		//pm->RegisterTranslator(,AllocateTranslator<QualityValuesParser>);
		break;

	}

	return false;
}
