#include "PointLightTranslator.h"
#include "opointlight.h"
#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"


void PointLightTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Create a mesh and connect it to the parent transform
	handle = string(parser->GetUniqueName("poitlight"));
	transform_handle = string(ParentTransformHandle);

	ctx.Create(handle, "particles");
	ctx.Connect(handle, "", transform_handle, "objects");

	BaseList2D *obj = (BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance();

	Bool seen_by_camera = data->GetBool(POINTLIGHT_SEEN_BY_CAMERA);
	int camera_visibility = 0;
	if (seen_by_camera){
		camera_visibility = 1;
	}


	//Create an attributes node, and connect it to the mesh
	string attributes_handle = string(parser->GetUniqueName("light_attributes"));
	ctx.Create(attributes_handle, "attributes");
	ctx.SetAttribute(attributes_handle, (
		NSI::IntegerArg("visibility.camera", camera_visibility)//,
		));
	ctx.Connect(attributes_handle, "", handle, "geometryattributes");

	//Create a shader for the mesh and connect it to the geometry attributes of the mesh
	shader_handle = string(parser->GetUniqueName("pointlight_shader"));
	ctx.Create(shader_handle, "shader");
	ctx.Connect(shader_handle, "", attributes_handle, "surfaceshader");

	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("AreaLight.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());

	ctx.SetAttribute(shader_handle, (
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0]))
		));


	vector<float> P(3); //One particle
	P[0] = 0;
	P[1] = 0;
	P[2] = 0;

	NSI::Argument arg_P("P");
	arg_P.SetType(NSITypePoint);
	arg_P.SetCount(1);
	arg_P.SetValuePointer((void*)&P[0]);

	ctx.SetAttribute(handle,  (arg_P));

	parser->SetAssociatedHandle((BaseList2D*)C4DNode, handle.c_str());

}

void PointLightTranslator::SampleMotion(double t, long i, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){

	NSI::Context ctx(parser->GetContext());

	BaseList2D *obj = (BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance();

	float radius = data->GetFloat(POINTLIGHT_RADIUS);

	ctx.SetAttributeAtTime(handle, t, (
		NSI::FloatArg("width", radius)
		));

	float intensity = data->GetFloat(POINTLIGHT_INTENSITY);

	intensity = intensity*pow(2, data->GetFloat(POINTLIGHT_EXPOSURE));

	Vector color = toLinear(data->GetVector(POINTLIGHT_COLOR), doc);

	float col[3];
	col[0] = color.x;
	col[1] = color.y;
	col[2] = color.z;

	ctx.SetAttributeAtTime(shader_handle, t, (
		NSI::ColorArg("tint", &col[0]),
		NSI::FloatArg("intensity", intensity)
		));
}