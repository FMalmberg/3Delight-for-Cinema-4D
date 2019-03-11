#include "TextureShaderTranslator.h"
#include "xtextureshader.h"
#include "DL_TypeConversions.h"
#include "nsi.hpp"

void TextureShaderTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Create the shader node
	string shader_handle = string(parser->GetUniqueName("texture"));
	ctx.Create(shader_handle, "shader");

	BaseObject* obj = (BaseObject*)C4DNode;
	BaseContainer* data = obj->GetDataInstance();

	//Shader file path
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("Texture.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());

	Filename texturefile = data->GetFilename(TEXTURE_FILE);
	string texturename = StringToStdString(texturefile.GetString());

	ctx.SetAttribute(shader_handle, (
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])),
		NSI::StringArg("texname", texturename)
		));

	parser->SetAssociatedHandle((BaseList2D*)C4DNode, shader_handle.c_str());
}