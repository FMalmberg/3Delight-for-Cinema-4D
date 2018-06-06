#include "c4d.h"
#include "IDs.h"
#include "odisplay.h"

class Display : public ObjectData{
public:
	static NodeData *Alloc(void) { return NewObjClear(Display); }
	virtual Bool Init(GeListNode* node);
	//virtual DRAWRESULT Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	//virtual LONG GetHandleCount(BaseObject* op);

	//virtual void GetHandle(BaseObject* op, LONG i, HandleInfo& info);
	//virtual void SetHandle(BaseObject* op, LONG i, Vector p, const HandleInfo& info);
	//virtual BaseObject* GetVirtualObjects(PluginObject *op, HierarchyHelp *hh);
};

Bool Display::Init(GeListNode* node){
	BaseObject		*op   = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();
	
	data->SetString(FILENAME,"frame#5f.#d");
	data->SetInt32(DRIVER, DRIVER_TIFF);

	data->SetInt32(DEPTH,DEPTH_16);

	return true;
}





Bool RegisterDisplay(void)
{
	return RegisterObjectPlugin(ID_DISPLAY,"DL_Output",OBJECT_GENERATOR,Display::Alloc,"Odisplay",AutoBitmap("Display.tif"),0);
}
