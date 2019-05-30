#include "LightCardTranslator.h"
#include "c4dLightTranslator.h"
#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "olight.h"
#include "nsi.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <tuple>


/**
	We use the c4dLightTranslator class to convert the Cinema4D lights to
	3Delight lights.  GeData is a container class, used to store data of all
	classic data types that are supported by Cinema 4D so we use this class to
	get the light type and the parameters of each light by using GetParameter()
	function.
*/

/*
	createHemisphere() function is used to create the hemisphere. The first
	loop indicates the number of cirlces that are going to be used and the
	second loop defines the number of quads per each circle.  Then we are
	creating stripes between consecutive cirlces and therefore the hemisphere
	will be formed from all the stripes.

	The radius of the hemisphere in C4D is always from -100 to 100 but as we
	are already applying the scale transformation matrix of 100 we assume that
	it is -1 to 1
*/
std::tuple<vector<float>, vector<int>, vector<int>> createHemisphere()
{
	const int k_vertical_axis_steps = 40;
	const int k_circle = 40;

	std::vector<float> Points;
	std::vector<int> indices, nvertices;

	int indices_jump = 0;

	for (int i = 0; i < k_vertical_axis_steps; i++)
	{
		float r = (float(i) / float(k_vertical_axis_steps));
		float z = sin(acos(r));

		for (int j = 0; j < k_circle; j++)
		{
			float angle = (float(j) / float(k_circle)) *float(2.0 * M_PI);

			float x = r * float(cos(angle));
			float y = r * float(sin(angle));

			Points.push_back(x);
			Points.push_back(z);
			Points.push_back(y);
		}

		r = (float(i + 1) / float(k_vertical_axis_steps));
		z = sin(acos(r));

		for (int j = 0; j < k_circle; j++)
		{
			float angle = (float(j) / float(k_circle)) *float(2.0 * M_PI);

			float x = r * float(cos(angle));
			float y = r * float(sin(angle));

			Points.push_back(x);
			Points.push_back(z);
			Points.push_back(y);
		}

		for (int j = 0; j < k_circle; j++)
		{
			nvertices.push_back(4);
			if (j != k_circle - 1)
			{
				indices.push_back(indices_jump);
				indices.push_back(indices_jump + 1);
				indices.push_back(k_circle + indices_jump + 1);
				indices.push_back(k_circle + indices_jump);

			}

			indices_jump++;
			if (j == k_circle - 1)
			{
				indices_jump = (2 * (i + 1)) * (k_circle);
				indices.push_back(indices_jump - (2 * k_circle));
				indices.push_back(indices_jump / 2 + (k_circle*(i)-1));
				indices.push_back(indices_jump - 1);
				indices.push_back(indices_jump / 2 + (k_circle*(i)));
			}
		}
	}
	return std::make_tuple(Points, nvertices, indices);
}




