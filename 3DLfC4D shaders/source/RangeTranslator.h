#ifndef RANGE_TRANSLATOR_H
#define RANGE_TRANSLATOR_H

#include "DL_TranslatorPlugin.h"
#include "c4d.h"
#include <string>

class RangeTranslator : public DL_TranslatorPlugin{
public:
	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void ConnectNSINodes(const char* Handle, BaseList2D* C4DNode, BaseDocument* doc,DL_SceneParser* parser);
	
private:
	std::string shader_handle;
};


#endif