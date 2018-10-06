#include "ShaderNode.h"
#include "PluginManager.h"
#include "c4d.h"
#include "NS_TypeConversions.h"

extern PluginManager PM;

class ShaderTranslatorDeleter{
public:
	void operator()(NS_ShaderTranslator* translator){
		if(translator){
			translator->Free();
		}
	}
};

ShaderNode::ShaderNode(){
	c4d_node=NULL;
	AlreadyEmitted=false;
}

ShaderNode::ShaderNode(BaseList2D* node){ 
	AlreadyEmitted=false;
	if(node){
		c4d_node=node;
	
		ShaderTranslatorDeleter d;

	translator=NS_ShaderTranslatorPtr(PM.GetShaderTranslator(node->GetType()), d);
	if(translator){
		translator->Init((NS_ShaderNode*)this);
	}
	}

}

BaseList2D* ShaderNode::GetC4DNode(){
	return c4d_node;
}

NS_ShaderTranslator* ShaderNode::GetTranslator(){
	return translator.get();
}

char* ShaderNode::GetHandle(){
	return &HandleString[0];
}

void ShaderNode::SetHandle(String handlename){
	HandleString=StringToChars(handlename);
}

bool ShaderNode::IsEmitted(){
	return AlreadyEmitted;
}

void ShaderNode::Emit(BaseDocument* doc, NS_SceneParser* parser){
	AlreadyEmitted=true;
	if(translator){
		translator->Emit(GetHandle(), GetC4DNode(), doc, parser);
	}
	
}