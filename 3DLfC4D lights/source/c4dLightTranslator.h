#pragma once

#include "DL_TranslatorPlugin.h"
#include "c4d.h"
#include <string>

class c4dLightTranslator : public DL_TranslatorPlugin {
public:
	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void SampleMotion(DL_SampleInfo* info, const char* Handle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);

private:
	std::string m_handle;
	std::string m_shader_handle;
	std::string m_transform_handle;
	std::string m_attributes_handle;
	std::string m_cap1;
	std::string m_cap2;

};