void c4dLightTranslator::CreateNSINodes(
	const char* Handle,
	const char* ParentTransformHandle,
	BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());
	m_transform_handle = string(ParentTransformHandle);
	

	BaseList2D *obj = (BaseList2D*)C4DNode;
	GeData data;

	m_handle = string(Handle);
	m_attributes_handle = string(Handle) + string("attributes");
	m_shader_handle = string(Handle) + string("shader");

	obj->GetParameter(DescID(LIGHT_AREADETAILS_SIZEX), data, DESCFLAGS_GET::NONE);
	float halfwidth = float(data.GetFloat()*0.5);
	obj->GetParameter(DescID(LIGHT_AREADETAILS_SIZEY), data, DESCFLAGS_GET::NONE);
	float halfheight = float(data.GetFloat()*0.5);
	obj->GetParameter(DescID(LIGHT_AREADETAILS_SIZEZ), data, DESCFLAGS_GET::NONE);
	float zWidth = float(data.GetFloat()*0.5);

	obj->GetParameter(DescID(LIGHT_TYPE), data, DESCFLAGS_GET::NONE);
	Int32 Type = data.GetInt32();

	if (Type == LIGHT_TYPE_AREA)
	{
		obj->GetParameter(DescID(LIGHT_AREADETAILS_SHAPE), data, DESCFLAGS_GET::NONE);
		Int32 shape = data.GetInt32();

		std::string transform_cap;
		std::string transform_cap2;
		switch (shape)
		{
		case LIGHT_AREADETAILS_SHAPE_RECTANGLE:
		case LIGHT_AREADETAILS_SHAPE_CUBE:
		case LIGHT_AREADETAILS_SHAPE_HEMISPHERE:
			//m_handle = string(parser->GetUniqueName("arealight_mesh"));
			ctx.Create(m_handle, "mesh");
			break;
		case LIGHT_AREADETAILS_SHAPE_DISC:
		case LIGHT_AREADETAILS_SHAPE_SPHERE:
			halfwidth = halfwidth * 2;
			halfheight = halfheight * 2;
			zWidth = zWidth * 2;
			//m_handle = string(parser->GetUniqueName("arealight_particle"));
			ctx.Create(m_handle, "particles");
			break;

		case LIGHT_AREADETAILS_SHAPE_CYLINDER:
		case LIGHT_AREADETAILS_SHAPE_PCYLINDER:
		case LIGHT_AREADETAILS_SHAPE_LINE:
			//m_handle = string(parser->GetUniqueName("arealight_mesh"));
			ctx.Create(m_handle, "mesh");

			//m_cap1 = string(parser->GetUniqueName("cylinder_cap_front"));
			m_cap1 = string(Handle) + string("cap_front");
			ctx.Create(m_cap1, "particles");

			//m_cap2 = string(parser->GetUniqueName("cylinder_cap_back"));
			m_cap2 = string(Handle) + string("cap_back");

			ctx.Create(m_cap2, "particles");

			transform_cap = string(Handle)+string("cap_front_transform");
			ctx.Create(transform_cap, "transform");

			transform_cap2 = string(Handle) + string("cap_back_transform");
			ctx.Create(transform_cap2, "transform");
			if (shape == LIGHT_AREADETAILS_SHAPE_LINE)
			{
				double cap1[16] =
				{
					halfwidth * 0.001,0,0,0,
					0,halfheight * 0.001,0,0,
					0,0,zWidth * 2,0,
					0,0,zWidth,1
				};

				double cap2[16] =
				{
					halfwidth * 0.001,0,0,0,
					0,halfwidth * 0.001,0,0,
					0,0,zWidth * 2,0,
					0,0,-zWidth,1
				};

				NSI::Argument xform2("transformationmatrix");
				xform2.SetType(NSITypeDoubleMatrix);
				xform2.SetValuePointer((void*)&cap1[0]);
				ctx.SetAttribute(transform_cap, xform2);

				NSI::Argument xform3("transformationmatrix");
				xform3.SetType(NSITypeDoubleMatrix);
				xform3.SetValuePointer((void*)&cap2[0]);
				ctx.SetAttribute(transform_cap2, xform3);
			}
			else if (shape == LIGHT_AREADETAILS_SHAPE_CYLINDER)
			{
				double cap1[16] =
				{
					halfwidth * 2,0,0,0,
					0,halfheight * 2,0,0,
					0,0,zWidth * 2,0,
					0,0,zWidth,1
				};

				double cap2[16] =
				{
					halfwidth * 2,0,0,0,
					0,halfheight * 2,0,0,
					0,0,zWidth * 2,0,
					0,0,-zWidth,1
				};

				NSI::Argument xform2("transformationmatrix");
				xform2.SetType(NSITypeDoubleMatrix);
				xform2.SetValuePointer((void*)&cap1[0]);
				ctx.SetAttribute(transform_cap, xform2);

				NSI::Argument xform3("transformationmatrix");
				xform3.SetType(NSITypeDoubleMatrix);
				xform3.SetValuePointer((void*)&cap2[0]);
				ctx.SetAttribute(transform_cap2, xform3);
			}
			
			else
			{
				double cap1p[16] =
				{
					halfwidth * 2,0,0,0,
					0,halfheight * 2,0,0,
					0,0,zWidth * 2,0,
					halfwidth,0,0,1
				};

				double cap2p[16] =
				{
					halfwidth * 2,0,0,0,
					0,halfheight * 2,0,0,
					0,0,zWidth * 2,0,
					-halfwidth,0,-0,1
				};

				NSI::Argument xform2("transformationmatrix");
				xform2.SetType(NSITypeDoubleMatrix);
				xform2.SetValuePointer((void*)&cap1p[0]);
				ctx.SetAttribute(transform_cap, xform2);

				NSI::Argument xform3("transformationmatrix");
				xform3.SetType(NSITypeDoubleMatrix);
				xform3.SetValuePointer((void*)&cap2p[0]);
				ctx.SetAttribute(transform_cap2, xform3);
			}
	

			ctx.Connect(transform_cap, "", m_transform_handle, "objects");
			ctx.Connect(m_cap1, "", transform_cap, "objects");

			ctx.Connect(transform_cap2, "", m_transform_handle, "objects");
			ctx.Connect(m_cap2, "", transform_cap2, "objects");
			break;
		}


		//std::string scale_transform = string(parser->GetUniqueName("scale_tranform"));
		std::string scale_transform = string(Handle) + string("scale_transform");
		ctx.Create(scale_transform, "transform");
		/*
		The default length size in Cinema4D is alwyas 200 cm, whereas the default scale parameters for
		the tranformation matrix is always 1. In NSI we create the object with the default length size
		of each direction equal to 1, and then we create another transformation maatrix which will be used
		for the scaling of the object and connect it to the default transformation matrix of the object.
		So this way we have exported the object with it's proper size for each direction.
		
		*/
		NSI::Argument xform("transformationmatrix");

		if (shape == LIGHT_AREADETAILS_SHAPE_LINE)
		{
			/*
			The length of x and y direction should be the same
			no matter if we zoom in or zoom out the camera which means
			that it should be in proportional with the camera.
			*/
			BaseDraw* bd = doc->GetActiveBaseDraw();
			CameraObject* camera = (CameraObject*)bd->GetSceneCamera(doc);
			Matrix m = camera->GetMg();
			Vector x = m.off;
			vector<double> matrix = MatrixToNSIMatrix(m);
			double size = matrix[12];
			double c4dDefault = 600;
			double increase_ratio = 10;
			double finalSize = double(size / (size*(c4dDefault / size * increase_ratio)));
			//ApplicationOutput("Output Size @ @",size,finalSize);

			double scale[16]
			{
				finalSize,0,0,0,
				0,finalSize,0,0,
				0,0,zWidth,0,
				0,0,0,1
			};
			xform.SetType(NSITypeDoubleMatrix);
			xform.SetValuePointer((void*)&scale[0]);
			ctx.SetAttribute(scale_transform, (
				xform
				));
		}
		else
		{
			double scale[16]
			{
			halfwidth,0,0,0,
			0,halfheight,0,0,
			0,0,zWidth,0,
			0,0,0,1
			};
			xform.SetType(NSITypeDoubleMatrix);
			xform.SetValuePointer((void*)&scale[0]);
			ctx.SetAttribute(scale_transform, (
				xform
				));
		}		


		ctx.Connect(scale_transform, "", m_transform_handle, "objects");
		ctx.Connect(m_handle, "", scale_transform, "objects");


		obj->GetParameter(DescID(LIGHT_AREADETAILS_SHOWINRENDER), data, DESCFLAGS_GET::NONE);
		int camera_visibility = data.GetInt32();

		//string attributes_handle = string(parser->GetUniqueName("cinema4dLight"));
		string attributes_handle = string(Handle) + string("attributes");
		ctx.Create(attributes_handle, "attributes");
		ctx.SetAttribute(attributes_handle, (
			NSI::IntegerArg("visibility.camera", camera_visibility)
			));
		ctx.Connect(attributes_handle, "", m_handle, "geometryattributes");
		ctx.Connect(attributes_handle, "", m_cap1, "geometryattributes");
		ctx.Connect(attributes_handle, "", m_cap2, "geometryattributes");

		//m_shader_handle = string(parser->GetUniqueName("light_shading"));
		//m_shader_handle = string(Handle) + string("shader");
		ctx.Create(m_shader_handle, "shader");
		ctx.Connect(m_shader_handle, "", attributes_handle, "surfaceshader");

		Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("AreaLight.oso"));
		vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
		ctx.SetAttribute(m_shader_handle, (
			NSI::StringArg("shaderfilename", &c_shaderpath[0])
			));

		//const char* hndl = m_handle.c_str();
		//parser->SetAssociatedHandle((BaseList2D*)C4DNode, hndl);
	}
	else if (Type == LIGHT_TYPE_OMNI)
	{
		//m_handle = string(parser->GetUniqueName("pointlight"));
		//m_transform_handle = string(ParentTransformHandle);
		ctx.Create(m_handle, "particles");
		ctx.Connect(m_handle, "", m_transform_handle, "objects");


		obj->GetParameter(DescID(LIGHT_AREADETAILS_SHOWINRENDER), data, DESCFLAGS_GET::NONE);
		int camera_visibility = data.GetInt32();

		//m_attributes_handle = string(parser->GetUniqueName("cinema4dLight"));
		//m_attributes_handle = string(Handle) + string("attributes");
		ctx.Create(m_attributes_handle, "attributes");
		ctx.SetAttribute(m_attributes_handle, (
			NSI::IntegerArg("visibility.camera", camera_visibility)
			));

		ctx.Connect(m_attributes_handle, "", m_handle, "geometryattributes");
		//m_shader_handle = string(parser->GetUniqueName("light_shading"));
		//m_shader_handle = string(Handle) + string("shader");
		ctx.Create(m_shader_handle, "shader");
		ctx.Connect(m_shader_handle, "", m_attributes_handle, "surfaceshader");

		Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("AreaLight.oso"));
		vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
		ctx.SetAttribute(m_shader_handle, (
			NSI::StringArg("shaderfilename", &c_shaderpath[0])
			));

		float P[3] = { 0.0f, 0.0f, 0.0f };
		NSI::Argument arg_P("P");
		arg_P.SetType(NSITypePoint);
		arg_P.SetCount(1);
		arg_P.SetValuePointer((void*)&P[0]);

		ctx.SetAttribute(m_handle, (arg_P));

		//const char* hndl = m_handle.c_str();
		//parser->SetAssociatedHandle((BaseList2D*)C4DNode, hndl);

	}
	else if (Type == LIGHT_TYPE_DISTANT)
	{
		//m_handle = string(parser->GetUniqueName("directional_light"));
		//std::string transform_handle2 = string(parser->GetUniqueName("env_transform"));
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

		ctx.Create(m_handle, "environment");
		ctx.Connect(m_handle, "", transform_handle2, "objects");


		obj->GetParameter(DescID(LIGHT_AREADETAILS_SHOWINRENDER), data, DESCFLAGS_GET::NONE);
		int camera_visibility = data.GetInt32();

		//m_attributes_handle = string(parser->GetUniqueName("directional_attributes"));
		ctx.Create(m_attributes_handle, "attributes");
		ctx.SetAttribute(m_attributes_handle, (
			NSI::IntegerArg("visibility.camera", camera_visibility)
			));

		ctx.Connect(m_attributes_handle, "", m_handle, "geometryattributes");

		//m_shader_handle = string(parser->GetUniqueName("light_shading"));
		ctx.Create(m_shader_handle, "shader");
		ctx.Connect(m_shader_handle, "", m_attributes_handle, "surfaceshader");

		Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("directionalLight.oso"));
		vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
		ctx.SetAttribute(m_shader_handle, (
			NSI::StringArg("shaderfilename", &c_shaderpath[0])
			));

		const char* hndl = m_handle.c_str();
		//parser->SetAssociatedHandle((BaseList2D*)C4DNode, hndl);
	}

	else if (Type == LIGHT_TYPE_SPOT)
	{
		//m_handle = string(parser->GetUniqueName("spot_light"));
		ctx.Create(m_handle, "mesh");

		m_transform_handle = string(ParentTransformHandle);

		std::string transform = string(Handle) + string("transform"); // string(parser->GetUniqueName("spot_tranform"));
		ctx.Create(transform, "transform");

		obj->GetParameter(DescID(LIGHT_DETAILS_OUTERDISTANCE), data, DESCFLAGS_GET::NONE);
		double radius = data.GetFloat();
		double scale_direction[16]
		{
			1,0,0,0,
			0,-1,0,0,
			0,0,-1,0,
			0,0,0,1
		};

		m_transform_handle = string(ParentTransformHandle);

		NSI::Argument xform("transformationmatrix");
		xform.SetType(NSITypeDoubleMatrix);
		xform.SetValuePointer((void*)&scale_direction[0]);
		ctx.SetAttribute(transform, (
			xform
			));

		obj->GetParameter(DescID(LIGHT_AREADETAILS_SHOWINRENDER), data, DESCFLAGS_GET::NONE);
		int camera_visibility = data.GetInt32();

		//string attributes_handle = string(parser->GetUniqueName("cinema4dLightSpotLight"));
		string attributes_handle = string(Handle) + string("attributes"); //string(parser->GetUniqueName("cinema4dLightSpotLight"));
		ctx.Create(attributes_handle, "attributes");
		ctx.SetAttribute(attributes_handle, (
			NSI::IntegerArg("visibility.camera", camera_visibility)
			));

		//m_shader_handle = string(parser->GetUniqueName("spot_shading"));
		ctx.Create(m_shader_handle, "shader");
		ctx.Connect(transform, "", m_transform_handle, "objects");
		ctx.Connect(attributes_handle, "", m_handle, "geometryattributes");
		ctx.Connect(m_shader_handle, "", attributes_handle, "surfaceshader");
		ctx.Connect(m_handle, "", transform, "objects");

		Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("spotLight.oso"));
		vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
		ctx.SetAttribute(m_shader_handle, (
			NSI::StringArg("shaderfilename", &c_shaderpath[0])
			));
	}
}

