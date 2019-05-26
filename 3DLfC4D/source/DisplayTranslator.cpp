#include "DisplayTranslator.h"
#include "odisplay.h"

#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include <customgui_inexclude.h>

void DisplayTranslator::CreateNSINodes(const char* Handle, const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	if (parser->GetRenderMode() != FINAL_RENDER){ //Don't output AOVs for preview render
		return;
	}
	NSI::Context ctx(parser->GetContext());

	BaseObject* obj=(BaseObject*)C4DNode;
	BaseContainer* data=obj->GetDataInstance();

	layer_handle = string(Handle) + string("outputlayer");
	//layer_handle=string(parser->GetUniqueName("outputlayer"));
	ctx.Create(layer_handle, "outputlayer");

	string scalarformat="float";
	long datatype=data->GetInt32(DEPTH, DEPTH_16);
	long drivertype_ID=data->GetInt32(DRIVER);
	
	string drivername="tiff";
	if(drivertype_ID==DRIVER_EXR){
		drivername="exr";
	}

	if(drivertype_ID==DRIVER_TIFF){
		if(datatype==DEPTH_8){
			scalarformat="uint8";
		}
		else if(datatype==DEPTH_16){
			scalarformat="uint16";
		}
		else if(datatype==DEPTH_32){
			scalarformat="float";
		}
	}
	else if(drivertype_ID==DRIVER_EXR){
		if(datatype==DEPTH_16){
			scalarformat="half";
		}
		else if(datatype==DEPTH_32){
			scalarformat="float";
		}
	}

	ctx.SetAttribute(layer_handle,(
		NSI::StringArg("variablename", "Ci"), 
		NSI::StringArg("layername", "primary"),
		NSI::StringArg("layertype", "color"),
		NSI::StringArg("variablesource", "shader"),
		NSI::StringArg("scalarformat", scalarformat),
		NSI::IntegerArg("withalpha",1),
		NSI::StringArg("filter", "gaussian")
	));

//	driver_handle=string(parser->GetUniqueName("outputdriver"));
	driver_handle = string(Handle) + string("outputdriver");
	String file=data->GetString(FILENAME,"frame"_s);
	Filename filepath=data->GetFilename(PATH,"");
	filepath=filepath+Filename("/"+file);

	string image_filename=StringToStdString(filepath.GetString());

	ctx.Create(driver_handle, "outputdriver");
	ctx.SetAttribute(driver_handle, (
		NSI::StringArg("drivername", drivername),
		NSI::StringArg("imagefilename", image_filename)
		));


	ctx.Connect(driver_handle, "",layer_handle, "outputdrivers");



}

void DisplayTranslator::ConnectNSINodes(const char* Handle, GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser){
	if (parser->GetRenderMode() != FINAL_RENDER){ //Don't output AOVs for preview render
		return;
	}

	NSI::Context ctx(parser->GetContext());
	ctx.Connect(layer_handle, "", "scene_camera_screen", "outputlayers");


	//Light sets
	BaseObject* obj=(BaseObject*)C4DNode;
	BaseContainer* data=obj->GetDataInstance();
	InExcludeData*  light_list=NULL;
	light_list = (InExcludeData*)data->GetCustomDataType(LIGHTS,CUSTOMDATATYPE_INEXCLUDE_LIST);
	BaseList2D* light=NULL;
	if(light_list){
		
		long nlights=light_list->GetObjectCount();
		for(long i=0; i<nlights; i++){
			light=NULL;
			light=light_list->ObjectFromIndex(doc,i);
			if(light){
				//Connect light to display
				//string light_handle=string(parser->GetAssociatedHandle(light));
				//ctx.Connect(light_handle,"",layer_handle,"lightset");
				
			}
		}
	}
}