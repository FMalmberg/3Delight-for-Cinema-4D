#include "DirectionalLightTranslator.h"
#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "odirectionallight.h"

void DirectionalLightTranslator::CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());
	m_transform_handle = string(ParentTransformHandle);

	BaseList2D *obj = (BaseList2D*)C4DNode;
	GeData data;

	string light_handle = parser->GetHandleName(obj);

	m_handle = string(Handle);
	ctx.Create(m_handle, "environment");
	m_shader_handle = string(Handle) + string("shader");
	ctx.Create(m_shader_handle, "shader");

	string attributes_handle = string(Handle) + string("attributes"); //string(parser->GetUniqueName("cinema4dLightSpotLight"));
	ctx.Create(attributes_handle, "attributes");
	ctx.Connect(attributes_handle, "", m_handle, "geometryattributes");
	ctx.Connect(m_shader_handle, "", attributes_handle, "surfaceshader");

	std::string transform_handle2 = string(Handle) + string("env_transform"); // string(parser->GetUniqueName("env_transform"));
	ctx.Create(transform_handle2, "transform");

	/*
	In 3Delight the light comes from Z+ towards the Z- direction
	whereas in Cinema4D the light comes from Z- towards the Z+
	direction.  Thus we create this tranformation matrix to change the
	direction of the light and then connect (multiply) it with the
	transformation matrix of the object.
	*/
	vector<double> change_direction
	{
		1,0,0,0,
		0,-1,0,0,
		0,0,-1,0,
		0,0,0,1
	};

	m_transform_handle = string(ParentTransformHandle);

	NSI::Argument xform("transformationmatrix");
	xform.SetType(NSITypeDoubleMatrix);
	xform.SetValuePointer((void*)&change_direction[0]);
	ctx.SetAttribute(transform_handle2, xform);


	ctx.Connect(transform_handle2, "", m_transform_handle, "objects");
	ctx.Connect(m_handle, "", transform_handle2, "objects");

	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("directionalLight.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	ctx.SetAttribute(m_shader_handle, (
		NSI::StringArg("shaderfilename", &c_shaderpath[0])
		));

}

void DirectionalLightTranslator::SampleAttributes(DL_SampleInfo* info, const char* Handle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{

	NSI::Context ctx(parser->GetContext());
	BaseList2D *obj = (BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance();

	float intensity = data->GetFloat(DIRECTIONAL_LIGHT_INTENSITY);
	float exposure = data->GetFloat(DIRECTIONAL_LIGHT_EXPOSURE);
	float angular_diameter = data->GetFloat(DIRECTIONAL_LIGHT_ANGULAR_DIAMETER);

	float diffuse_contribution = data->GetFloat(DIRECTIONAL_LIGHT_DIFFUSE);
	float specular_contribution = data->GetFloat(DIRECTIONAL_LIGHT_SPECULAR);
	float hair_contribution = data->GetFloat(DIRECTIONAL_LIGHT_HAIR);
	float volume_contribution = data->GetFloat(DIRECTIONAL_LIGHT_VOLUME);
	Vector color = toLinear(data->GetVector(DIRECTIONAL_LIGHT_COLOR), doc);

	float col[3];
	col[0] = color.x;
	col[1] = color.y;
	col[2] = color.z;

	ctx.SetAttribute(m_shader_handle,(
		NSI::ColorArg("i_color", &col[0]),
		NSI::FloatArg("intensity", intensity),
		NSI::FloatArg("exposure", exposure),
		NSI::FloatArg("diffuse_contribution", diffuse_contribution),
		NSI::FloatArg("specular_contribution", specular_contribution),
		NSI::FloatArg("hair_contribution", hair_contribution),
		NSI::FloatArg("volume_contribution", volume_contribution)
		));
	ctx.SetAttribute(m_handle, (
		NSI::DoubleArg("angle", angular_diameter)
		));
}