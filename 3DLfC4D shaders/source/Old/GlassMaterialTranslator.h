#ifndef GLASS_MATERIAL_TRANSLATOR_H
#define GLASS_MATERIAL_TRANSLATOR_H

#include "DL_TranslatorPlugin.h"
#include "c4d.h"
#include <string>

class GlassMaterialTranslator : public DL_TranslatorPlugin{
public:
	virtual void CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser);
	
private:
	std::string shader_handle;
};

#endif