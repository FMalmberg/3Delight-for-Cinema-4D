#ifndef POINTLIGHT_TRANSLATOR_H
#define POINTLIGHT_TRANSLATOR_H

#include "DL_TranslatorPlugin.h"
#include "c4d.h"
#include <string>

class PointLightTranslator : public DL_TranslatorPlugin{
public:
	virtual void CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void SampleMotion(double t, long i, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);

private:
	std::string handle;
	std::string shader_handle;
	std::string transform_handle;

};

#endif