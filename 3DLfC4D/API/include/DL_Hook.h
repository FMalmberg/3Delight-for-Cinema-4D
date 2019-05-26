#ifndef DL_HOOK_H
#define DL_HOOK_H

//Forward declaration
class BaseDocument;

#include "DL_SceneParser.h"
#include "DL_SampleInfo.h"

class DL_Hook{
public:
	virtual void Free() = 0;

	virtual void Init(BaseDocument* doc, DL_SceneParser* parser) = 0;
	virtual void CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser) = 0;
	virtual void ConnectNSINodes(BaseDocument* doc, DL_SceneParser* parser) = 0;
	virtual void SampleAttributes(DL_SampleInfo* info, BaseDocument* doc, DL_SceneParser* parser) = 0;
};

typedef DL_Hook* (*HookAllocator)();

#endif