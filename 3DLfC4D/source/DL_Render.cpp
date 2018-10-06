#include "SceneParser.h"
#include "DL_TypeConversions.h"
#include "DL_render.h"
#include "IDs.h"
#include "vprendersettings.h"
#include <vector>
using namespace std;

bool DL_RenderFrame(BaseDocument* doc, long frame, RENDER_MODE mode, bool progressive){


	RenderData* rd=doc->GetActiveRenderData();
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
		GePrint("Could not find render settings");
		return false;
	}

	BaseDocument* renderdoc = (BaseDocument*)doc->GetClone(COPYFLAGS_DOCUMENT,nullptr);
	rd = renderdoc->GetActiveRenderData();
	vp = rd->GetFirstVideoPost();

	BaseContainer render_data = rd->GetData();
	BaseContainer vp_data = vp->GetData();
	SceneParser sp;

	sp.SetRenderMode(mode);

	//Motion sampling
	bool useMotionBlur=vp_data.GetBool(DL_USE_MOTION_BLUR);
	long motionSamples=vp_data.GetInt32(DL_MOTION_SAMPLES,2);
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
	return RenderOK; 
}

