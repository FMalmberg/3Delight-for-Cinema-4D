#ifndef LIGHT_CARD_TRANSLATOR_H
#define LIGHT_CARD_TRANSLATOR_H

#include "DL_TranslatorPlugin.h"
#include "c4d.h"
#include <string>

class LightCardTranslator : public DL_TranslatorPlugin{
public:
	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void SampleMotion(DL_SampleInfo* info, const char* Handle, GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser);

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