#ifndef DL_MATERIAL_TRANSLATOR_H
#define DL_MATERIAL_TRANSLATOR_H

#include "DL_TranslatorPlugin.h"
#include <string>

class DL_material_translator : public DL_TranslatorPlugin {
public:

	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void ConnectNSINodes(const char* Handle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);


private:
	std::string attributes_handle;
};

#endif