#ifndef CAMERA_HOOK_H
#define CAMERA_HOOK_H
#pragma warning(disable : 4265)
#include "DL_HookPlugin.h"
#include "c4d.h"
#include <vector>

class CameraHook:public DL_HookPlugin{
public:
	void CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser);
	void SampleMotion(double t, long i, BaseDocument* document, DL_SceneParser* parser);

private:

	CameraObject* camera;
	std::string transform_name;
	
};

#endif