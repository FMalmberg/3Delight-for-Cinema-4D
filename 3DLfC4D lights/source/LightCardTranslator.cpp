#include "LightCardTranslator.h"
#include "olightcard.h"
#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "../../../frameworks/core.framework/source/maxon/stringencoding.h"
//#include "ri.h"
//#include "nsi_ri.h"
//#include "NSI_tools.h"
//#include "Util.h"


void LightCardTranslator::CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser) {
	ApplicationOutput("Lightcard create nodes");
	NSI::Context ctx(parser->GetContext());

	//Create a mesh and connect it to the parent transform
	//handle = string(parser->GetUniqueName("quadlight"));
	string handle = string(Handle);
	string transform_handle = string(ParentTransformHandle);

	ctx.Create(handle, "mesh");
	ctx.Connect(handle, "", transform_handle, "objects");

	BaseList2D *obj = (BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance();

	Bool seen_by_camera = data->GetBool(LIGHTCARD_SEEN_BY_CAMERA);
	int camera_visibility = 0;
	if (seen_by_camera) {
		camera_visibility = 1;
	}


	//Create an attributes node, and connect it to the mesh
	//string attributes_handle = string(parser->GetUniqueName("light_attributes"));
	string attributes_handle = string(Handle) + string("attributes");
	ctx.Create(attributes_handle, "attributes");
	ctx.SetAttribute(attributes_handle, (
		NSI::IntegerArg("visibility.camera", camera_visibility)//,
		//NSI::IntegerArg("visibility.shadow",-1)
		));
	ctx.Connect(attributes_handle, "", handle, "geometryattributes");

	//Create a shader for the mesh and connect it to the geometry attributes of the mesh
	//shader_handle = string(parser->GetUniqueName("quadlight_shader"));
	string shader_handle = string(Handle) + string("shader");
	ctx.Create(shader_handle, "shader");
	ctx.Connect(shader_handle, "", attributes_handle, "surfaceshader");

	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("QuadLight.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	//ApplicationOutput(&c_shaderpath[0]);
	ctx.SetAttribute(shader_handle, (
		NSI::StringArg("shaderfilename", &c_shaderpath[0])
		));


	int nvertices = 4;
	vector<int> indices(4);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;

	NSI::Argument arg_nvertices("nvertices");
	arg_nvertices.SetType(NSITypeInteger);
	arg_nvertices.SetCount(1);
	arg_nvertices.SetValuePointer((void*)&nvertices);

	NSI::Argument arg_indices("P.indices");
	arg_indices.SetType(NSITypeInteger);
	arg_indices.SetCount(indices.size());
	arg_indices.SetValuePointer((void*)&indices[0]);


	ctx.SetAttribute(handle, (
		arg_nvertices,
		arg_indices
		));
	//string str = string(handle.c_str());
	
	//const char* hndl = handle.c_str();
	//parser->SetAssociatedHandle((BaseList2D*)C4DNode, hndl);

}

void LightCardTranslator::SampleAttributes(DL_SampleInfo* info, const char* Handle,  BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser) {
	ApplicationOutput("Lightcard sample attributes");

	NSI::Context ctx(parser->GetContext());

	BaseList2D *obj = (BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance();

	float halfwidth = 0.5*float(data->GetFloat(LIGHTCARD_WIDTH));
	float halfheight = 0.5*float(data->GetFloat(LIGHTCARD_HEIGHT));

	//float halfwidth=0.5*width;
	//float halfheight=0.5*height;

	vector<float> P(4 * 3); //One quad

	P[0] = -halfwidth;
	P[1] = -halfheight;
	P[2] = 0;

	P[3] = halfwidth;
	P[4] = -halfheight;
	P[5] = 0;

	P[6] = halfwidth;
	P[7] = halfheight;
	P[8] = 0;

	P[9] = -halfwidth;
	P[10] = halfheight;
	P[11] = 0;

	NSI::Argument arg_P("P");
	arg_P.SetType(NSITypePoint);
	arg_P.SetCount(4);
	arg_P.SetValuePointer((void*)&P[0]);
	//arg_P.SetFlags(NSIParamIndirect);

	string handle = string(Handle);

	ctx.SetAttributeAtTime(handle, info->sample_time, (
		arg_P
		));

	float intensity = data->GetFloat(LIGHTCARD_INTENSITY);

	intensity = intensity * pow(2, data->GetFloat(LIGHTCARD_EXPOSURE));

	//spread=data->GetFloat(LIGHTCARD_SPREAD);

	Vector color = toLinear(data->GetVector(LIGHTCARD_COLOR), doc);

	float col[3];
	col[0] = color.x;
	col[1] = color.y;
	col[2] = color.z;

	string shader_handle = string(Handle) + string("shader");

	ctx.SetAttributeAtTime(shader_handle, info->sample_time, (
		NSI::ColorArg("tint", &col[0]),
		NSI::FloatArg("intensity", intensity)
		));
	
}
