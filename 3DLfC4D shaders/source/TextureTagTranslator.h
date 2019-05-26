#ifndef TEXTURE_TAG_TRANSLATOR_H
#define TEXTURE_TAG_TRANSLATOR_H

#include "DL_TranslatorPlugin.h"
#include <string>

class TextureTagTranslator : public DL_TranslatorPlugin{
public:
	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void ConnectNSINodes(const char* Handle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);

private:
	//std::string attributes_handle;
	std::string transform_handle;
	//virtual void RegisterShaders(BaseTag* node, BaseDocument* doc, DL_SceneParser* parser);
	//virtual void EmitBegin(BaseTag* node, BaseDocument* doc, DL_SceneParser* parser);
	//virtual void EmitEnd(BaseTag* node, BaseDocument* doc, DL_SceneParser* parser);
	//virtual void CacheStaticData(GeListNode* node, BaseDocument* doc, DL_SceneParser* parser);
};

#endif