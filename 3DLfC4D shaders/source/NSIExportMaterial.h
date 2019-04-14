#pragma once

#include "DL_TranslatorPlugin.h"
#include "DL_HookPlugin.h"
#include "c4d.h"
#include <string>

class NSI_Export_Material : public DL_TranslatorPlugin {
public:
	virtual void CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
private:
	std::string m_material_handle;
	std::string m_material_attributes;
};