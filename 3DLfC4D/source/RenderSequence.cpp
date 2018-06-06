#include "c4d.h"

#include "IDs.h"
#include "DL_Render.h"



class RenderSequence : public CommandData
{

public:
	virtual Bool Execute(BaseDocument* doc);
};


Bool RenderSequence::Execute(BaseDocument* doc){
	BaseTime t= doc->GetTime();
	
	RenderData* rd=doc->GetActiveRenderData();
	BaseContainer render_data = rd->GetData();
	
	long frame_start=0;
	long frame_end=0;


	long FrameSequence=render_data.GetInt32(RDATA_FRAMESEQUENCE);
	
	if(FrameSequence==RDATA_FRAMESEQUENCE_MANUAL){
		BaseTime t1=render_data.GetTime(RDATA_FRAMEFROM);
		BaseTime t2=render_data.GetTime(RDATA_FRAMETO);
		frame_start=t1.GetFrame(doc->GetFps());
		frame_end=t2.GetFrame(doc->GetFps());
	}
	else if(FrameSequence==RDATA_FRAMESEQUENCE_CURRENTFRAME){
		frame_start=frame_end=t.GetFrame(doc->GetFps());
	}
	else if(FrameSequence==RDATA_FRAMESEQUENCE_ALLFRAMES){
		BaseTime t1=doc->GetMinTime();
		BaseTime t2=doc->GetMaxTime();
		frame_start=t1.GetFrame(doc->GetFps());
		frame_end=t2.GetFrame(doc->GetFps());
	}
	else if(FrameSequence==RDATA_FRAMESEQUENCE_PREVIEWRANGE){
		BaseTime t1=doc->GetLoopMinTime();
		BaseTime t2=doc->GetLoopMaxTime();
		frame_start=t1.GetFrame(doc->GetFps());
		frame_end=t2.GetFrame(doc->GetFps());
	}


	long framestep=render_data.GetInt32(RDATA_FRAMESTEP,1);

	bool renderOK=true;
	for(long i=frame_start; i<=frame_end && renderOK; i=i+framestep){
		renderOK=DL_RenderFrame(doc, i ,FINAL_RENDER, false);
	}

	//Restore document time
	doc->SetTime(t);
	EventAdd();
	return true;
}


Bool RegisterRenderSequence(void){
	return RegisterCommandPlugin(ID_RENDERSEQUENCE, "Render animation", 0, AutoBitmap("filmcamera.tif"), String(), NewObjClear(RenderSequence));
}