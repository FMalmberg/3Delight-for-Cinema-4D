#ifndef STANDARD_MATERIAL_TRANSLATOR_H
#define STANDARD_MATERIAL_TRANSLATOR_H

#include "DL_TranslatorPlugin.h"
#include "c4d.h"
#include <string>

class StandardMaterialTranslator : public DL_TranslatorPlugin{
public:
	virtual void CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser);
	/*virtual void Emit(char* handle, BaseList2D* node, BaseDocument* doc, DL_SceneParser* parser);
	virtual void CacheStaticData(BaseList2D* node, BaseDocument* doc);
	virtual bool IsType(ShaderType type);
	virtual void RegisterSubshaders(BaseList2D* node, BaseDocument* doc, DL_SceneParser* parser);*/

//private:
	//BaseContainer data;
private:
	std::string shader_handle;
};

#endif