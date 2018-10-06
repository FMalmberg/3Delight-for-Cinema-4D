#ifndef NODE_H
#define NODE_H

#include "NS_Node.h"
#include "NS_Translator.h"
#include "c4d.h"
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

//class Node;
//typedef boost::scoped_ptr<Node> NodePtr;
typedef boost::shared_ptr<NS_Translator> NS_TranslatorPtr;

class Node: public NS_Node{
private:

	bool isVisible;

	GeListNode* c4d_node;
	//BaseDocument* c4d_document;

	NS_TranslatorPtr translator;

public:
	//Inherited from ObjectNode
	//virtual NS_ObjectNode* GetNext()=0;
	//virtual NS_ObjectNode* GetChild()=0;
	//virtual void MarkAsInputObject() = 0;

	//Inherited from Node
	//virtual NS_Translator* GetTranslator() = 0;
	//virtual GeListNode* GetC4DNode() = 0;

	Node(GeListNode* listnode); 
	//virtual ~ObjectNode();

	void SetVisibility(bool visible);

	//void AddTag(TagNode t);
	//virtual TagNode* GetTags();
	//virtual long GetNTags();

	virtual NS_Translator* GetTranslator();
	virtual GeListNode* GetC4DNode();
	bool IsVisible();

};

#endif