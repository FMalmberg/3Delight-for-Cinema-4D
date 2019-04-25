#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "DL_SceneParser.h"
#include "c4d.h"
#include <string>

class Transform{
private:
	BaseObject* obj;
	std::string handle;

public:
	Transform(BaseObject* object, DL_SceneParser* parser, std::string parent);
	void SampleMotion(double t, long i,  BaseDocument* document, DL_SceneParser* parser);
	std::string GetHandle();
};

#endif