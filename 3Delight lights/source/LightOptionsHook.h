#ifndef LIGHT_OPTIODL_HOOK_H
#define LIGHT_OPTIODL_HOOK_H

#include "DL_HookPlugin.h"
#include "c4d.h"

class LightOptionsHook:public DL_HookPlugin{
public:

	virtual void Emit(BaseDocument* doc, DL_SceneParser* parser);
	
	
};

#endif