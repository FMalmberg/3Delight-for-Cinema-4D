#pragma once

#include "DL_TranslatorPlugin.h"
#include "c4d.h"
#include <string>

class IncandescenceLightTranslator : public DL_TranslatorPlugin {
public:
	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void SampleAttributes(DL_SampleInfo* info, const char* Handle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);

private:

	std::string handle;
	std::string shader_handle;
	std::string transform_handle;

};
