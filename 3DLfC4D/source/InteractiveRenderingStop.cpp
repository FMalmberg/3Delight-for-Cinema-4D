#include "c4d.h"

#include "IDs.h"

extern bool interactive_rendering_active;

class InteractiveRenderingStop : public CommandData
{

public:
	virtual Bool Execute(BaseDocument* doc);

};

Bool InteractiveRenderingStop::Execute(BaseDocument* doc){
	SpecialEventAdd(DL_STOP_INTERACTIVE_RENDERING);
	return true;
}

Bool RegisterInteractiveRenderingStop(void){
	return RegisterCommandPlugin(ID_INTERACTIVE_RENDERING_STOP, "Stop interactive rendering"_s, 0, AutoBitmap("interactiveStop.tif"_s), String(), NewObjClear(InteractiveRenderingStop));
}