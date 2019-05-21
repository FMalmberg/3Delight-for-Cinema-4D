#include "EnvironmentLightTranslator.h"
#include "oenvironmentlight.h"

#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
//#include "Util.h"

void EnvironmentLightTranslator::CreateNSINodes(const char* Handle, const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Create a mesh and connect it to the parent transform
	handle=string(parser->GetUniqueName("environment_light"));
	transform_handle=string(ParentTransformHandle);

	//GePrint("Env light");

	ctx.Create(handle, "environment");
	ctx.Connect(handle,"",transform_handle,"objects");

	BaseList2D *obj=(BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance(); 

	

	float intensity = data->GetFloat(ENVIRONMENT_INTENSITY);
	float exposure = data->GetFloat(ENVIRONMENT_EXPOSURE);
	Vector tint_c4d = data->GetVector(ENVIRONMENT_TINT);
	float tint[3]{ tint_c4d.x, tint_c4d.y, tint_c4d.z };
	int mapping = data->GetInt32(ENVIRONMENT_MAPPING);
	int camera_visibility = data->GetBool(ENVIRONMENT_SEEN_BY_CAMERA);
	int prelit = data->GetBool(ENVIRONMENT_PRELIT);
	bool illumination = data->GetBool(ENVIRONMENT_ILLUMINATES_BY_DEFAULT);

	NSI::ArgumentList attributes_args;
	attributes_args.Add(new NSI::IntegerArg("visibility.camera", camera_visibility));
	attributes_args.Add(new NSI::IntegerArg("prelit", prelit));
	attributes_args.Add(new NSI::IntegerArg("visibility", illumination));

	//Create an attributes node, and connect it to the mesh
	string attributes_handle=string(parser->GetUniqueName("light_attributes"));
	ctx.Create(attributes_handle, "attributes");
	ctx.SetAttribute(attributes_handle,attributes_args);
	ctx.Connect(attributes_handle,"",handle,"geometryattributes");

	//Create a shader for the mesh and connect it to the geometry attributes of the mesh
	shader_handle=string(parser->GetUniqueName("environmentlight_shader"));
	ctx.Create(shader_handle, "shader");
	ctx.Connect(shader_handle,"",attributes_handle,"surfaceshader");

	Filename shaderpath=Filename(GeGetPluginPath()+Filename("OSL")+Filename("dlEnvironmentShape.oso"));
	vector<char> c_shaderpath =StringToChars(shaderpath.GetString());

	Filename texturefile = data->GetFilename(ENVIRONMENT_TEXTURE);
	string texturename=StringToStdString(texturefile.GetString());

	NSI::ArgumentList shader_args;
	shader_args.Add(new NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0])));
	shader_args.Add(new NSI::FloatArg("intensity", intensity));
	shader_args.Add(new NSI::FloatArg("exposure", exposure));
	shader_args.Add(new NSI::ColorArg("tint", tint));
	shader_args.Add(new NSI::IntegerArg("mapping", mapping));
	ctx.SetAttribute(shader_handle,shader_args);


	BaseList2D* texture_shader = data->GetLink(ENVIRONMENT_TEXTURE_SHADER, doc);
	if (texture_shader)
	{
		string texture_handle = string(parser->GetAssociatedHandle(texture_shader));
		if (texture_handle != "")
		{
			ctx.Connect(texture_handle, "outColor", shader_handle, "i_texture");
		}
	}


	parser->SetAssociatedHandle((BaseList2D*)C4DNode, handle.c_str());

}

void EnvironmentLightTranslator::SampleMotion(DL_SampleInfo* info, const char* Handle, GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser){
	
	NSI::Context ctx(parser->GetContext());

	BaseList2D *obj=(BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance(); 
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