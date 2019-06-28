#include "LightCardTranslator.h"
#include "olightcard.h"
#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#define _USE_MATH_DEFINES
#include <math.h>


void LightCardTranslator::CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser) {
	NSI::Context ctx(parser->GetContext());

	//Handle names
	std::string handle = string(Handle);
	std::string m_cap1= string(Handle) + string("cap_front");
	std::string m_cap2= string(Handle) + string("cap_back");
	std::string shader_handle = string(Handle) + string("shader");
	std::string transform_handle= string(ParentTransformHandle);

	//Create a mesh and connect it to the parent transform
	//handle = string(parser->GetUniqueName("quadlight"));
	//handle = string(Handle);
	//transform_handle = string(ParentTransformHandle);

	BaseList2D *obj = (BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance();

	float halfwidth = 0.5*float(data->GetFloat(LIGHTCARD_WIDTH));
	float halfheight = 0.5*float(data->GetFloat(LIGHTCARD_HEIGHT));
	float zHeight = 1;
	if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_RECTANGLE)
	{
		ctx.Create(handle, "mesh");
	}
	else if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_DISC || data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_SPHERE)
	{
		halfwidth = float(data->GetFloat(LIGHTCARD_WIDTH));
		halfheight = float(data->GetFloat(LIGHTCARD_HEIGHT));
		zHeight = float(data->GetFloat(LIGHTCARD_ZLENGTH));
		ctx.Create(handle, "particles");
	}

	else if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_CYLINDER)
	{
		halfwidth = 0.5*float(data->GetFloat(LIGHTCARD_WIDTH));
		halfheight = 0.5*float(data->GetFloat(LIGHTCARD_HEIGHT));
		zHeight = 0.5*float(data->GetFloat(LIGHTCARD_ZLENGTH));

		ctx.Create(handle, "mesh");

		//m_cap1 = string(parser->GetUniqueName("cylinder_cap_front"));
		//m_cap1 = string(Handle) + string("cap_front");
		ctx.Create(m_cap1, "particles");

		//m_cap2 = string(parser->GetUniqueName("cylinder_cap_back"));
		//m_cap2 = string(Handle) + string("cap_back");

		ctx.Create(m_cap2, "particles");

		string transform_cap = string(Handle) + string("cap_front_transform");
		ctx.Create(transform_cap, "transform");

		string transform_cap2 = string(Handle) + string("cap_back_transform");
		ctx.Create(transform_cap2, "transform");

		double cap11[16] =
		{
			halfwidth*2.0,0,0,
			0,halfheight*2.0,0,
			0,0,zHeight*2.0,0,
			0,0,zHeight,1
		};

		double cap22[16] =
		{
			halfwidth*2.0,0,0,0,
			0,halfheight*2.0,0,0,
			0,0,zHeight*2.0,0,
			0,0,-zHeight ,1
		};

		NSI::Argument xform2("transformationmatrix");
		xform2.SetType(NSITypeDoubleMatrix);
		xform2.SetValuePointer((void*)&cap11[0]);
		ctx.SetAttribute(transform_cap, xform2);

		NSI::Argument xform3("transformationmatrix");
		xform3.SetType(NSITypeDoubleMatrix);
		xform3.SetValuePointer((void*)&cap22[0]);
		ctx.SetAttribute(transform_cap2, xform3);

		ctx.Connect(transform_cap, "", transform_handle, "objects");
		ctx.Connect(m_cap1, "", transform_cap, "objects");

		ctx.Connect(transform_cap2, "", transform_handle, "objects");
		ctx.Connect(m_cap2, "", transform_cap2, "objects");
	}

	std::string scale_transform = string(Handle) + string("scale_transform");
	ctx.Create(scale_transform, "transform");

	double scale[16]
	{
		halfwidth,0,0,0,
		0,halfheight,0,0,
		0,0,zHeight,0,
		0,0,0,1
	};
	NSI::Argument xform("transformationmatrix");

	xform.SetType(NSITypeDoubleMatrix);
	xform.SetValuePointer((void*)&scale[0]);
	ctx.SetAttribute(scale_transform, (
		xform
		));


	ctx.Connect(scale_transform, "", transform_handle, "objects");
	ctx.Connect(handle, "", scale_transform, "objects");

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
	ctx.Connect(attributes_handle, "", m_cap1, "geometryattributes");
	ctx.Connect(attributes_handle, "", m_cap2, "geometryattributes");

	//Create a shader for the mesh and connect it to the geometry attributes of the mesh
	//shader_handle = string(parser->GetUniqueName("quadlight_shader"));
	//shader_handle = string(Handle) + string("shader");
	ctx.Create(shader_handle, "shader");
	ctx.Connect(shader_handle, "", attributes_handle, "surfaceshader");

	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("AreaLight.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
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
}

