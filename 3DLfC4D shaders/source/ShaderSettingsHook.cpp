#include "ShaderSettingsHook.h"
#include "c4d.h"
#include "nsi.hpp"
#include <string>
#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
using namespace std;

void ShaderSettingsHook::CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser){

	NSI::Context ctx(parser->GetContext());

	//Create an attributes node, and connect it to the mesh
	string attributes_handle=string(parser->GetUniqueName("root_attributes"));
	ctx.Create(attributes_handle, "attributes");
	ctx.Connect(attributes_handle,"",".root","geometryattributes");

	//Create a shader for the mesh and connect it to the geometry attributes of the mesh
	std::string shader_handle(parser->GetUniqueName("default_surface_shader"));
	ctx.Create(shader_handle, "shader");
	ctx.Connect(shader_handle,"",attributes_handle,"surfaceshader");

	Filename shaderpath=Filename(GeGetPluginPath()+Filename("OSL")+Filename("DefaultSurface.oso"));
	vector<char> c_shaderpath =StringToChars(shaderpath.GetString());

	ctx.SetAttribute(shader_handle,(
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0]))
		));
}

void ShaderSettingsHook::ConnectNSINodes(BaseDocument* doc, DL_SceneParser* parser){
}
