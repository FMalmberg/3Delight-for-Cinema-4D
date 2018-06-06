#ifndef RENDER_OPTIODL_HOOK_H
#define RENDER_OPTIODL_HOOK_H

#include "DL_HookPlugin.h"
#include "c4d.h"
#include <vector>

class RenderOptionsHook:public DL_HookPlugin{
public:
	virtual void CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser);
	virtual void ConnectNSINodes(BaseDocument* doc, DL_SceneParser* parser);

private:
	std::string layer_handle;
	std::string driver_handle;
	
	
};

#endif