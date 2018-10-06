#include "ObjectNode.h"
#include "PluginManager.h"
#include "c4d.h"


extern PluginManager PM;


class ObjectTranslatorDeleter{
public:
	void operator()(NS_ObjectTranslator* translator){
		if(translator){
			translator->Free();
		}
	}
};


ObjectNode::ObjectNode(BaseObject* obj){ 
	c4d_object=obj;
	
	//child=0;
	//next=0;

	isVisible=true;
	isInputObject=false;
	
	ObjectTranslatorDeleter d;
	//translator=NS_ObjectTranslatorPtr(PM.GetObjectTranslator(obj->GetType()));
	translator=NS_ObjectTranslatorPtr(PM.GetObjectTranslator(obj->GetType()), d);
	if(translator){
		translator->Init((NS_ObjectNode*)this);
	}
}

//ObjectNode::~ObjectNode(){
	//if(child!=0){ delete child;}
	//if(next!=0){ delete next;}
	//if(translator!=0){ translator->Free(); }
//}

 ObjectNode* ObjectNode::GetNext(){
	return next.get();
}

 ObjectNode* ObjectNode::GetChild(){
	return child.get();
}

void ObjectNode::SetNext(ObjectNode* node){
	next.reset(node);
	//if(next!=0){ delete next;}
	//next=node;
}

void ObjectNode::SetChild(ObjectNode* node){
	child.reset(node);
	//if(child!=0){ delete child;}
	//child=node;
}

void ObjectNode::SetVisibility(bool visible){
	isVisible=visible;
}

NS_ObjectTranslator* ObjectNode::GetTranslator(){
	return translator.get();
}

void ObjectNode::MarkAsInputObject(){
	isInputObject=true;
}

BaseObject* ObjectNode::GetC4DNode(){
	return c4d_object;
}

bool ObjectNode::IsVisible(){
	return isVisible;
}

bool ObjectNode::IsInputObject(){
	return isInputObject;
}

void ObjectNode::AddTag(TagNode t){
	tags.push_back(t);
}

TagNode* ObjectNode::GetTags(){
	return &tags[0];
}

long ObjectNode::GetNTags(){
	return tags.size();
}