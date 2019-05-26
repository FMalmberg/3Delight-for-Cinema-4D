#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "NSIExportMaterial.h"
#include <assert.h>
#include <map>

/*
	NSI_Export_Material Translater class is the class for exporting C4D standard materials
	to NSI materials. This class with it's functions will only be executed as many times
	as the number of materials created. So one time for each material.
	CreateNSINodes() function will create the NSI shader for the current material
	with a unique name each time there is a new material and connect it with the
	OSL shader containing all the materials attributes
	ConnectNSINodes() will check for the materials' textures if there is used any
	shader on that specific material or not. if there is we find which of the
	materials' parameter has used that shader and then use nsiConnect to connect
	that shader with it's material and parameter
*/
void NSI_Export_Material::CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());
	m_material_handle = string(Handle) + string("shader"); // string(parser->GetUniqueName("c4d_material"));
	ctx.Create(m_material_handle, "shader");
	m_material_attributes = string(Handle); // string(parser->GetUniqueName("c4d_material_attribute"));
	ctx.Create(m_material_attributes, "attributes");

	BaseMaterial* material = (BaseMaterial*)C4DNode;
	BaseContainer* material_container = material->GetDataInstance();
	vector<char> c_shaderpath;
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("Main_Shader.oso"));
	if (m_ids_to_names.size() > 0)
		c_shaderpath = StringToChars(m_ids_to_names[0].second.c_str());
	else
		c_shaderpath = StringToChars(shaderpath.GetString());


	ctx.SetAttribute(m_material_handle, NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])));

	//parser->SetAssociatedHandle((BaseList2D*)C4DNode, m_material_attributes.c_str());
}

void NSI_Export_Material::ConnectNSINodes(const char* Handle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());

	BaseMaterial* material = (BaseMaterial*)C4DNode;
	BaseContainer* material_container = material->GetDataInstance();

	Int32   id;
	GeData* data = nullptr;
	BrowseContainer browse(material_container);

	ctx.Connect(m_material_handle, "", m_material_attributes, "surfaceshader");
	while (browse.GetNext(&id, &data))
	{
		if (data->GetType() != DA_ALIASLINK)
			continue;

		BaseList2D* shader = data->GetLink(doc);
		if (!shader)
			continue;
		std::string osl_parameter_name;
		std::string osl_source_attr;
		if (m_ids_to_names.count(id) == 1)
		{
			osl_parameter_name = m_ids_to_names[id].second;
			osl_source_attr = m_ids_to_names[id].first;
		}
		else
		{
			osl_parameter_name = "_" + std::to_string(id);
			std::string osl_parameter_name = "_" + std::to_string(id);
			std::string use_shader = "shader" + osl_parameter_name; //pass this parameter to osl to check if shader is loaded or not.
			vector<float> col = { 1,1,1 };
			ctx.SetAttribute(m_material_handle, NSI::ColorArg(osl_parameter_name, &col[0]));
			ctx.SetAttribute(m_material_handle, NSI::IntegerArg(use_shader, 1));
			osl_source_attr = "outColor";
		}
		//std::string link_shader = parser->GetAssociatedHandle(shader);
		std::string link_shader = parser->GetHandleName(shader);
		ctx.Connect(link_shader, osl_source_attr, m_material_handle, osl_parameter_name);

#ifdef VERBOSE
		ApplicationOutput("Material @ Parameter ID @, Shader @", m_material_handle.c_str(), id, link_shader.c_str());
#endif // VERBOSE
	}
}

void NSI_Export_Material::SampleAttributes(DL_SampleInfo* info, const char* Handle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser) {
	if (info->sample > 0) {
		return; //Don't do motion blur sampling of shader parameters, only first sample is exported
	}

	NSI::Context ctx(parser->GetContext());

	BaseMaterial* material = (BaseMaterial*)C4DNode;
	BaseContainer* material_container = material->GetDataInstance();

	Int32   id;
	GeData* data = nullptr;
	BrowseContainer browse(material_container);

	NSI::ArgumentList args;

	std::string osl_parameter_name;
	while (browse.GetNext(&id, &data))		// loop through the values
	{
		if (m_ids_to_names.count(id) == 1)
			osl_parameter_name = m_ids_to_names[id].second;
		else
			osl_parameter_name = "_" + std::to_string(id);
		switch (data->GetType())
		{
		case DA_LONG: //Integer data type
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
			Vector c4d_vector_value = toLinear(data->GetVector(), doc);
			const float vector_val[3] = { c4d_vector_value.x,c4d_vector_value.y, c4d_vector_value.z };
			args.Add(new NSI::ColorArg(osl_parameter_name, &vector_val[0]));
			break;
		}

		default:
			break;
		}

	}
	if (args.size() > 0)
		ctx.SetAttribute(m_material_handle, args);
}
