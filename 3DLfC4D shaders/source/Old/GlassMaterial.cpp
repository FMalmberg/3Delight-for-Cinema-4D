#include "c4d.h"
#include "IDs.h"
#include "oglassmaterial.h"

class GlassMaterial : public ObjectData{
public:
	static NodeData *Alloc(void) { return NewObjClear(GlassMaterial); }
	virtual Bool Init(GeListNode* node);
	//virtual DRAWRESULT Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	//virtual LONG GetHandleCount(BaseObject* op);

	//virtual void GetHandle(BaseObject* op, LONG i, HandleInfo& info);
	//virtual void SetHandle(BaseObject* op, LONG i, Vector p, const HandleInfo& info);
	//virtual BaseObject* GetVirtualObjects(PluginObject *op, HierarchyHelp *hh);
};

Bool GlassMaterial::Init(GeListNode* node){
	BaseObject		*op   = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();

	//Coating default values
	data->SetVector(REFLECTION_COLOR,Vector(1,1,1));
	data->SetVector(REFRACTION_COLOR,Vector(1,1,1));
	data->SetVector(TINT,Vector(1,1,1));
	data->SetFloat(ROUGHNESS, 0.05);
	data->SetFloat(IOR, 1.5);

	/*data->SetVector(COATING_COLOR, Vector(1,1,1));
	data->SetVector(COATING_TRANSMITTANCE, Vector(1,1,1));
	data->SetFloat(COATING_THICKNESS, 0.01);
	data->SetFloat(COATING_ROUGHNESS, 0.05);
	data->SetFloat(COATING_FR, 0.05);*/

	
	return true;
}

Bool RegisterGlassMaterial(void)
{
	return RegisterObjectPlugin(ID_GLASSMATERIAL,"DL_GlassMaterial",OBJECT_GENERATOR,GlassMaterial::Alloc,"Oglassmaterial",AutoBitmap("Glass.tif"),0);
}
