#ifndef DL_TRANSLATOR_H
#define DL_TRANSLATOR_H

#include "DL_SceneParser.h"
#include "DL_SampleInfo.h"

//Forward  declarations
class BaseDocument;
class GeListNode;

//virtual interface
class DL_Translator{

public:
	virtual void Free() = 0;

	virtual void Init(GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser) = 0;
	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser) = 0;
	virtual void ConnectNSINodes(const char* Handle, GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser) = 0;
	virtual void SampleMotion(DL_SampleInfo* info, const char* Handle, GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser) = 0;

};

typedef DL_Translator* (*TranslatorAllocator)();


#endif