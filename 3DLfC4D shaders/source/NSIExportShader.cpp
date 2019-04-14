#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "NSIExportShader.h"
#include <assert.h>
/*
	NSI_Export_Shader Translater class is the class for exporting C4D shaders
	to NSI shaders. Same as with the material this class with it's functions
	will only be executed as many times as the number of shaders
	and subshader created. 

	CreateNSINodes() function will create the NSI shader for the current shader
	with a unique name each time there is a new material and connect it with it's
	corresponding OSL shader containing attributes of the current shader.

	ConnectNSINodes() will check for the if any of the used shaders on the material 
	have a subshader linked with them.
*/
void NSI_Export_Shader::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());

	BaseShader* shader = (BaseShader*)C4DNode;
	m_shader_handle = parser->GetUniqueName(shader->GetTypeName().GetCStringCopy());
	ctx.Create(m_shader_handle, "shader");
	 
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename(shader->GetTypeName().GetCStringCopy()));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	ctx.SetAttribute(m_shader_handle, NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])));
	
	Int32 Id;
	GeData* data = nullptr;
	BaseContainer* shader_container = shader->GetDataInstance();
	BrowseContainer browse(shader_container);

	NSI::ArgumentList args;
	while (browse.GetNext(&Id,&data))
	{
		std::string osl_parameter_name = "_" + std::to_string(Id);
		switch (data->GetType())
		{
			case DA_LONG:
			{
				Int32 value = data->GetInt32();
				args.Add(new NSI::IntegerArg(osl_parameter_name, value));
				break;
			}

			case DA_REAL: //Float data type
			{
				Float float_value = data->GetFloat();
				args.Add(new NSI::FloatArg(osl_parameter_name, float_value));
				break;
			}

			case DA_VECTOR: //Vector data type
			{
				Vector c4d_vector_value = data->GetVector();
				const float vector_val[3] = { c4d_vector_value.x,c4d_vector_value.y, c4d_vector_value.z };
				args.Add(new NSI::ColorArg(osl_parameter_name, &vector_val[0]));
				break;
			}

		default:
			break;
		}

	};
	ctx.SetAttribute(m_shader_handle, args);
	parser->SetAssociatedHandle((BaseList2D*)C4DNode, m_shader_handle.c_str());
}

void NSI_Export_Shader::ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	/*
		In this function we search the applied shaders if they have any other shader
		connected to them. If so we find which is this subshader and connect it 
		with the parent shader.
	*/
	NSI::Context ctx(parser->GetContext());
	BaseShader* shader = (BaseShader*)C4DNode;
	BaseContainer* shader_container = shader->GetDataInstance();

	Int32 Id;
	GeData* data = nullptr;
	BrowseContainer browse(shader_container);
	while (browse.GetNext(&Id,&data))
	{
		if (data->GetType() != DA_ALIASLINK)
			continue;
		std::string osl_parameter_name = "_" + std::to_string(Id);
		BaseList2D* shader = data->GetLink(doc);
		if (!shader)
			continue;

		std::string link_shader = parser->GetAssociatedHandle(shader);
		ctx.Connect(link_shader, "output", m_shader_handle, osl_parameter_name);
		
		#ifdef VERBOSE
			ApplicationOutput("ShaderParent @ Parameter ID @, Shader @", m_shader_handle.c_str(), Id, link_shader.c_str());
		#endif // VERBOSE
	}
}