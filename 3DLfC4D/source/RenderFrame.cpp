#include "c4d.h"

#include "IDs.h"
#include "DL_Render.h"



class RenderFrame : public CommandData
{

public:
	virtual Bool Execute(BaseDocument* doc);
};


Bool RenderFrame::Execute(BaseDocument* doc){

	BaseTime t= doc->GetTime();
	long frame=t.GetFrame(doc->GetFps());
	DL_RenderFrame(doc, frame, PREVIEW_RENDER, true );

	//Restore document time
	doc->SetTime(t);
	EventAdd();
	return true;
}


Bool RegisterRenderFrame(void){
	return RegisterCommandPlugin(ID_RENDERFRAME, "Render frame", 0, AutoBitmap("camera.tif"), String(), NewObjClear(RenderFrame));
}