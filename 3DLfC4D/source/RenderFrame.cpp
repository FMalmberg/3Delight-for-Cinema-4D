#include "c4d.h"

#include "IDs.h"
#include "DL_Render.h"
#include "myres.h"


class RenderFrame : public CommandData
{

public:
	virtual Bool Execute(BaseDocument* doc);
};



Bool RenderFrame::Execute(BaseDocument* doc)
{

	BaseTime t= doc->GetTime();
	long frame=t.GetFrame(doc->GetFps());
	DL_RenderFrame(doc, frame, PREVIEW_RENDER, true );



//	Int32 num = dldata->GetInt32(DL_SHADING_SAMPLES);
//	ApplicationOutput(String::IntToString(num));
//RenderData* data = doc->GetFirstRenderData();
//ApplicationOutput(data->GetName());
	

	//Restore document time
	//doc->SetTime(t);
	//EventAdd();
	return true;
}


Bool RegisterRenderFrame(void)
{
	return RegisterCommandPlugin(ID_RENDERFRAME, "Render frameXXX"_s, 0, AutoBitmap("camera.tif"_s), String(), 
		NewObjClear(RenderFrame));
}
