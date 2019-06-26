#ifndef NODE_H
#define NODE_H

#include "DL_Translator.h"
#include "c4d.h"
#include <vector>

#include <memory> //Shared pointer

typedef std::shared_ptr<DL_Translator> DL_TranslatorPtr;
//typedef std::shared_ptr<BaseLink> BaseLinkPtr;


struct HierarchyData {
	bool isVisible;
	int cachePos; //Position in the cache hierarchy. Used to keep track of virtual objects. 
	int deformationSamples;
	int transformSamples;
	std::string parent_transform;
};

class Node{ 
private:
	
	
	BaseList2D* c4d_node;
	DL_TranslatorPtr translator;


public:
	HierarchyData hdata;
	std::string handle;

	/*bool isVisible;
	int cachePos; //Position in the cache hierarchy. Used to keep track of virtual objects. 
	int motionSamples;
	std::string parent_transform;*/

	Int64 dirty_checksum; //Used to determine if the node has been modified and needs to be updated during interactive rendering
	
	Node(BaseList2D* listnode); 

	//void SetVisibility(bool visible);

	DL_Translator* GetTranslator();
	BaseList2D* GetC4DNode();
	
	//Remove this function, use direct access instead
	//bool IsVisible();

};

#endif