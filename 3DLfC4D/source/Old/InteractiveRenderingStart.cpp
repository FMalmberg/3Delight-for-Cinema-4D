#include "c4d.h"

#include "IDs.h"

class InteractiveRenderingStart : public CommandData
{

public:
	InteractiveRenderingStart();
	virtual Bool Execute(BaseDocument* doc);
	virtual Bool Message	(	LONG 	type, void * 	data );	

private:
	bool active;
	long counter;
};

InteractiveRenderingStart::InteractiveRenderingStart(){
	active=false;
	counter=0;
}

Bool InteractiveRenderingStart::Execute(BaseDocument* doc){
	active=true;
	return true;
}

Bool InteractiveRenderingStart::Message	(	LONG 	type, void * 	data ){
	GePrint("Message");
	return true;
}

Bool RegisterInteractiveRenderingStart(void){
	return RegisterCommandPlugin(ID_INTERACTIVE_RENDERING_START, "Start interactive rendering", 0, AutoBitmap("camera.tif"), String(), gNew InteractiveRenderingStart);
}