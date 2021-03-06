#ifndef DL_HOOK_PLUGIN_H
#define DL_HOOK_PLUGIN_H

#include "DL_Hook.h"

class DL_HookPlugin:public DL_Hook{
public:
	virtual ~DL_HookPlugin(){}
	virtual void Free(){ delete this; };

	virtual void Init(BaseDocument* doc, DL_SceneParser* parser){}
	virtual void CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser){}
	virtual void ConnectNSINodes(BaseDocument* doc, DL_SceneParser* parser){}
	virtual void SampleAttributes(DL_SampleInfo* info, BaseDocument* doc, DL_SceneParser* parser){}
};

template<class T>

DL_Hook* AllocateHook()
{
    return new T();
}

#endif