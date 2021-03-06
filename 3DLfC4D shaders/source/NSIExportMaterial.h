#pragma once
#include "DL_TranslatorPlugin.h"
#include "DL_HookPlugin.h"
#include "c4d.h"
#include <string>
#include <map>
#include <utility>


class NSI_Export_Material : public DL_TranslatorPlugin {
public:

	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void ConnectNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void SampleAttributes(DL_SampleInfo* info, const char* Handle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	std::map<int, std::pair<std::string,std::string> > m_ids_to_names;
	
private:
	//std::string m_material_handle;
	//std::string m_material_attributes;
};
