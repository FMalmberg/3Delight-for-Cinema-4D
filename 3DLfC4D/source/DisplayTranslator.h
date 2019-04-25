#ifndef DISPLAY_TRANSLATOR_H
#define DISPLAY_TRANSLATOR_H

#include "DL_TranslatorPlugin.h"
#include "c4d.h"
#include <string>

class DisplayTranslator : public DL_TranslatorPlugin{
public:
	virtual void CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser);
private:

	std::string layer_handle;
	std::string driver_handle;
	
};


#endif