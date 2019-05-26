#ifndef ENVIRONMENT_LIGHT_TRANSLATOR_H
#define ENVIRONMENT_LIGHT_TRANSLATOR_H

#include "DL_TranslatorPlugin.h"
#include "c4d.h"
#include <string>

class EnvironmentLightTranslator : public DL_TranslatorPlugin {
public:

	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void SampleAttributes(DL_SampleInfo* info, const char* Handle, BaseList2D* C4DNode, BaseDocument* doc,DL_SceneParser* parser);

private:
	std::string m_handle;
	std::string m_shader_handle;
	std::string m_transform_handle;

};
#endif