#include "CameraHook.h"
#include "myres.h"
//#include "ri.h"
#pragma warning(disable : 4265)
#include "nsi.hpp"
#include "DL_TypeConversions.h"
using namespace std;

void CameraHook::Init(BaseDocument* doc, DL_SceneParser* parser) {
	transform_name = string("3dlfc4d::scene_camera_transform"); //string(parser->GetUniqueName("transform"));
	camera_name = string("3dlfc4d::scene_camera");
}

void CameraHook::CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser){

	NSI::Context ctx(parser->GetContext());

	ctx.Create(camera_name, "perspectivecamera");
	//transform_name = string("3dlfc4d::scene_camera_transform"); //string(parser->GetUniqueName("transform"));
	ctx.Create(transform_name, "transform");
	ctx.Create("3dlfc4d::scene_camera_screen", "screen");

	ctx.Connect(camera_name, "", transform_name, "objects");
	ctx.Connect(transform_name, "", ".root", "objects");
	ctx.Connect("3dlfc4d::scene_camera_screen", "", "3dlfc4d::scene_camera", "screens");

	RenderData* rd=doc->GetActiveRenderData();
	BaseContainer* render_data=rd->GetDataInstance();

	BaseContainer* DL_Settings=parser->GetSettings();

	int enable_dof = 1;
	double fstop = 2;

	vector<int> resolution(2);
	resolution[0]=render_data->GetInt32(RDATA_XRES);
	resolution[1]=render_data->GetInt32(RDATA_YRES);

	NSI::Argument resolution_arg("resolution");
	resolution_arg.SetArrayType(NSITypeInteger,2);
	resolution_arg.SetValuePointer((void*)&resolution[0]);

	double f=double(resolution[1])/double(resolution[0]);
	double screen_window[4];
	screen_window[0]=-1;
	screen_window[1]=-f;
	screen_window[2]=1;
	screen_window[3]=f;
	NSI::Argument screenwindow_arg("screenwindow");
	screenwindow_arg.SetArrayType(NSITypeDouble,2);
	screenwindow_arg.SetCount(2);
	screenwindow_arg.SetValuePointer((void*)&screen_window[0]);

	

	double shutter_opening[2];
	shutter_opening[0]=0.5*(1.0-DL_Settings->GetFloat(DL_SHUTTER_OPENING_EFFICIENCY,1));
	shutter_opening[1]=1-0.5*(1.0-DL_Settings->GetFloat(DL_SHUTTER_CLOSING_EFFICIENCY,1));
	NSI::Argument shutteropening_arg("shutteropening");
	shutteropening_arg.SetType(NSITypeDouble);
	shutteropening_arg.SetCount(2);
	shutteropening_arg.SetValuePointer((void*)&shutter_opening[0]);

	//Get scene camera
	BaseDraw* bd=doc->GetActiveBaseDraw();
	CameraObject* camera=(CameraObject*)bd->GetSceneCamera(doc);

	int pixel_samples=DL_Settings->GetInt32(DL_PIXEL_SAMPLES,16);
	

	ctx.SetAttribute(camera_name,(
			shutteropening_arg,
			NSI::IntegerArg("depthoffield.enable",enable_dof),
			NSI::DoubleArg("depthoffield.fstop",fstop)
		));

	ctx.SetAttribute("3dlfc4d::scene_camera_screen", (
		resolution_arg,
		screenwindow_arg,
		NSI::IntegerArg("oversampling", pixel_samples)
		));

}

void CameraHook::SampleAttributes(DL_SampleInfo* info,  BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	if (info->sample==0) { //Set camera shutter range at first motion sample
		double shutter_range[2];
		shutter_range[0] = info->shutter_open_time;
		shutter_range[1] = info->shutter_close_time;
		NSI::Argument shutter_arg("shutterrange");
		shutter_arg.SetType(NSITypeDouble);
		shutter_arg.SetCount(2);
		shutter_arg.SetValuePointer((void*)&shutter_range[0]);

		ctx.SetAttribute("3dlfc4d::scene_camera", (
			shutter_arg
			));
	}
	
	//Get scene camera
	BaseDraw* bd = doc->GetActiveBaseDraw();
	CameraObject* camera = (CameraObject*)bd->GetSceneCamera(doc);

	Float aperture=camera->GetAperture();
	Float focus=camera->GetFocus();
	float horizontal_fov=RadToDeg(2*ATan(0.5*aperture/focus));

	float focal_length_cm = focus / 10;
	


	BaseContainer* camdata = camera->GetDataInstance();
	double focus_distance = camdata->GetFloat(CAMERAOBJECT_TARGETDISTANCE);
	
	//Sample camera transform
	Matrix m=camera->GetMg();
	
	Matrix flip; 
	flip.sqmat.v1=Vector(1,0,0);
	flip.sqmat.v2=Vector(0,1,0);
	flip.sqmat.v3=Vector(0,0,-1);
	m=(m*flip);

	vector<double> v=MatrixToNSIMatrix(m);
	
	NSI::Argument xform("transformationmatrix");
	xform.SetType(NSITypeDoubleMatrix);
	xform.SetValuePointer((void*)&v[0]);

	ctx.SetAttributeAtTime(transform_name, info->sample_time, (
			xform
		));

	
	//Sample fov
	ctx.SetAttributeAtTime("3dlfc4d::scene_camera",info->sample_time,(
			NSI::FloatArg("fov",horizontal_fov),
			NSI::FloatArg("focallength", focal_length_cm),
			NSI::DoubleArg("depthoffield.focaldistance", focus_distance)
		));

}