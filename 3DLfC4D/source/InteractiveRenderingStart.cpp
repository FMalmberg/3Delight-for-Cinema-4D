#include "c4d.h"

#include "IDs.h"

class InteractiveRenderingStart : public CommandData
{

public:
	virtual Bool Execute(BaseDocument* doc);

};

Bool InteractiveRenderingStart::Execute(BaseDocument* doc){
	SpecialEventAdd(DL_START_INTERACTIVE_RENDERING);
	return true;
}

Bool RegisterInteractiveRenderingStart(void){
	return RegisterCommandPlugin(ID_INTERACTIVE_RENDERING_START, "Start interactive rendering"_s, 0, AutoBitmap("interactiveStart.tif"_s), String(), NewObjClear(InteractiveRenderingStart));
}