#include "EnvironmentLightTranslator.h"
#include "oenvironmentlight.h"

#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
//#include "Util.h"

void EnvironmentLightTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Create a mesh and connect it to the parent transform
	handle=string(parser->GetUniqueName("environment_light"));
	transform_handle=string(ParentTransformHandle);

	//GePrint("Env light");

	ctx.Create(handle, "environment");
	ctx.Connect(handle,"",transform_handle,"objects");

	BaseList2D *obj=(BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance(); 

	Bool seen_by_camera=data->GetBool(ENVIRONMENT_SEEN_BY_CAMERA);
	int camera_visibility=0;
	if(seen_by_camera){
		camera_visibility=1;
	}

	//Create an attributes node, and connect it to the mesh
	string attributes_handle=string(parser->GetUniqueName("light_attributes"));
	ctx.Create(attributes_handle, "attributes");
	ctx.SetAttribute(attributes_handle,(
		NSI::IntegerArg("visibility.camera",camera_visibility)));
	ctx.Connect(attributes_handle,"",handle,"geometryattributes");

	//Create a shader for the mesh and connect it to the geometry attributes of the mesh
	shader_handle=string(parser->GetUniqueName("environmentlight_shader"));
	ctx.Create(shader_handle, "shader");
	ctx.Connect(shader_handle,"",attributes_handle,"surfaceshader");

	Filename shaderpath=Filename(GeGetPluginPath()+Filename("OSL")+Filename("EnvLight.oso"));
	vector<char> c_shaderpath =StringToChars(shaderpath.GetString());

	ctx.SetAttribute(shader_handle, (
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0]))
		));

	Filename texturefile = data->GetFilename(ENVIRONMENT_TEXTURE);
	if (texturefile.GetFileString() != String("")){
		Filename texturefile_absolute;
		GenerateTexturePath(doc->GetDocumentPath(), texturefile, Filename(), &texturefile_absolute);

		string texturename = StringToStdString(texturefile_absolute.GetString());

		string colorspace = "linear";
		long cs = data->GetInt32(ENVIRONMENT_TEXTURE_COLORSPACE);
		if (cs == ENVIRONMENT_TEXTURE_COLORSPACE_SRGB){
			colorspace = "sRGB";
		}

		ctx.SetAttribute(shader_handle, (
			NSI::StringArg("texname", texturename),
			NSI::StringArg("texname.meta.colorspace", colorspace)
			));
	}

	parser->SetAssociatedHandle((BaseList2D*)C4DNode, handle.c_str());

	/*int nvertices=4;
	vector<int> indices(4);

	indices[0]=0; 
	indices[1]=1;
	indices[2]=2;
	indices[3]=3;

	NSI::Argument arg_nvertices("nvertices");
	arg_nvertices.SetType(NSITypeInteger);
	arg_nvertices.SetCount(1);
	arg_nvertices.SetValuePointer((void*)&nvertices);

	NSI::Argument arg_indices("P.indices");
	arg_indices.SetType(NSITypeInteger);
	arg_indices.SetCount(indices.size());
	arg_indices.SetValuePointer((void*)&indices[0]);


	ctx.SetAttribute(handle,(
		arg_nvertices,
		arg_indices
		));*/

}

void EnvironmentLightTranslator::SampleMotion(double t, long i, GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser){
	GePrint("Envlight sample motion");
	NSI::Context ctx(parser->GetContext());

	BaseList2D *obj=(BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance(); 

	//float halfwidth=0.5*float(data->GetFloat(LIGHTCARD_WIDTH));
	//float halfheight=0.5*float(data->GetFloat(LIGHTCARD_HEIGHT));

	//float halfwidth=0.5*width;
	//float halfheight=0.5*height;

	float intensity=(data->GetFloat(ENVIRONMENT_INTENSITY));

	intensity=intensity*pow(2,data->GetFloat(ENVIRONMENT_EXPOSURE));

	//spread=data->GetFloat(LIGHTCARD_SPREAD);
	
	Vector color=toLinear(data->GetVector(ENVIRONMENT_TINT),doc);

	float col[3];
	col[0]=color.x;
	col[1]=color.y;
	col[2]=color.z;


	double angle = maxon::Deg(data->GetFloat(ENVIRONMENT_ANGLE));


	ctx.SetAttributeAtTime(shader_handle,t,(
		NSI::ColorArg("tint", &col[0]),
		NSI::FloatArg("intensity",intensity)
		));

	ctx.SetAttributeAtTime(handle, t, (
		NSI::DoubleArg("angle", angle)
		));
}


//#include "DL_Utilities.h"
/*#include "DL_TypeConversions.h"
#include "DL_DataMap.h"
#include "c4d.h"
#include "ri.h"
#include "IDs.h"

void EnvironmentLightTranslator::CacheStaticData(BaseObject* obj, BaseDocument* doc, DL_SceneParser* parser){
	

	
	
}

void EnvironmentLightTranslator::Emit(BaseObject* obj, BaseDocument* doc, DL_SceneParser* parser){
	
	DL_DataMap* data=parser->GetAttributes();
	if(obj){
		Matrix flip;
		flip.v1=Vector(-1,0,0);
		flip.v2=Vector(0,0,1);
		flip.v3=Vector(0,-1,0);
		flip=!flip;
		RiAttributeBegin();
		RiIdentity();
		RtMatrix rtm;
		RiTransform(MatrixToRtMatrix(flip,rtm));
		RiCoordinateSystem("environment_space");
		RiAttributeEnd();
		BaseContainer* envdata=obj->GetDataInstance();
		Filename file=envdata->GetFilename(ENVIRONMENT_TEXTURE,Filename(""));
		GePrint("Env: "+file.GetString());
		vector<char> cfilename= StringToChars(file.GetString());
		data->SetString("Scene:Environment",&cfilename[0]);
		
		data->SetFloat("Scene:Environment_intensity",toLinear(envdata->GetFloat(ENVIRONMENT_INTENSITY)));
	}
}*/