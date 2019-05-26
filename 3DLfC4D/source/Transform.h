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

public:
	Transform(BaseObject* object, DL_SceneParser* parser, std::string parent);
	void SampleAttributes(DL_SampleInfo* info,  BaseDocument* document, DL_SceneParser* parser);
	std::string GetHandle();
};

#endif