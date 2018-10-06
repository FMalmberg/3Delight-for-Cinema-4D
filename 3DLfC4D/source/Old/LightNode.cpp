#include "LightNode.h"
#include "PluginManager.h"
#include "c4d.h"

extern PluginManager PM;

class LightTranslatorDeleter{
public:
	void operator()(NS_LightTranslator* translator){
		if(translator){
			translator->Free();
		}
	}
};

LightNode::LightNode(BaseObject* object){ 
	c4d_object=object;
	
	
	LightTranslatorDeleter d;
	translator=NS_LightTranslatorPtr(PM.GetLightTranslator(object->GetType()), d);
	if(translator){
		translator->Init((NS_LightNode*)this);
	}
}

BaseObject* LightNode::GetC4DNode(){
	return c4d_object;
}

NS_LightTranslator* LightNode::GetTranslator(){
	return translator.get();
}