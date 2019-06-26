#ifndef CAMERA_HOOK_H
#define CAMERA_HOOK_H
#pragma warning(disable : 4265)
#include "DL_HookPlugin.h"
#include "c4d.h"
#include <vector>

class CameraHook:public DL_HookPlugin{
public:
	void Init(BaseDocument* doc, DL_SceneParser* parser);
	void CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser);
	void SampleAttributes(DL_SampleInfo* info, BaseDocument* doc, DL_SceneParser* parser);

private:

	std::string transform_name;
	std::string camera_name;
	
};

#endif