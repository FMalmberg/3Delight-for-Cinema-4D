#include "TextureTranslator.h"
#include "otexture.h"
#include "DL_TypeConversions.h"
#include "nsi.hpp"


void TextureTranslator::CreateNSINodes(const char* Handle, const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Create the shader node
	string shader_handle=string(parser->GetUniqueName("texture"));
	ctx.Create(shader_handle, "shader");

	BaseShader* shader=(BaseShader*)C4DNode;
	BaseContainer* data=shader->GetDataInstance();

	//Shader file path
	Filename shaderpath=Filename(GeGetPluginPath()+Filename("OSL")+Filename("Texture.oso"));
	vector<char> c_shaderpath =StringToChars(shaderpath.GetString());

	Filename texturefile = data->GetFilename(BITMAPSHADER_FILENAME);
	Filename texturefile_absolute;

	GenerateTexturePath(doc->GetDocumentPath(), texturefile, Filename(), &texturefile_absolute);
	string texturename=StringToStdString(texturefile_absolute.GetString());

	string colorspace = "sRGB";

	long colorprofile = data->GetInt32(BITMAPSHADER_COLORPROFILE);

	if (colorprofile == BITMAPSHADER_COLORPROFILE_LINEAR){
		colorspace = "linear";
	}

	ctx.SetAttribute(shader_handle,(
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])),
		NSI::StringArg("texname", texturename),
		NSI::StringArg("texname.meta.colorspace",colorspace)
		));

	parser->SetAssociatedHandle((BaseList2D*)C4DNode, shader_handle.c_str());
}

