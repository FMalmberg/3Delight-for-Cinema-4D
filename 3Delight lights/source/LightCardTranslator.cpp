#include "LightCardTranslator.h"
#include "olightcard.h"
#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
//#include "ri.h"
//#include "nsi_ri.h"
//#include "NSI_tools.h"
//#include "Util.h"

void LightCardTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Create a mesh and connect it to the parent transform
	handle=string(parser->GetUniqueName("quadlight"));
	transform_handle=string(ParentTransformHandle);

	ctx.Create(handle, "mesh");
	ctx.Connect(handle,"",transform_handle,"objects");

	BaseList2D *obj=(BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance(); 

	Bool seen_by_camera=data->GetBool(LIGHTCARD_SEEN_BY_CAMERA);
	int camera_visibility=0;
	if(seen_by_camera){
		camera_visibility=1;
	}


	//Create an attributes node, and connect it to the mesh
	string attributes_handle=string(parser->GetUniqueName("light_attributes"));
	ctx.Create(attributes_handle, "attributes");
	ctx.SetAttribute(attributes_handle,(
		NSI::IntegerArg("visibility.camera",camera_visibility)//,
		//NSI::IntegerArg("visibility.shadow",-1)
		));
	ctx.Connect(attributes_handle,"",handle,"geometryattributes");

	//Create a shader for the mesh and connect it to the geometry attributes of the mesh
	shader_handle=string(parser->GetUniqueName("quadlight_shader"));
	ctx.Create(shader_handle, "shader");
	ctx.Connect(shader_handle,"",attributes_handle,"surfaceshader");

	Filename shaderpath=Filename(GeGetPluginPath()+Filename("OSL")+Filename("AreaLight.oso"));
	vector<char> c_shaderpath =StringToChars(shaderpath.GetString());

	ctx.SetAttribute(shader_handle,(
		NSI::StringArg("shaderfilename", std::string(&c_shaderpath[0]))
		));


	int nvertices=4;
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
		));

	parser->SetAssociatedHandle((BaseList2D*)C4DNode, handle.c_str());

}

