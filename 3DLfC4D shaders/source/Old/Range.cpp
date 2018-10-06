#include "c4d.h"
#include "IDs.h"
#include "orange.h"

class Range : public ObjectData{
public:
	static NodeData *Alloc(void) { return NewObjClear(Range); }
	virtual Bool Init(GeListNode* node);
	//virtual DRAWRESULT Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	//virtual LONG GetHandleCount(BaseObject* op);

	//virtual void GetHandle(BaseObject* op, LONG i, HandleInfo& info);
	//virtual void SetHandle(BaseObject* op, LONG i, Vector p, const HandleInfo& info);
	//virtual BaseObject* GetVirtualObjects(PluginObject *op, HierarchyHelp *hh);
};

Bool Range::Init(GeListNode* node){
	BaseObject		*op   = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();

	data->SetFloat(IN_MIN,0);
	data->SetFloat(IN_MAX,1);
	data->SetFloat(OUT_MIN,0);
	data->SetFloat(OUT_MAX,1);

	return true;
}





Bool RegisterRange(void)
{
	return RegisterObjectPlugin(ID_RANGE,"DL_Range",OBJECT_GENERATOR,Range::Alloc,"Orange",AutoBitmap("Range.tif"),0);
}
