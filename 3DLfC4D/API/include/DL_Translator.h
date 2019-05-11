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

	virtual void CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser) = 0;
	virtual void ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser)=0;
	virtual void SampleMotion(DL_SampleInfo* info, GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser) = 0;

};

typedef DL_Translator* (*TranslatorAllocator)();


#endif