void LightCardTranslator::SampleMotion(double t, long i, GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser){
	
	NSI::Context ctx(parser->GetContext());

	BaseList2D *obj=(BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance(); 

	float halfwidth=0.5*float(data->GetFloat(LIGHTCARD_WIDTH));
	float halfheight=0.5*float(data->GetFloat(LIGHTCARD_HEIGHT));

	//float halfwidth=0.5*width;
	//float halfheight=0.5*height;

	vector<float> P(4*3); //One quad

	P[0]=-halfwidth;
	P[1]=-halfheight;
	P[2]=0;

	P[3]=halfwidth;
	P[4]=-halfheight;
	P[5]=0;

	P[6]=halfwidth;
	P[7]=halfheight;
	P[8]=0;
	
	P[9]=-halfwidth;
	P[10]=halfheight;
	P[11]=0;

	NSI::Argument arg_P("P");
	arg_P.SetType(NSITypePoint);
	arg_P.SetCount(4);
	arg_P.SetValuePointer((void*)&P[0]);
	arg_P.SetFlags(NSIParamIndirect);

	ctx.SetAttributeAtTime(handle,t,(
			arg_P
		));

	float intensity=data->GetFloat(LIGHTCARD_INTENSITY);

	intensity=intensity*pow(2,data->GetFloat(LIGHTCARD_EXPOSURE));

	//spread=data->GetFloat(LIGHTCARD_SPREAD);
	
	Vector color=toLinear(data->GetVector(LIGHTCARD_COLOR),doc);

	float col[3];
	col[0]=color.x;
	col[1]=color.y;
	col[2]=color.z;

	ctx.SetAttributeAtTime(shader_handle,t,(
		NSI::ColorArg("tint", &col[0]),
		NSI::FloatArg("intensity",intensity)
		));
}

/*
void LightCardTranslator::CacheStaticData(BaseObject* obj, BaseDocument* doc, DL_SceneParser* parser){
	//GePrint("LightCard: cache static data");
	BaseContainer *data = obj->GetDataInstance(); 

	width=float(data->GetFloat(LIGHTCARD_WIDTH));
	height=float(data->GetFloat(LIGHTCARD_HEIGHT));
	intensity=toLinear(data->GetFloat(LIGHTCARD_INTENSITY));

	intensity=intensity*pow(2,data->GetFloat(LIGHTCARD_EXPOSURE));

	spread=data->GetFloat(LIGHTCARD_SPREAD);
	
	color=data->GetVector(LIGHTCARD_COLOR);
	texturename = data->GetFilename(LIGHTCARD_TEXTURE);

	visible=data->GetBool(LIGHTCARD_SEEN_BY_CAMERA);

	transform=obj->GetMgn();


	
	
}
*/

	/*
	void LightCardTranslator::Emit(BaseObject* obj, BaseDocument* doc, DL_SceneParser* parser){
	//GePrint("LightCard: emit");
	RtColor col;
	col[0]=toLinear(color.x);
	col[1]=toLinear(color.y);
	col[2]=toLinear(color.z);

	RiAttributeBegin();




	if(!visible){
		RtInt SeenByCamera=0;
		RiAttribute("visibility", "int camera", &SeenByCamera,RI_NULL);
	}


	/*vector<char> texname=StringToChars(texturename.GetString());
	RtString rt_texname=&texname[0];

	RtLightHandle handle=RiAreaLightSource("QuadLight",
					  "intensity", &intensity,
					  "lightcolor",&col,
					  "spread", &spread,
					  "uniform string filename", &rt_texname,
					  RI_NULL);

	*/

	//Get contexts
	/*NSIContext_t ctx;
	RtContextHandle RiCtx;
	RiCtx = RiGetContext();
	ctx = RiToNSIContext(RiCtx);

	String HandleString=GetIDString((BaseList2D*)obj);


	//Define a handle
	vector<char> HandleStringChars=StringToChars(HandleString);
	NSIHandle_t shader_handle = &HandleStringChars[0]; //"emission";

	//Create a NSI node for the shader
	NSICreate(ctx, shader_handle, "shader", 0, NULL);

	//Set node parameters
	vector<NSIParam_t> shader_parameters;
	
	char* shaderfile="AreaLight.oso";
	shader_parameters.push_back( GetParameter("shaderfilename", NSITypeString, &shaderfile) );
	shader_parameters.push_back( GetParameter("intensity", NSITypeFloat, &intensity) );
	shader_parameters.push_back( GetParameter("c", NSITypeColor, &col[0]) );

	
	//shader_parameters.push_back( GetParameter("c", NSITypeColor, col) );
	//NSISetAttribute(ctx,shader_handle,1, &color_p );
	NSISetAttribute(ctx,shader_handle,shader_parameters.size(), &shader_parameters[0] );
	//NSISetAttribute(ctx,shader_handle,1, &shader_parameters[1] );
	RiAttribute("nsi", "string oslsurface", (RtString*)&shader_handle, RI_NULL);


	float halfwidth=0.5*width;
	float halfheight=0.5*height;;
	
	RtPoint points[4];
	
	points[0][0]=-halfwidth;
	points[0][1]=-halfheight;
	points[0][2]=0;

	points[1][0]=halfwidth;
	points[1][1]=-halfheight;
	points[1][2]=0;

	points[2][0]=halfwidth;
	points[2][1]=halfheight;
	points[2][2]=0;
	
	points[3][0]=-halfwidth;
	points[3][1]=halfheight;
	points[3][2]=0;

	std::vector<float> st; 
	st.push_back(1); 
	st.push_back(1);
	st.push_back(0); 
	st.push_back(1);
	st.push_back(0); 
	st.push_back(0);
	st.push_back(1); 
	st.push_back(0);
	
	RiTransformBegin();
	
	RtMatrix m;
	RiTransform(MatrixToRtMatrix(transform,m));

	/*RiSurface("QuadLightSurface",
				"intensity", &intensity,
				"lightcolor",&col,
				"uniform string filename", &rt_texname,
				RI_NULL);*/
	/*RiPolygon(4,"P", (RtPointer)points,"facevarying float[2] st", &st[0],RI_NULL);
	
	RiTransformEnd();

	RiAttributeEnd(); 

	//RiIlluminate(handle, RI_TRUE);
}*/