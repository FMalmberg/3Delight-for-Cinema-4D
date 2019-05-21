#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "NSIExportBitmap.h"
#include <assert.h>
/*
	NSI_Export_Bitmap Translater class is used only to utput the colorspace
	used by the Bitmap Shader. Everything else is done in Shader_Translator.
*/
void NSI_Export_Bitmap::CreateNSINodes(const char* Handle, const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{

	NSI_Export_Shader::CreateNSINodes(Handle ,ParentTransformHandle, C4DNode, doc, parser);

	NSI::Context ctx(parser->GetContext());
	BaseShader* shader = (BaseShader*)C4DNode;
	NSI::ArgumentList args;

	BaseContainer* shader_container = shader->GetDataInstance();

	std::string color_space;
	std::string meta;

	if (shader_container->GetInt32(BITMAPSHADER_COLORPROFILE) == BITMAPSHADER_COLORPROFILE_EMBEDDED)
		color_space = "auto";

	else if (shader_container->GetInt32(BITMAPSHADER_COLORPROFILE) == BITMAPSHADER_COLORPROFILE_LINEAR)
		color_space = "linear";

	else if (shader_container->GetInt32(BITMAPSHADER_COLORPROFILE) == BITMAPSHADER_COLORPROFILE_SRGB)
		color_space = "srgb";

	else if (shader_container->GetInt32(BITMAPSHADER_COLORPROFILE) == BITMAPSHADER_COLORPROFILE_CUSTOM)
		color_space = "none";
	
	if (shader_container->GetType(BITMAPSHADER_FILENAME))
	{
		std::string osl_parameter_name = "_" + std::to_string(BITMAPSHADER_FILENAME);
		meta = osl_parameter_name + ".meta.colorspace";
		args.Add(new NSI::StringArg(meta, color_space));
		ctx.SetAttribute(m_shader_handle, args);
	}
}

