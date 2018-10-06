#include "c4d.h"
#include "IDs.h"
#include "obasicdisplacement.h"

class BasicDisplacement : public ObjectData{
public:
	static NodeData *Alloc(void) { return NewObjClear(BasicDisplacement); }
	virtual Bool Init(GeListNode* node);
	//virtual DRAWRESULT Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	//virtual LONG GetHandleCount(BaseObject* op);

	//virtual void GetHandle(BaseObject* op, LONG i, HandleInfo& info);
	//virtual void SetHandle(BaseObject* op, LONG i, Vector p, const HandleInfo& info);
	//virtual BaseObject* GetVirtualObjects(PluginObject *op, HierarchyHelp *hh);
};

Bool BasicDisplacement::Init(GeListNode* node){
	BaseObject		*op   = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();

	data->SetFloat(DISPLACEMENT_HEIGHT, 0.01);
	data->SetFloat(DISPLACEMENT_STRENGTH, 1.0);
	data->SetBool(DISPLACEMENT_CENTERED,true);

	return true;
}





Bool RegisterBasicDisplacement(void)
{
	return RegisterObjectPlugin(ID_BASICDISPLACEMENT,"DL_NormalDisplacement",OBJECT_GENERATOR,BasicDisplacement::Alloc,"Obasicdisplacement",AutoBitmap("Standardmaterial.tif"),0);
}
