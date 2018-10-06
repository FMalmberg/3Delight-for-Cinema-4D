#ifndef SHADER_NODE_H
#define SHADER_NODE_H

#include "NS_API.h"
#include "NS_ShaderNode.h"
#include "c4d.h"
#include <boost/shared_ptr.hpp>
#include <vector>

typedef boost::shared_ptr<NS_ShaderTranslator> NS_ShaderTranslatorPtr;

class ShaderNode : public NS_ShaderNode{

private:
	BaseList2D* c4d_node;
	NS_ShaderTranslatorPtr translator;
	std::vector<char> HandleString;
	bool AlreadyEmitted;

public:
	ShaderNode(); 
	ShaderNode(BaseList2D* node); 
	virtual BaseList2D* GetC4DNode();
	virtual NS_ShaderTranslator* GetTranslator();
	
	char* GetHandle();
	void SetHandle(String handlename);
	bool IsEmitted();
	void Emit(BaseDocument* doc, NS_SceneParser* parser);
};

#endif