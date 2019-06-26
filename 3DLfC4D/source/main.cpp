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
#include "nsi_dynamic.hpp"


void Create3DelightMenu(BaseContainer* bc)
{
	if (!bc) return;
	BrowseContainer browse(bc);
	Int32 id = 0;
	GeData * dat = nullptr;

	while (browse.GetNext(&id, &dat))
	{
		if (id == MENURESOURCE_SUBMENU)
		{
			BaseContainer* container = dat->GetContainer();
			String subtitle = container->GetString(MENURESOURCE_SUBTITLE);
			if (container && subtitle == "IDS_EDITOR_PLUGINS")
			{
				BaseContainer sc;
				sc.InsData(MENURESOURCE_SUBTITLE, String("3Delight"));
						
				BaseContainer MaterialsSubMenu;
				MaterialsSubMenu.InsData(MENURESOURCE_SUBTITLE, String("Materials"));
				MaterialsSubMenu.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052718")); //DL_Principled
				MaterialsSubMenu.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052717")); //DL_Glass
				MaterialsSubMenu.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052719")); //DL_HAIRANDFUR
				MaterialsSubMenu.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052720")); //DL_METAL
				MaterialsSubMenu.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052721")); //DL_SKIN
				MaterialsSubMenu.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052722")); //DL_SUBSTANCE
				MaterialsSubMenu.InsData(MENURESOURCE_COMMAND, String("PLUGIN_CMD_1052724")); //DL_CARPAINT


				BaseContainer ObjectsSubMenu;
				ObjectsSubMenu.InsData(MENURESOURCE_SUBTITLE, String("Objects"));
				ObjectsSubMenu.InsData(MENURESOURCE_COMMAND, "PLUGIN_CMD_" + String::IntToString(ID_ENVIRONMENTLIGHT));
				ObjectsSubMenu.InsData(MENURESOURCE_COMMAND, "PLUGIN_CMD_" + String::IntToString(DL_OPENVDB));


				BaseContainer LightsSubMenu;
				LightsSubMenu.InsData(MENURESOURCE_SUBTITLE, String("Lights"));
				LightsSubMenu.InsData(MENURESOURCE_COMMAND, "PLUGIN_CMD_" + String::IntToString(ID_LIGHTCARD));
				LightsSubMenu.InsData(MENURESOURCE_COMMAND, "PLUGIN_CMD_" + String::IntToString(ID_DIRECTIONAL_LIGHT));
				LightsSubMenu.InsData(MENURESOURCE_COMMAND, "PLUGIN_CMD_" + String::IntToString(ID_POINTLIGHT));
				LightsSubMenu.InsData(MENURESOURCE_COMMAND, "PLUGIN_CMD_" + String::IntToString(ID_INCANDESCENCELIGHT));

				BaseContainer RenderSubMenu;
				RenderSubMenu.InsData(MENURESOURCE_SUBTITLE, String("Render"));
				RenderSubMenu.InsData(MENURESOURCE_COMMAND, "PLUGIN_CMD_" + String::IntToString(ID_RENDER_COMMAND));
				RenderSubMenu.InsData(MENURESOURCE_COMMAND, "PLUGIN_CMD_" + String::IntToString(ID_INTERACTIVE_RENDERING_START));
				RenderSubMenu.InsData(MENURESOURCE_COMMAND, "PLUGIN_CMD_" + String::IntToString(ID_INTERACTIVE_RENDERING_STOP));
				
				sc.InsData(MENURESOURCE_SUBMENU, MaterialsSubMenu);
				sc.InsData(MENURESOURCE_SUBMENU, ObjectsSubMenu);
				sc.InsData(MENURESOURCE_SUBMENU, LightsSubMenu);
				sc.InsData(MENURESOURCE_SUBMENU, RenderSubMenu);

				bc->InsDataAfter(MENURESOURCE_STRING, sc, dat);
			}
			//ApplicationOutput(subtitle);
		}
	}
}

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
	std::string _3delight_path(env);
	_3delight_path += "/bin/3Delight.DLL";
		
	NSI::DynamicAPI nsi_api = NSI::DynamicAPI(_3delight_path.c_str());
	NSIContext_t     nsi_ctx;
	bool all_good = false;
	nsi_ctx = nsi_api.NSIBegin(0, nullptr);
	if (nsi_ctx != NSI_BAD_CONTEXT)
	{
		nsi_api.NSIEnd(nsi_ctx);
		all_good = true;
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
	{
		DL_PluginManager* pm = (DL_PluginManager*)data;
		pm->RegisterHook(AllocateHook<CameraHook>);
		pm->RegisterHook(AllocateHook<RenderOptionsHook>);
		pm->RegisterTranslator(ID_DISPLAY, AllocateTranslator<DisplayTranslator>);
		pm->RegisterTranslator(ID_DL_VISIBILITYTAG, AllocateTranslator<VisibilityTagTranslator>);
		//pm->RegisterTranslator(,AllocateTranslator<QualityValuesParser>);
		break;
	}
	
	case C4DPL_BUILDMENU:
	{
		BaseContainer* bc = GetMenuResource("M_EDITOR"_s);
		if (!bc) return FALSE;
		Create3DelightMenu(bc);
		break;
	}

	}

	
	return false;
}
