#ifndef ENVIRONMENT_LIGHT_TRANSLATOR_H
#define ENVIRONMENT_LIGHT_TRANSLATOR_H

#include "DL_TranslatorPlugin.h"
#include "c4d.h"
#include <string>

class EnvironmentLightTranslator : public DL_TranslatorPlugin{
public:
	virtual void CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void SampleMotion(double t, long i, GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser);

	//virtual void CacheStaticData(BaseObject* obj, BaseDocument* doc, DL_SceneParser* parser);
	//virtual void Emit(BaseObject* obj, BaseDocument* doc, DL_SceneParser* parser);

private:
	//float width;
	//float height;
	//float intensity;
	//float spread;
	//Filename texturename;
	//bool visible;
	//Vector color;
	//Matrix transform;

	std::string handle;
	std::string shader_handle;
	std::string transform_handle;
	
};


#endif