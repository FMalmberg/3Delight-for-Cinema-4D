#ifndef OBJECT_NODE_H
#define OBJECT_NODE_H

#include "NS_ObjectNode.h"
#include "NS_ObjectTranslator.h"
#include "c4d.h"
#include "TagNode.h"
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

class ObjectNode;
typedef boost::scoped_ptr<ObjectNode> ObjectNodePtr;
typedef boost::shared_ptr<NS_ObjectTranslator> NS_ObjectTranslatorPtr;

class ObjectNode: public NS_ObjectNode{
private:
	ObjectNodePtr child;
	ObjectNodePtr next;

	bool isVisible;
	bool isInputObject;

	BaseObject* c4d_object;
	//BaseDocument* c4d_document;

	NS_ObjectTranslatorPtr translator;

	std::vector<TagNode> tags;

public:
	//Inherited from ObjectNode
	//virtual NS_ObjectNode* GetNext()=0;
	//virtual NS_ObjectNode* GetChild()=0;
	//virtual void MarkAsInputObject() = 0;

	//Inherited from Node
	//virtual NS_Translator* GetTranslator() = 0;
	//virtual GeListNode* GetC4DNode() = 0;

	ObjectNode(BaseObject* obj); 
	//virtual ~ObjectNode();

	virtual ObjectNode* GetNext();
	virtual ObjectNode* GetChild();

	//The called object takes ownership of the passed node
	void SetNext(ObjectNode* node);
	//The called object takes ownership of the passed node
	void SetChild(ObjectNode* node);

	void SetVisibility(bool visible);

	void AddTag(TagNode t);
	virtual TagNode* GetTags();
	virtual long GetNTags();

	virtual NS_ObjectTranslator* GetTranslator();
	virtual void MarkAsInputObject();
	virtual BaseObject* GetC4DNode();
	virtual bool IsVisible();
	virtual bool IsInputObject();

};

#endif