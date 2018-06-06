#ifndef SHADER_SETTINGS_HOOK_H
#define SHADER_SETTINGS_HOOK_H

#include "DL_HookPlugin.h"

class ShaderSettingsHook :public DL_HookPlugin{
public:
	virtual void CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser);
	virtual void ConnectNSINodes(BaseDocument* doc, DL_SceneParser* parser);
	//virtual void Init(BaseDocument* doc, DL_SceneParser* parser);
	//virtual void Emit(BaseDocument* doc, DL_SceneParser* parser);

private:
	//std::string layer_handle;
	//std::string driver_handle;
};

#endif