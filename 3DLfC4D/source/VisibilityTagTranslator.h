#ifndef VISIBILITY_TAG_TRANSLATOR_H
#define VISIBILITY_TAG_TRANSLATOR_H

#include "DL_TranslatorPlugin.h"
#include "c4d.h"
#include <string>

class VisibilityTagTranslator : public DL_TranslatorPlugin{
public:
	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
//	virtual void ConnectNSINodes(BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);


};


#endif