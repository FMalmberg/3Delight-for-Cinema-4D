#include "LightOptionsHook.h"
#include "ri.h"
#include "c4d.h"
#include "DL_TypeConversions.h"
#include "DL_Utilities.h"
#include "IDs.h"
#include "oenvironmentlight.h"
#include "nsi_ri.h"
#include "NSI_tools.h"

void LightOptionsHook::Emit(BaseDocument* doc, DL_SceneParser* parser){
	Filename shaderpath=Filename(GeGetPluginPath()+Filename("\OSL"));
	vector<char> c_shaderpath =StringToChars("&; "+shaderpath.GetString());
	RtString rt_shaderpath=&c_shaderpath[0];
	RiOption( "searchpath", "shader", &rt_shaderpath, RI_NULL );

	//Export environment light info
	BaseObject* env=doc->GetHighest(ID_ENVIRONMENTLIGHT,false);
	if(env){
		//Get contexts
		NSIContext_t ctx;
		RtContextHandle RiCtx;
		RiCtx = RiGetContext();
		ctx = RiToNSIContext(RiCtx);

		//NSIHandle_t node_handle = "EnvironmentNode";
		//NSICreate(ctx, node_handle, "environment", 0, NULL);

		//NSIConnect(ctx,node_handle, "", "root","", 0, NULL );

		NSIHandle_t shader_handle = "EnvironmentLightShader";
		NSICreate(ctx, shader_handle, "shader", 0, NULL);
		vector<NSIParam_t> shader_parameters;
		char* shaderfile="EnvLight.oso";
		shader_parameters.push_back( GetParameter("shaderfilename", NSITypeString, &shaderfile) );
		
		BaseContainer* envdata=env->GetDataInstance();

		Filename file=envdata->GetFilename(ENVIRONMENT_TEXTURE,Filename(""));
		vector<char> envname=StringToChars(file.GetString());
		RtString rt_envname=&envname[0];
		//RiOption("user","string DL_environment_texture",&rt_envname,RI_NULL);

		RtFloat envintensity=toLinear(envdata->GetReal(ENVIRONMENT_INTENSITY));
		envintensity=envintensity*pow(2,envdata->GetReal(ENVIRONMENT_EXPOSURE));
		shader_parameters.push_back( GetParameter("intensity", NSITypeFloat, &envintensity) );

		Vector color=envdata->GetVector(ENVIRONMENT_TINT);

		RtColor envtint;
		envtint[0]=envintensity*toLinear(color.x);
		envtint[1]=envintensity*toLinear(color.y);
		envtint[2]=envintensity*toLinear(color.z);
		shader_parameters.push_back( GetParameter("tint", NSITypeColor, &envtint[0]) );
		//RiOption("user","color DL_environment_tint", &envtint, RI_NULL);
		
		NSISetAttribute(ctx,shader_handle,shader_parameters.size(), &shader_parameters[0] );

		bool seen_by_camera=envdata->GetBool(ENVIRONMENT_SEEN_BY_CAMERA);
		if(seen_by_camera){
			RiOption("render","string oslbackground",&shader_handle, "string oslenvironment",&shader_handle, RI_NULL);
		}
		else{
			RiOption("render","string oslenvironment",&shader_handle, RI_NULL);
		}
		//RiOption("user","string DL_environment_texture",&rt_envname,RI_NULL);

		//NSIHandle_t attribute_handle = "EnvironmentAttributes";
		//NSICreate(ctx, attribute_handle, "attribute", 0, NULL);
		//NSIConnect(ctx,shader_handle,"",attribute_handle,"surfaceshader",0,NULL);

		/*Matrix flip;
		flip.v1=Vector(-1,0,0);
		flip.v2=Vector(0,0,1);
		flip.v3=Vector(0,-1,0);
		flip=!flip;
		RiAttributeBegin();
		RiIdentity();
		RtMatrix rtm;
		RiTransform(MatrixToRtMatrix(flip,rtm));
		RiCoordinateSystem("DL_environment_space");
		RiAttributeEnd();
		//BaseContainer* envdata=env->GetDataInstance();
		Filename file=envdata->GetFilename(ENVIRONMENT_TEXTURE,Filename(""));
		if(file.GetString()==""){
			file=Filename(GeGetPluginPath()+Filename("res")+Filename("default_env.tdl"));
		}*/

		//=envdata->GetReal(ENVIRONMENT_INTENSITY)
		/*vector<char> envname=StringToChars(file.GetString());
		RtString rt_envname=&envname[0];
		RiOption("user","string DL_environment_texture",&rt_envname,RI_NULL);

		RtFloat envintensity=toLinear(envdata->GetReal(ENVIRONMENT_INTENSITY));
		envintensity=envintensity*pow(2,envdata->GetReal(ENVIRONMENT_EXPOSURE));

		Vector color=envdata->GetVector(ENVIRONMENT_TINT);

		RtColor envtint;
		envtint[0]=envintensity*toLinear(color.x);
		envtint[1]=envintensity*toLinear(color.y);
		envtint[2]=envintensity*toLinear(color.z);
		RiOption("user","color DL_environment_tint", &envtint, RI_NULL);*/
		//RiOption("user","float DL_environment_intensity",&envintensity,RI_NULL);
		//RiOption("user"...,
		//ps->SetString("Scene::Environment",StringToStdString(file.GetString()));;
		//ps->SetFloat("Scene:Environment_intensity",envdata->GetReal(ENVIRONMENT_INTENSITY));
	}

}