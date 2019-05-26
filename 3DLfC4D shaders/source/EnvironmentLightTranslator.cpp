#include "EnvironmentLightTranslator.h"
#include "oenvironmentlight.h"
#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "IDs.h"
#include "c4d.h"
#include "nsi.hpp"

void eraseSubStr(std::string &mainStr, const std::string toErase)
{
	size_t pos = mainStr.find(toErase);

	if (pos != std::string::npos)
	{
		// If found then erase it from string
		mainStr.erase(pos, toErase.length());
	}
}

void TraverseShaders(BaseShader* shader, std::string uv_handle, DL_SceneParser* parser) {
	if (!shader) { return; }

	NSI::Context ctx(parser->GetContext());

	//std::string texture_handle = parser->GetAssociatedHandle((BaseList2D*)shader);
	std::string texture_handle = parser->GetHandleName((BaseList2D*)shader);

	ctx.SetAttribute(texture_handle, NSI::IntegerArg("is_connected_to_sky", 1));
	ctx.Connect(uv_handle, "o_outUV", texture_handle, "uvCoord");

	TraverseShaders(shader->GetDown(), uv_handle, parser);
	TraverseShaders(shader->GetNext(), uv_handle, parser);
}

void EnvironmentLightTranslator::CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser){

	NSI::Context ctx(parser->GetContext());

	//Create a mesh and connect it to the parent transform
	//m_handle = string(parser->GetUniqueName("environment_light"));
	m_handle = string(Handle);
	m_transform_handle = string(ParentTransformHandle);

	//std::string transform_handle2 = string(parser->GetUniqueName("transform"));
	std::string transform_handle2 = string(Handle) + string("transform"); 
	ctx.Create(transform_handle2, "transform");

	vector<double> change_direction
	{
		0,0,-1,0,
		0, 1,0,0,
		-1,0,0,0,
		0,0,0,1
	};

	NSI::Argument xform("transformationmatrix");
	xform.SetType(NSITypeDoubleMatrix);
	xform.SetValuePointer((void*)&change_direction[0]);
	ctx.SetAttribute(transform_handle2, xform);


	ctx.Connect(transform_handle2, "", m_transform_handle, "objects");

	ctx.Create(m_handle, "environment");
	ctx.Connect(m_handle, "", transform_handle2, "objects");

	BaseList2D *obj = (BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance();

	float intensity = 1; // data->GetFloat(ENVIRONMENT_INTENSITY);
	float exposure = 0;// data->GetFloat(ENVIRONMENT_EXPOSURE);
	Vector tint_c4d = Vector(1, 1, 1);// data->GetVector(ENVIRONMENT_TINT);
	float tint[3]{ tint_c4d.x, tint_c4d.y, tint_c4d.z };
	int mapping = 0; //data->GetInt32(ENVIRONMENT_MAPPING);
	int camera_visibility = 1; //data->GetBool(ENVIRONMENT_SEEN_BY_CAMERA);
	int prelit = 0; //data->GetBool = 0; (ENVIRONMENT_PRELIT);
	bool illumination = 1;// data->GetBool(ENVIRONMENT_ILLUMINATES_BY_DEFAULT);

	NSI::ArgumentList attributes_args;
	attributes_args.Add(new NSI::IntegerArg("visibility.camera", camera_visibility));
	attributes_args.Add(new NSI::IntegerArg("prelit", prelit));
	attributes_args.Add(new NSI::IntegerArg("visibility", illumination));

	//Create an attributes node, and connect it to the mesh
	//string attributes_handle = string(parser->GetUniqueName("light_attributes"));
	string attributes_handle = string(Handle) + string("attributes");
	ctx.Create(attributes_handle, "attributes");
	ctx.SetAttribute(attributes_handle, attributes_args);
	ctx.Connect(attributes_handle, "", m_handle, "geometryattributes");

	//Create a shader for the mesh and connect it to the geometry attributes of the mesh
	//m_shader_handle = string(parser->GetUniqueName("environmentlight_shader"));
	m_shader_handle = string(Handle) + string("shader");
	ctx.Create(m_shader_handle, "shader");
	ctx.Connect(m_shader_handle, "", attributes_handle, "surfaceshader");

	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlEnvironmentShape.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());

	Filename texturefile = data->GetFilename(ENVIRONMENT_TEXTURE);
	string texturename = StringToStdString(texturefile.GetString());

	NSI::ArgumentList shader_args;
	shader_args.Add(new NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])));
	shader_args.Add(new NSI::FloatArg("intensity", intensity));
	shader_args.Add(new NSI::FloatArg("exposure", exposure));
	shader_args.Add(new NSI::ColorArg("tint", tint));
	shader_args.Add(new NSI::IntegerArg("mapping", mapping));
	ctx.SetAttribute(m_shader_handle, shader_args);

	//parser->SetAssociatedHandle((BaseList2D*)C4DNode, m_handle.c_str());

}


void EnvironmentLightTranslator::SampleAttributes(DL_SampleInfo* info, const char* Handle, BaseList2D* C4DNode, BaseDocument* doc,DL_SceneParser* parser){
	
	NSI::Context ctx(parser->GetContext());

	BaseObject *obj = (BaseObject*)C4DNode;
	BaseContainer *data = obj->GetDataInstance();

	//Temporarily commenting out the code below/Filip
	//Should this really be in SampleAttributes and not in ConnectNodes?

	/*Filename uv_shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("uvCoordEnvironment.oso"));
	std::string uv_handle = parser->GetUniqueName("Uv_Environment");
	ctx.Create(uv_handle, "shader");
	vector<char> c_shaderpath = StringToChars(uv_shaderpath.GetString());

	ctx.SetAttribute(uv_handle, NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])));


	TextureTag* tag = static_cast<TextureTag*>(obj->GetTag(Ttexture));
	if (!tag)
		return;
	BaseList2D* material = tag->GetMaterial();
	if (!material)
		return;

	BaseList2D* shader = material->GetFirstShader();
	TraverseShaders((BaseShader*)shader, uv_handle, parser);

	std::string material_handle = parser->GetAssociatedHandle((BaseList2D*)material);
	std::string to_erase = "attribute_";
	eraseSubStr(material_handle, to_erase);
	ctx.Connect(material_handle, "outColor", m_shader_handle, "i_texture");*/


}
