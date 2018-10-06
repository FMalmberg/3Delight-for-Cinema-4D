#include "TagNode.h"
#include "PluginManager.h"
#include "c4d.h"

extern PluginManager PM;

class TagTranslatorDeleter{
public:
	void operator()(NS_TagTranslator* translator){
		if(translator){
			translator->Free();
		}
	}
};

TagNode::TagNode(BaseTag* tag){ 
	c4d_tag=tag;
	
	
	TagTranslatorDeleter d;
	//translator=NS_ObjectTranslatorPtr(PM.GetObjectTranslator(obj->GetType()));
	translator=NS_TagTranslatorPtr(PM.GetTagTranslator(tag->GetType()), d);
	if(translator){
		translator->Init((NS_TagNode*)this);
	}
}

BaseTag* TagNode::GetC4DNode(){
	return c4d_tag;
}

NS_TagTranslator* TagNode::GetTranslator(){
	return translator.get();
}