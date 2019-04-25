#ifndef NODE_H
#define NODE_H

#include "DL_Translator.h"
#include "c4d.h"
#include <vector>

#include <memory> //Shared pointer

typedef std::shared_ptr<DL_Translator> DL_TranslatorPtr;
typedef std::shared_ptr<BaseLink> BaseLinkPtr;

class Node{ //: public DL_Node{
private:

	bool isVisible;
	BaseLinkPtr c4d_node_link;
	DL_TranslatorPtr translator;

public:

	Node(BaseList2D* listnode); 

	void SetVisibility(bool visible);

	DL_Translator* GetTranslator();
	//void SetTranslator(DL_Translator* translator); //Manually override translator for this node
	BaseList2D* GetC4DNode(BaseDocument* doc);
	bool IsVisible();

};

#endif