void c4dLightTranslator::SampleAttributes(DL_SampleInfo* info, const char* Handle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());

	BaseList2D *obj = (BaseList2D*)C4DNode;

	GeData data;

	obj->GetParameter(DescID(LIGHT_TYPE), data, DESCFLAGS_GET::NONE);

	if (data.GetInt32() == LIGHT_TYPE_AREA)
	{
		obj->GetParameter(DescID(LIGHT_AREADETAILS_SIZEX), data, DESCFLAGS_GET::NONE);
		Float width = data.GetFloat();
		obj->GetParameter(DescID(LIGHT_AREADETAILS_SIZEY), data, DESCFLAGS_GET::NONE);
		Float height = data.GetFloat();
		float halfwidth = 0.5*float(width);
		float halfheight = 0.5*float(height);

		obj->GetParameter(DescID(LIGHT_AREADETAILS_SHAPE), data, DESCFLAGS_GET::NONE);
		Int32 shape = data.GetInt32();

		if (shape == LIGHT_AREADETAILS_SHAPE_RECTANGLE)
		{
			float P[4 * 3];
			//Building the rectangle shape for area light
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

			ctx.SetAttributeAtTime(m_handle, info->sample_time, (
				NSI::IntegerArg("nvertices", 4),
				arg_P
				));
		}

		else if (shape == LIGHT_AREADETAILS_SHAPE_DISC)
		{
			float P[3];
			P[0] = 0;
			P[1] = 0;
			P[2] = 0;

			float N[3];
			N[0] = 0;
			N[1] = 0;
			N[2] = -1;

			NSI::Argument arg_P("P");
			arg_P.SetType(NSITypePoint);
			arg_P.SetCount(1);
			arg_P.SetValuePointer((void*)&P[0]);

			NSI::Argument arg_N("N");
			arg_N.SetType(NSITypeNormal);
			arg_N.SetCount(1);
			arg_N.SetValuePointer((void*)&N[0]);


			ctx.SetAttribute(m_handle, (
				arg_P,
				NSI::FloatArg("width", 1),
				arg_N
				));
		}

		else if (shape == LIGHT_AREADETAILS_SHAPE_SPHERE)
		{
			float P[3];
			P[0] = 0;
			P[1] = 0;
			P[2] = 0;

			NSI::Argument arg_P("P");
			arg_P.SetType(NSITypePoint);
			arg_P.SetCount(1);
			arg_P.SetValuePointer((void*)&P[0]);


			ctx.SetAttribute(m_handle, (
				arg_P,
				NSI::FloatArg("width", 1)
				));
		}


		else if (shape == LIGHT_AREADETAILS_SHAPE_CUBE)
		{

			int nvertices[6]{ 4, 4, 4, 4, 4, 4 };
			int indices[6 * 4]{ 0, 1, 3, 2, 2, 3, 5, 4, 4, 5, 7, 6, 6, 7, 1, 0, 1, 7, 5, 3, 6, 0, 2, 4 };

			NSI::Argument arg_nvertices("nvertices");
			arg_nvertices.SetType(NSITypeInteger);
			arg_nvertices.SetCount(6);
			arg_nvertices.SetValuePointer((void*)&nvertices[0]);

			NSI::Argument arg_indices("P.indices");
			arg_indices.SetType(NSITypeInteger);
			arg_indices.SetCount(24);
			arg_indices.SetValuePointer((void*)&indices[0]);

			float P[24] =
			{
				-1, -1, -1,
				-1,  1, -1,
				1, -1, -1,
				1,  1, -1,
				1, -1,  1,
				1,  1,  1,
				-1, -1,  1,
				-1,  1,  1
			};


			NSI::Argument arg_P("P");
			arg_P.SetType(NSITypePoint);
			arg_P.SetCount(8);
			arg_P.SetValuePointer((void*)&P[0]);

			ctx.SetAttribute(m_handle, (
				arg_nvertices,
				arg_indices,
				arg_P
				));
		}

		else if (shape == LIGHT_AREADETAILS_SHAPE_CYLINDER || shape == LIGHT_AREADETAILS_SHAPE_PCYLINDER || shape == LIGHT_AREADETAILS_SHAPE_LINE)
		{
			std::vector<float> P;
			std::vector<int> indices, nvertices;
			const int kNumSteps = 18;
			if (shape == LIGHT_AREADETAILS_SHAPE_CYLINDER || shape == LIGHT_AREADETAILS_SHAPE_LINE)
			{
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
			}

			else if (shape == LIGHT_AREADETAILS_SHAPE_PCYLINDER)
			{
				for (int i = 0; i < kNumSteps; ++i)
				{
					float angle = (float(i) / float(kNumSteps)) * float(2.0 * M_PI);
					float z = 1.0f * cos(angle);
					float y = 1.0f * sin(angle);
					P.push_back(1.0f); P.push_back(z); P.push_back(y);
					P.push_back(-1.0f); P.push_back(z); P.push_back(y);

					nvertices.push_back(4);
					indices.push_back(i * 2);
					indices.push_back(i * 2 + 1);
					indices.push_back((i * 2 + 3) % (2 * kNumSteps));
					indices.push_back((i * 2 + 2) % (2 * kNumSteps));
				}
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

			ctx.SetAttribute(m_handle, (
				args
				));

			float Point[3];
			Point[0] = 0;
			Point[1] = 0;
			Point[2] = 0;

			float N[3];

			if (shape == LIGHT_AREADETAILS_SHAPE_CYLINDER || shape == LIGHT_AREADETAILS_SHAPE_LINE)
			{
				N[0] = 0;
				N[1] = 0;
				N[2] = -1;
			}
			else
			{
				N[0] = -1;
				N[1] = 0;
				N[2] = 0;
			}
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

		else if (shape == LIGHT_AREADETAILS_SHAPE_HEMISPHERE)
		{
			auto hemisphere_components = createHemisphere();
			vector<float> Points = get<0>(hemisphere_components);
			vector<int> nvertices = get<1>(hemisphere_components);
			vector<int> indices = get<2>(hemisphere_components);

			NSI::ArgumentList args;
			args.push(NSI::Argument::New("nvertices")
				->SetType(NSITypeInteger)
				->SetCount(nvertices.size())
				->SetValuePointer(&nvertices[0]));

			args.push(NSI::Argument::New("P")
				->SetType(NSITypePoint)
				->SetCount(Points.size() / 3)
				->SetValuePointer(&Points[0]));

			args.push(NSI::Argument::New("P.indices")
				->SetType(NSITypeInteger)
				->SetCount(2 * Points.size() / 3)
				->SetValuePointer(&indices[0]));

			ctx.SetAttribute(m_handle, (
				args
				));

		}

		obj->GetParameter(DescID(LIGHT_BRIGHTNESS), data, DESCFLAGS_GET::NONE);
		float intensity = data.GetFloat();

		obj->GetParameter(DescID(LIGHT_COLOR), data, DESCFLAGS_GET::NONE);
		Vector color = toLinear(data.GetVector(), doc);

		float col[3];
		col[0] = color.x;
		col[1] = color.y;
		col[2] = color.z;

		obj->GetParameter(DescID(LIGHT_DETAILS_DIFFUSE), data, DESCFLAGS_GET::NONE);
		int diffuse = data.GetInt32();

		obj->GetParameter(DescID(LIGHT_DETAILS_SPECULAR), data, DESCFLAGS_GET::NONE);
		int specular = data.GetInt32();

		obj->GetParameter(DescID(LIGHT_DETAILS_FALLOFF), data, DESCFLAGS_GET::NONE);
		int decay = data.GetInt32();

		if (decay == LIGHT_DETAILS_FALLOFF_NONE)
			decay == 0;
		else decay == 1;

		ctx.SetAttribute(m_shader_handle, (
			NSI::ColorArg("i_color", &col[0]),
			NSI::FloatArg("intensity", intensity),
			NSI::IntegerArg("decayRate", decay),
			NSI::FloatArg("diffuse_contribution", diffuse),
			NSI::FloatArg("specular_contribution", specular),	
			NSI::IntegerArg("normalize_area", 1),	
			NSI::IntegerArg("twosided", 1)
			));
	}

	else if (data.GetInt32() == LIGHT_TYPE_OMNI)
	{

		//obj->GetParameter(DescID(LIGHT_DETAILS_OUTERRADIUS), data, DESCFLAGS_GET::NONE);
		//float radius = data.GetFloat();
		
		//Set radius to a small, but finite value.
		//If control over the radius is desired, the user can select to use a spherical area light instead
		float radius = 0.1;

		ctx.SetAttributeAtTime(m_handle, info->sample_time, (
			NSI::FloatArg("width", radius)
			));

		obj->GetParameter(DescID(LIGHT_BRIGHTNESS), data, DESCFLAGS_GET::NONE);
		float intensity = data.GetFloat();

		obj->GetParameter(DescID(LIGHT_COLOR), data, DESCFLAGS_GET::NONE);
		Vector color = toLinear(data.GetVector(), doc);

		float col[3];
		col[0] = color.x;
		col[1] = color.y;
		col[2] = color.z;

		obj->GetParameter(DescID(LIGHT_DETAILS_DIFFUSE), data, DESCFLAGS_GET::NONE);
		int diffuse = data.GetInt32();

		obj->GetParameter(DescID(LIGHT_DETAILS_SPECULAR), data, DESCFLAGS_GET::NONE);
		int specular = data.GetInt32();

		obj->GetParameter(DescID(LIGHT_DETAILS_FALLOFF), data, DESCFLAGS_GET::NONE);
		int decay = data.GetInt32();

		if (decay == LIGHT_DETAILS_FALLOFF_NONE)
			decay == 0;
		else decay == 1;

		ctx.SetAttributeAtTime(m_shader_handle, info->sample_time, (
			NSI::ColorArg("i_color", &col[0]),
			NSI::FloatArg("intensity", intensity),
			NSI::IntegerArg("decayRate", decay),
			NSI::FloatArg("diffuse_contribution", diffuse),
			NSI::IntegerArg("normalize_area", 1),
			NSI::FloatArg("specular_contribution", specular)
			));
	}


	else if (data.GetInt32() == LIGHT_TYPE_DISTANT)
	{
		obj->GetParameter(DescID(LIGHT_BRIGHTNESS), data, DESCFLAGS_GET::NONE);
		float intensity = data.GetFloat();

		obj->GetParameter(DescID(LIGHT_COLOR), data, DESCFLAGS_GET::NONE);
		Vector color = toLinear(data.GetVector(), doc);

		float col[3];
		col[0] = color.x;
		col[1] = color.y;
		col[2] = color.z;

		obj->GetParameter(DescID(LIGHT_DETAILS_DIFFUSE), data, DESCFLAGS_GET::NONE);
		int diffuse = data.GetInt32();

		obj->GetParameter(DescID(LIGHT_DETAILS_SPECULAR), data, DESCFLAGS_GET::NONE);
		int specular = data.GetInt32();

		ctx.SetAttribute(m_shader_handle, (
			NSI::ColorArg("i_color", &col[0]),
			NSI::FloatArg("intensity", intensity),
			NSI::FloatArg("diffuse_contribution", diffuse),
			NSI::IntegerArg("normalize_area", 1),
			NSI::FloatArg("specular_contribution", specular)
			));

		ctx.SetAttribute(m_handle, (
			NSI::DoubleArg("angle", 0)
			));

	}


	else if (data.GetInt32() == LIGHT_TYPE_SPOT)
	{
		float P[4 * 3];

		P[0] = -0.1;
		P[1] = -0.1;
		P[2] = 0;

		P[3] = -0.1;
		P[4] = 0.1;
		P[5] = 0;

		P[6] = 0.1;
		P[7] = 0.1;
		P[8] = 0;

		P[9] = 0.1;
		P[10] = -0.1;
		P[11] = 0;

		NSI::Argument arg_P("P");
		arg_P.SetType(NSITypePoint);
		arg_P.SetCount(4);
		arg_P.SetValuePointer((void*)&P[0]);
		//arg_P.SetFlags(NSIParamIndirect);

		ctx.SetAttribute(m_handle, (
			NSI::IntegerArg("nvertices", 4),
			arg_P
			));


		obj->GetParameter(DescID(LIGHT_BRIGHTNESS), data, DESCFLAGS_GET::NONE);
		float intensity = data.GetFloat();

		obj->GetParameter(DescID(LIGHT_COLOR), data, DESCFLAGS_GET::NONE);
		Vector color = toLinear(data.GetVector(), doc);

		float col[3];
		col[0] = color.x;
		col[1] = color.y;
		col[2] = color.z;
		float decayRate = 0;
		obj->GetParameter(DescID(LIGHT_DETAILS_FALLOFF), data, DESCFLAGS_GET::NONE);
		if (data.GetInt32() != LIGHT_DETAILS_FALLOFF_NONE)
			decayRate = 1;

		obj->GetParameter(DescID(LIGHT_DETAILS_OUTERANGLE), data, DESCFLAGS_GET::NONE);
		float coneAngle = RadToDeg(data.GetFloat());
		ctx.SetAttribute(m_shader_handle, (
			NSI::ColorArg("i_color", &col[0]),
			NSI::FloatArg("intensity", intensity),
			NSI::IntegerArg("decayRate", decayRate),
			NSI::IntegerArg("normalize_area", 1),
			NSI::FloatArg("coneAngle", coneAngle)
			));

	}

}
