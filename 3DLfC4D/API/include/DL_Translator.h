#ifndef DL_TRANSLATOR_H
#define DL_TRANSLATOR_H

#include "DL_SceneParser.h"
#include "DL_SampleInfo.h"

//Forward  declarations
class BaseDocument;
class BaseList2D;

//virtual interface
class DL_Translator{

public:
	virtual void Free() = 0;

	virtual void Init(BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser) = 0;
	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser) = 0;
	virtual void ConnectNSINodes(const char* Handle, BaseList2D* C4DNode, BaseDocument* doc,DL_SceneParser* parser) = 0;
	virtual void SampleAttributes(DL_SampleInfo* info, const char* Handle, BaseList2D* C4DNode, BaseDocument* doc,DL_SceneParser* parser) = 0;

};

typedef DL_Translator* (*TranslatorAllocator)();


#endif