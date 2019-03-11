#pragma once

#include "DL_TranslatorPlugin.h"
#include "DL_HookPlugin.h"

/**
	Each defined ID correspond with an icon which is later used for the aov layers.
	The ids are registered with a corresponding icon on Register3DelightPlugin() funcion in 3DlghtRenderer.cpp
	You can just define parameters on res file and for every parameter on example.res
	file there should be a corresponding example.h file which will carry the id of each parameter.
*/
#define ID_HELPPLUGIN 1050930
#define ID_SDK_EXAMPLE_CUSTOMGUI_STRING 1034666
#define DL_DISPLAY_ON	1051140
#define DL_DISPLAY_OFF	1051141
#define DL_FOLDER_ON	1051142
#define DL_FOLDER_OFF	1051143
#define DL_JPG_ON	1051144
#define DL_JPG_OFF	1051145

/**
	The Render Settings class must inherit from VideoPostData as this is the class for 
	creating video post plugins. This plugin type can be used to add render engines to Cinema 4D.
	Also VideoPostData plugins are based on NodeData so the settings are automatically displayed 
	in the Render Settings window.
*/
class RenderSettings : public VideoPostData
{
public:
	/**
	We override Alloc function to create a new instance of the plugin. It is used instead of a constructor 
	at this one provide means to return an error if something goes wrong. The initial values of this 
	instance are defined in the Init function declared below. 
	*/
	static NodeData *Alloc(void) { 
		return NewObj(RenderSettings) iferr_ignore("Wrong Instance");
	}

	virtual Bool Init(GeListNode *i_node);
	virtual Bool GetDEnabling(GeListNode* i_node, const DescID& i_id, const GeData& i_data, DESCFLAGS_ENABLE i_flags, const BaseContainer* i_itemdesc);
	virtual Bool GetDDescription(GeListNode *i_node, Description *i_description, DESCFLAGS_DESC &i_flags);
	virtual Bool Message(GeListNode* i_node, Int32 i_type, void* i_data);
};
