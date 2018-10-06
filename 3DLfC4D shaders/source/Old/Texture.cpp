#include "c4d.h"
#include "IDs.h"
#include "otexture.h"

class Texture : public ObjectData{
public:
	static NodeData *Alloc(void) { return NewObjClear(Texture); }
	virtual Bool Init(GeListNode* node);
	//virtual DRAWRESULT Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	//virtual LONG GetHandleCount(BaseObject* op);

	//virtual void GetHandle(BaseObject* op, LONG i, HandleInfo& info);
	//virtual void SetHandle(BaseObject* op, LONG i, Vector p, const HandleInfo& info);
	//virtual BaseObject* GetVirtualObjects(PluginObject *op, HierarchyHelp *hh);
};

Bool Texture::Init(GeListNode* node){
	BaseObject		*op   = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();


	return true;
}





Bool RegisterTexture(void)
{
	return RegisterObjectPlugin(ID_TEXTURE,"DL_Texture",OBJECT_GENERATOR,Texture::Alloc,"Otexture",AutoBitmap("Texture.tif"),0);
}
