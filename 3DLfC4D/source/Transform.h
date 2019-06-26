#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "DL_SceneParser.h"
#include "DL_SampleInfo.h"
#include "c4d.h"
#include <string>

class Transform{
private:
	BaseObject* obj;
	std::string handle;
	std::string parent;

public:
	int motionsSamples;
	Transform(std::string object_handle, BaseObject* object, std::string parent);
	void CreateNodes(BaseDocument* document, DL_SceneParser* parser);
	void SampleAttributes(DL_SampleInfo* info,  BaseDocument* document, DL_SceneParser* parser);
	std::string GetHandle();
};

#endif