#include "RenderOptionsHook.h"
#include "IDs.h"
#include "vprendersettings.h"
#include "nsi.hpp"
//#include "DL_DataMap.h"
//#include "odisplay.h"
#include "DL_TypeConversions.h"

void RenderOptionsHook::CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	BaseContainer* settings=parser->GetSettings();
	int shading_samples=settings->GetInt32(DL_SHADING_SAMPLES,16);

	ctx.SetAttribute(NSI_SCENE_GLOBAL,(
		NSI::IntegerArg("quality.shadingsamples",shading_samples)
		));

	int diffuse_depth=settings->GetInt32(DL_MAX_DIFFUSE_DEPTH);
		ctx.SetAttribute(NSI_SCENE_GLOBAL,(
		NSI::IntegerArg("maximumraydepth.diffuse",diffuse_depth)
		));

	int reflection_depth=settings->GetInt32(DL_MAX_REFLECTION_DEPTH);
		ctx.SetAttribute(NSI_SCENE_GLOBAL,(
		NSI::IntegerArg("maximumraydepth.reflection",reflection_depth)
		));

	int refraction_depth=settings->GetInt32(DL_MAX_REFRACTION_DEPTH);
		ctx.SetAttribute(NSI_SCENE_GLOBAL,(
		NSI::IntegerArg("maximumraydepth.refraction",refraction_depth)
		));

	layer_handle=string(parser->GetUniqueName("outputlayer"));
	ctx.Create(layer_handle, "outputlayer");

	ctx.SetAttribute(layer_handle,(
		NSI::StringArg("variablename", "Ci"), 
		NSI::StringArg("layername", "test"),
		NSI::StringArg("layertype", "color"),
		NSI::StringArg("variablesource", "shader"),
		NSI::StringArg("scalarformat", "float"),
		NSI::IntegerArg("withalpha",1),
		NSI::StringArg("filter", "gaussian")
		));

	driver_handle=string(parser->GetUniqueName("outputdriver"));

	ctx.Create(driver_handle, "outputdriver");
	ctx.SetAttribute(driver_handle, (
		NSI::StringArg("drivername", "idisplay"),
		NSI::StringArg("imagefilename", "test_output")
		));


	ctx.Connect(driver_handle, "",layer_handle, "outputdrivers");
}

void RenderOptionsHook::ConnectNSINodes(BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());
	ctx.Connect(layer_handle, "", "scene_camera_screen", "outputlayers");
}

/*void ParseAOVs(BaseObject* obj, bool visible=true){
if(!obj){
return;
}

bool obj_visible=visible;
long rendermode=obj->GetRenderMode();
if(rendermode==MODE_ON){
obj_visible=true;
}
else if(rendermode==MODE_OFF){
obj_visible=false;
}

if(obj_visible && obj->GetType()==ID_DISPLAY && obj->GetDeformMode()){
BaseContainer* data=obj->GetDataInstance();

String file=data->GetString(FILENAME,"frame");
//vector<char> filechars=StringToChars(file);

Filename filepath=data->GetFilename(PATH,"");
filepath=filepath+Filename("/"+file);
//Filename filepath(path); //+Filename(file);
GePrint(filepath.GetString());
vector<char> c_filepath =StringToChars("+"+filepath.GetString());
//vector<char> filechars=StringToChars(file);
//RtString rt_file=&filechars[0];

long format=data->GetInt32(DRIVER,DRIVER_TIFF);
//vector<char> driverchars=StringToChars(driver);
//RtString rt_driver=&driverchars[0];

String variable=data->GetString(VARIABLE,"rgba");
vector<char> variablechars=StringToChars(variable);

long depth=data->GetInt32(DEPTH,DEPTH_16);

//"float quantize[4]"
//"quantize" [zero one min max]
char* driver="tiff";
char* profile="srgb";

float dither=0.5;
float exposure[2];
exposure[0]=1;
exposure[1]=2.2;
float quantization[4];

if(format==DRIVER_TIFF){
quantization[0]=0;
quantization[1]=255;
quantization[2]=0;
quantization[3]=255;	
}
else if(format==DRIVER_EXR){
driver="exr";
profile="linear";
quantization[0]=0;
quantization[1]=0;
quantization[2]=0;
quantization[3]=0;
exposure[1]=1;
}



RiDisplay(&c_filepath[0],driver,&variablechars[0],"exposure", &exposure[0],"quantize", &quantization[0],"dither", &dither,"colorprofile",profile,RI_NULL);

}

ParseAOVs(obj->GetDown(), obj_visible);
ParseAOVs(obj->GetNext(),visible);
}*/
/*
void RenderOptionsHook::Init(BaseDocument* doc, DL_SceneParser* parser){
RenderData* rd=doc->GetActiveRenderData();
BaseContainer* render_data=rd->GetDataInstance();
//bool has_vp=false;


BaseContainer* vp_data = parser->GetSettings();
if(!vp_data){return;}


//Format
film_aspect=render_data->GetReal(RDATA_FILMASPECT); // Width/Height
xres=render_data->GetInt32(RDATA_XRES);
yres=render_data->GetInt32(RDATA_YRES);


//Render mode
rendermode=vp_data->GetInt32(DL_GENERAL_MODE);


PixelSamples=vp_data->GetInt32(DL_PIXELSAMPLES);
//PixelSamplesY=vp_data->GetInt32(DL_PIXELSAMPLES_Y);
pixelfilter=vp_data->GetInt32(DL_PIXELFILTER,DL_FILTER_SINC);
FilterWidth=vp_data->GetReal(DL_FILTERWIDTH,1.0);
//FilterWidthY=vp_data->GetReal(DL_FILTERWIDTH_Y,1.0);


//Sampling
//shading_samples=64;
//raycache=1;

shading_samples=vp_data->GetInt32(DL_SHADING_SAMPLES);
raycache=0;
if(vp_data->GetBool(DL_RAYCACHE)){raycache=1;}




//useMotionBlur=vp_data->GetBool(DL_USE_MOTION_BLUR);
//deformationSamples= vp_data->GetInt32(DL_DEFORMATION_SAMPLES,2);
//transformationSamples= vp_data->GetInt32(DL_TRANSFORMATION_SAMPLES,2);
//shutterAngle=vp_data->GetReal(DL_SHUTTER_ANGLE);
shutterEfficiency=vp_data->GetReal(DL_SHUTTER_EFFICIENCY);


//Exposure
//gamma=2.2;
//if(has_vp){
gamma=vp_data->GetReal(DL_GAMMA);
//}

}*/

