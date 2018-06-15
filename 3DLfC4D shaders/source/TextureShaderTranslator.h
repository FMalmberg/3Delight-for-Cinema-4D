#ifndef TEXTURESHADER_TRANSLATOR_H
#define TEXTURESHADER_TRANSLATOR_H

#include "DL_TranslatorPlugin.h"
#include "c4d.h"

class TextureShaderTranslator : public DL_TranslatorPlugin{
public:
	virtual void CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);

private:
	BaseContainer data;
};

#endif