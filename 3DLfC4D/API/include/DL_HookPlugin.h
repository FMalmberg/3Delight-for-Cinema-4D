#ifndef DL_HOOK_PLUGIN_H
#define DL_HOOK_PLUGIN_H

#include "DL_Hook.h"


class DL_HookPlugin:public DL_Hook{
public:
	virtual ~DL_HookPlugin(){}
	virtual void Free(){ delete this; };
	virtual void CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser){}
	virtual void ConnectNSINodes(BaseDocument* doc, DL_SceneParser* parser){}
	virtual void SampleMotion(double t, long i, BaseDocument* doc, DL_SceneParser* parser){}
};

template<typename T>
DL_Hook* AllocateHook()
{
    return new T();
}

#endif