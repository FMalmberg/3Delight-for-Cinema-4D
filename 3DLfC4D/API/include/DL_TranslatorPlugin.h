#ifndef DL_TRANSLATOR_PLUGIN_H
#define DL_TRANSLATOR_PLUGIN_H

#include "DL_Translator.h"

//default implementation of interface
class DL_TranslatorPlugin:public DL_Translator{

public:
	virtual ~DL_TranslatorPlugin(){  };
	virtual void Free(){ delete this; }

	virtual void Init(GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){}
	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){}
	virtual void ConnectNSINodes(const char* Handle, GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser){}
	virtual void SampleMotion(DL_SampleInfo* info, const char* Handle, GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser){}
};

template<typename T>
DL_Translator* AllocateTranslator()
{
    return new T();
}



#endif