/*
void RenderOptionsHook::Emit(BaseDocument* doc, DL_SceneParser* parser){
//GePrint("Option hook:Emit");
RiFormat(xres,yres,1 );
RiDisplay("frame#5f","framebuffer","rgba",RI_NULL);

ParseAOVs(doc->GetFirstObject());

RiExposure(1,gamma);
//GePrint("Aspect: "+RealToString(film_aspect));

RiScreenWindow(-1,1,-1/film_aspect,1/film_aspect);


RiShutter(parser->GetShutterOpen(), parser->GetShutterClose());



RtFloat efficiency[ 2 ]; 
efficiency[0]=shutterEfficiency;
efficiency[1]=shutterEfficiency;
RiOption( "shutter", "efficiency", efficiency, RI_NULL );


RiPixelSamples(PixelSamples, PixelSamples);

DL_DataMap* parser_attributes=parser->GetAttributes();
parser_attributes->SetLong("DL_shading_samples", shading_samples);
RtFloat shading_samples_float=shading_samples;
//=envdata->GetReal(ENVIRONMENT_INTENSITY);
RiOption("user","float DL_shading_samples",&shading_samples_float,RI_NULL);

RtInt osl_shading_samples=shading_samples; //Value from UI
RiOption("osl", "int shadingsamples", &osl_shading_samples,RI_NULL);

switch(pixelfilter) {
case DL_FILTER_BOX :
RiPixelFilter(RiBoxFilter, FilterWidth, FilterWidth);
break;     
case DL_FILTER_TRIANGLE :
RiPixelFilter(RiTriangleFilter, FilterWidth, FilterWidth);
break; 
case DL_FILTER_BLACKMANN_HARRIS :
RiPixelFilter(RiBlackmanHarrisFilter, FilterWidth, FilterWidth);
break; 
case DL_FILTER_CATMULL_ROM :
RiPixelFilter(RiSeparableCatmullRomFilter, FilterWidth, FilterWidth);
break; 
case DL_FILTER_MITCHELL:
RiPixelFilter(RiMitchellFilter, FilterWidth, FilterWidth);
break; 
case DL_FILTER_BESSEL:
RiPixelFilter(RiBesselFilter, FilterWidth, FilterWidth);
break; 
case DL_FILTER_SINC:
RiPixelFilter(RiSincFilter, FilterWidth, FilterWidth);
break; 
case DL_FILTER_GAUSSIAN:
RiPixelFilter(RiGaussianFilter, FilterWidth, FilterWidth);
break; 
default:
RiPixelFilter(RiSincFilter, FilterWidth, FilterWidth);
break;
}


RiHider("osltracer",RI_NULL);

RiOption ("trace", "int diffuseraycache", &raycache, RI_NULL);
}*/