void LightCardTranslator::SampleAttributes(DL_SampleInfo* info, const char* Handle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());

	//Handle names
	std::string handle = string(Handle);
	std::string m_cap1 = string(Handle) + string("cap_front");
	std::string m_cap2 = string(Handle) + string("cap_back");
	std::string shader_handle = string(Handle) + string("shader");

	BaseList2D *obj = (BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance();

	float halfwidth = 0.5*float(data->GetFloat(LIGHTCARD_WIDTH));
	float halfheight = 0.5*float(data->GetFloat(LIGHTCARD_HEIGHT));

	if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_RECTANGLE)
	{
		vector<float> P(4 * 3); //One quad

		P[0] = -1;
		P[1] = -1;
		P[2] = 0;

		P[3] = 1;
		P[4] = -1;
		P[5] = 0;

		P[6] = 1;
		P[7] = 1;
		P[8] = 0;

		P[9] = -1;
		P[10] = 1;
		P[11] = 0;

		NSI::Argument arg_P("P");
		arg_P.SetType(NSITypePoint);
		arg_P.SetCount(4);
		arg_P.SetValuePointer((void*)&P[0]);
		//arg_P.SetFlags(NSIParamIndirect);

		ctx.SetAttributeAtTime(handle, info->sample_time, (
			arg_P
			));
	}

	else if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_DISC || data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_SPHERE)
	{
		float P[3];
		P[0] = 0;
		P[1] = 0;
		P[2] = 0;

		NSI::Argument arg_P("P");
		arg_P.SetType(NSITypePoint);
		arg_P.SetCount(1);
		arg_P.SetValuePointer((void*)&P[0]);

		ctx.SetAttribute(handle, (
			arg_P,
			NSI::FloatArg("width", 1)
			));

		if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_DISC)
		{
			float N[3];
			N[0] = 0;
			N[1] = 0;
			N[2] = -1;

			NSI::Argument arg_N("N");
			arg_N.SetType(NSITypeNormal);
			arg_N.SetCount(1);
			arg_N.SetValuePointer((void*)&N[0]);
			ctx.SetAttribute(handle, arg_N);
		}
	}

	else if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_CYLINDER)
	{
		std::vector<float> P;
		std::vector<int> indices, nvertices;
		const int kNumSteps = 30;

		for (int i = 0; i < kNumSteps; ++i)
		{
			float angle = (float(i) / float(kNumSteps)) * float(2.0 * M_PI);
			float z = 1.0f * cos(angle);
			float y = 1.0f * sin(angle);
			P.push_back(y); P.push_back(z); P.push_back(1.0f);
			P.push_back(y); P.push_back(z); P.push_back(-1.0f);

			nvertices.push_back(4);
			indices.push_back(i * 2);
			indices.push_back(i * 2 + 1);
			indices.push_back((i * 2 + 3) % (2 * kNumSteps));
			indices.push_back((i * 2 + 2) % (2 * kNumSteps));
		}

		NSI::ArgumentList args;
		args.push(NSI::Argument::New("nvertices")
			->SetType(NSITypeInteger)
			->SetCount(nvertices.size())
			->SetValuePointer(&nvertices[0]));
		args.push(NSI::Argument::New("P")
			->SetType(NSITypePoint)
			->SetCount(2 * kNumSteps)
			->SetValuePointer(&P[0]));
		args.push(NSI::Argument::New("P.indices")
			->SetType(NSITypeInteger)
			->SetCount(4 * kNumSteps)
			->SetValuePointer(&indices[0]));

		ctx.SetAttribute(handle, (
			args
			));

		float Point[3];
		Point[0] = 0;
		Point[1] = 0;
		Point[2] = 0;

		float N[3];

		N[0] = 0;
		N[1] = 0;
		N[2] = -1;

		NSI::Argument arg_P("P");
		arg_P.SetType(NSITypePoint);
		arg_P.SetCount(1);
		arg_P.SetValuePointer((void*)&Point[0]);

		NSI::Argument arg_N("N");
		arg_N.SetType(NSITypeNormal);
		arg_N.SetCount(1);
		arg_N.SetValuePointer((void*)&N[0]);


		ctx.SetAttribute(m_cap1, (
			arg_P,
			NSI::FloatArg("width", 1),
			arg_N
			));

		ctx.SetAttribute(m_cap2, (
			arg_P,
			NSI::FloatArg("width", 1),
			arg_N
			));
	}

	float intensity = data->GetFloat(LIGHTCARD_INTENSITY);
	float exposure = data->GetFloat(LIGHTCARD_EXPOSURE);
	float spread = data->GetFloat(LIGHTCARD_SPREAD);
	int decayRate = data->GetInt32(LIGHTCARD_DECAY);
	int towSided = data->GetInt32(LIGHTCARD_TWO_SIDED);
	int normalize = data->GetInt32(LIGHTCARD_NORMALIZE);
	float diffuse_contribution = data->GetFloat(LIGHTCARD_DIFFUSE);
	float specular_contribution = data->GetFloat(LIGHTCARD_SPECULAR);
	float hair_contribution = data->GetFloat(LIGHTCARD_HAIR);
	float volume_contribution = data->GetFloat(LIGHTCARD_VOLUME);

	Vector color = toLinear(data->GetVector(LIGHTCARD_COLOR), doc);

	float col[3];
	col[0] = color.x;
	col[1] = color.y;
	col[2] = color.z;

	ctx.SetAttributeAtTime(shader_handle, info->sample_time, (
		NSI::ColorArg("i_color", &col[0]),
		NSI::FloatArg("intensity", intensity),
		NSI::FloatArg("exposure", exposure),
		NSI::FloatArg("spread", spread),
		NSI::IntegerArg("decayRate", decayRate),
		NSI::IntegerArg("twosided", towSided),
		NSI::FloatArg("diffuse_contribution", diffuse_contribution),
		NSI::FloatArg("specular_contribution", specular_contribution),
		NSI::FloatArg("hair_contribution", hair_contribution),
		NSI::FloatArg("volume_contribution", volume_contribution),
		NSI::IntegerArg("normalize_area", normalize)
		));
}

