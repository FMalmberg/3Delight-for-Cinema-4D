#include "DL_ShaderTagTranslator.h"
#include "c4d.h"
#include "nsi.hpp"
//#include "ri.h"
#include "tshader.h"
//#include "nsi_ri.h"
//#include "NSI_tools.h"
//#include "Util.h"
#include "DL_TypeConversions.h"
#include <vector>
using namespace std;

void DL_ShaderTagTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	GePrint("NS shader tag");
	NSI::Context ctx(parser->GetContext());


	


	//Create an attributes node, and connect it to the transform
	attributes_handle=string(parser->GetUniqueName("transform_attributes"));
	ctx.Create(attributes_handle, "attributes");
	ctx.Connect(attributes_handle,"",ParentTransformHandle,"geometryattributes");
	
	BaseTag* tag=(BaseTag*)C4DNode;
	BaseContainer* data=tag->GetDataInstance();

	float displacement_bound= data->GetFloat(DISPLACEMENT_BOUND,0);

	//GePrint(RealToString(displacement_bound));
	ctx.SetAttribute(attributes_handle,(
		NSI::FloatArg("displacementbound", displacement_bound)
		));
}

void DL_ShaderTagTranslator::ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser){
	
	NSI::Context ctx(parser->GetContext());

	BaseTag* tag=(BaseTag*)C4DNode;
	BaseContainer* data=tag->GetDataInstance();
	BaseList2D* surface_shader=data->GetLink(SURFACE_LINK, doc);
	if(surface_shader){
		string surface_handle=string(parser->GetAssociatedHandle(surface_shader));
		if(surface_handle!=""){
			ctx.Connect(surface_handle,"",attributes_handle,"surfaceshader");
		}
	}

	BaseList2D* displacement_shader=data->GetLink(DISPLACEMENT_LINK, doc);
	if(displacement_shader){
		//GePrint(displacement_shader->GetName());
		string displacement_handle=string(parser->GetAssociatedHandle(displacement_shader));
		//GePrint(String(displacement_handle.c_str()));
		if(displacement_handle!=""){
			//GePrint("Connecting");
			ctx.Connect(displacement_handle,"",attributes_handle,"displacementshader");
		}
	}
}

/*
void DL_ShaderTagTranslator::RegisterShaders(BaseTag* node, BaseDocument* doc, DL_SceneParser* parser){
	BaseContainer* data=node->GetDataInstance();
	BaseList2D* surface_shader=data->GetLink(SURFACE_LINK, doc);
	if(!surface_shader){
		GePrint("Got NULL");
	}
	parser->RegisterShader(surface_shader,doc);

}
*/
/*
void DL_ShaderTagTranslator::EmitBegin(BaseTag* node, BaseDocument* doc, DL_SceneParser* parser){
	//Get contexts
	NSIContext_t ctx;
	RtContextHandle RiCtx;
	RiCtx = RiGetContext();
	ctx = RiToNSIContext(RiCtx);

	BaseContainer* data=node->GetDataInstance();
	BaseList2D* surface_shader=data->GetLink(SURFACE_LINK, doc);

	RiAttributeBegin();
	char* surface_handle= parser->GetShaderHandle(surface_shader,doc,TYPE_SURFACE);

	//Define a handle
	String handle_string=GetIDString(node);
	vector<char> handlechars=StringToChars(handle_string);
	NSIHandle_t handle = &handlechars[0];

	//Create a NSI node for the shader
	NSICreate(ctx, handle, "shader", 0, NULL);

	char* shaderfile="RootSurface.oso";
	vector<NSIParam_t> params;
	params.push_back(GetParameter("shaderfilename", NSITypeString, &shaderfile));
	NSISetAttribute(ctx,handle,params.size(), &params[0] );

	if(strcmp(surface_handle,"")!=0){
		NSIConnect(ctx,surface_handle,"cl_out", handle, "cl_in", 0,NULL );
	}

	RiAttribute("nsi", "string oslsurface", (RtString*)&handle, RI_NULL);



	
	//RiSurface("red");
}
*/
/*void DL_ShaderTagTranslator::EmitEnd(BaseTag* node, BaseDocument* doc, DL_SceneParser* parser){
	RiAttributeEnd();
}*/

/*void DL_ShaderTagTranslator::CacheStaticData(GeListNode* node, BaseDocument* doc, DL_SceneParser* parser){
	GePrint("Shader tag cache static data");
}*/