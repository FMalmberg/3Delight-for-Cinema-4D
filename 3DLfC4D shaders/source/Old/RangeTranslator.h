#ifndef RANGE_TRANSLATOR_H
#define RANGE_TRANSLATOR_H

#include "NS_TranslatorPlugin.h"
#include "c4d.h"
#include <string>

class RangeTranslator : public NS_TranslatorPlugin{
public:
	virtual void CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, NS_SceneParser* parser);
	virtual void ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc,NS_SceneParser* parser);
	
private:
	std::string shader_handle;
};


#endif