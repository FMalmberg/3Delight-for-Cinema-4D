#include "SceneParser.h"
#include "DL_TypeConversions.h"
#include "DL_render.h"
#include "IDs.h"
#include "myres.h"
#include <vector>
#include "nsi.hpp"
using namespace std;

void FrameStoppedCallback(
	void* stoppedcallbackdata,
	NSIContext_t ctx,
	int status) {

	//End context when rendering is completed (or stopped)
	NSIEnd(ctx);
}


bool DL_RenderFrame(BaseDocument* doc, long frame, RENDER_MODE mode, bool progressive){
	BaseDocument* renderdoc = (BaseDocument*)doc->GetClone(COPYFLAGS::DOCUMENT, nullptr);

	NSIContext_t context_handle = NSIBegin(0, 0);

	NSI::Context context(context_handle);

	SceneParser sp(renderdoc, context_handle);

	sp.SetRenderMode(mode);

	//Render scene
	bool RenderOK = sp.InitScene(true, frame);
	sp.SampleFrameMotion();

	BaseDocument::Free(renderdoc);

	context.SetAttribute(NSI_SCENE_GLOBAL, (
		NSI::IntegerArg("renderatlowpriority", 1)
		));

	context.RenderControl((
		NSI::StringArg("action", "start"),
		NSI::IntegerArg("progressive", 0),
		NSI::PointerArg("stoppedcallback", (void*)&FrameStoppedCallback),
		NSI::PointerArg("stoppedcallbackdata", 0)
		));


	return RenderOK;

	/*RenderData* rd=doc->GetActiveRenderData();
	BaseVideoPost* vp=rd->GetFirstVideoPost();

	bool found=false;

	while(vp!=NULL && !found){
		found=(vp->GetType()==ID_RENDERSETTINGS); //Look for rendersettings
		if(!found){
			vp=vp->GetNext();
		}
	}

	if(!found){
		BaseVideoPost* pvp=BaseVideoPost::Alloc(ID_RENDERSETTINGS);
		rd->InsertVideoPostLast(pvp);

		vp=rd->GetFirstVideoPost();
		while(vp!=NULL && !found){
			found=(vp->GetType()==ID_RENDERSETTINGS); //Look for rendersettings
			if(!found){
				vp=vp->GetNext();
			}
		}

	}

	if(!found){
		GePrint("Could not find render settings"_s);
		return false;
	}

	BaseDocument* renderdoc = (BaseDocument*)doc->GetClone(COPYFLAGS::DOCUMENT,nullptr);
	rd = renderdoc->GetActiveRenderData();
	vp = rd->GetFirstVideoPost();

	BaseContainer render_data = rd->GetData();
	BaseContainer vp_data = vp->GetData();
	SceneParser sp;

	sp.SetRenderMode(mode);

	//Motion sampling
	long motionSamples = 2;
	bool useMotionBlur=vp_data.GetBool(DL_MOTION_BLUR);
	//long motionSamples=vp_data.GetInt32(DL_MOTION_SAMPLES,2);
	if(!useMotionBlur){ motionSamples=1; }

	float ShutterAngle=vp_data.GetFloat(DL_SHUTTER_ANGLE);
	long fps=doc->GetFps();
	float shutterOpen=float(frame)/float(fps);
	float shutterClose=float(frame+ShutterAngle)/float(fps);

	sp.SetMotionSamples(motionSamples);
	sp.SetShutter(shutterOpen,shutterClose);

	//Render scene
	bool RenderOK=	sp.Parse(renderdoc, frame);

	BaseDocument::Free(renderdoc);  
	return RenderOK; */
}