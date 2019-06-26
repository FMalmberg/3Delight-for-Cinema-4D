#include "Node.h"
#include "PluginManager.h"
#include "c4d.h"

extern PluginManager PM;

class TranslatorDeleter{
public:
	void operator()(DL_Translator* translator){
		if(translator){
			translator->Free();
		}
	}
};

/*class BaseLinkDeleter{
public:
	void operator()(BaseLink* link){
		if (link){
			BaseLink::Free(link);
		}
	}
};*/


Node::Node(BaseList2D* listnode){ 
	c4d_node=listnode;
	hdata.deformationSamples = 2;
	hdata.transformSamples = 2;

	//BaseLinkDeleter bld;

	/*c4d_node_link = BaseLinkPtr(BaseLink::Alloc(),bld);
	if (c4d_node_link){
		c4d_node_link->SetLink((C4DAtomGoal*)listnode);
	}*/

	dirty_checksum = -1;

	//isVisible=true;

	//cachePos = 0;
	
	//motionSamples = 2;

	TranslatorDeleter d;
	if (listnode) {
		translator = DL_TranslatorPtr(PM.GetTranslator(listnode->GetType()), d);
	}
	else {
		translator = DL_TranslatorPtr(NULL);
	}
	//ApplicationOutput(String::IntToString(listnode->GetType()));
}


/*void Node::SetVisibility(bool visible){
	isVisible=visible;
}*/

DL_Translator* Node::GetTranslator(){
	return translator.get();
}


BaseList2D* Node::GetC4DNode(){
	return c4d_node;
	/*if (c4d_node_link){
		return c4d_node_link->GetLink(doc);
	}
	else return NULL;*/
}

/*bool Node::IsVisible(){
	return isVisible;
}*/
