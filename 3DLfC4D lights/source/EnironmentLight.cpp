#include "c4d.h"
#include "IDs.h"
#include "oenvironmentlight.h"

class EnvironmentLight : public ObjectData{
public:
	static NodeData *Alloc(void) { return NewObjClear(EnvironmentLight); }
	virtual Bool Init(GeListNode* node);
	//virtual Bool Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	//virtual BaseObject* GetVirtualObjects(PluginObject *op, HierarchyHelp *hh);
};

Bool EnvironmentLight::Init(GeListNode* node){
	BaseObject		*op   = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();

	data->SetFloat(ENVIRONMENT_ANGLE, 360);

	data->SetFloat(ENVIRONMENT_INTENSITY,1.0);
	data->SetFloat(ENVIRONMENT_EXPOSURE,0.0);
	data->SetVector(ENVIRONMENT_TINT,Vector(1,1,1));
	data->SetInt32(ENVIRONMENT_TEXTURE_COLORSPACE, ENVIRONMENT_TEXTURE_COLORSPACE_LINEAR);

	/*data->SetFloat(LIGHTCARD_WIDTH,200.0);
	data->SetFloat(LIGHTCARD_HEIGHT,200.0);

	data->SetVector(LIGHTCARD_COLOR,Vector(1,1,1));
	data->SetFloat(LIGHTCARD_INTENSITY,1.0);
	
	data->SetLong(LIGHTCARD_SAMPLES,32);*/
	return true;
}


/*Bool EnvironmentLight::Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh){
	if (drawpass==DRAWPASS_OBJECT)
	{ 
		BaseContainer *data = op->GetDataInstance(); 
		
		
	}
	
	return ObjectData::Draw(op, drawpass, bd, bh);
}*/





Bool RegisterEnvironmentLight(void)
{
	return RegisterObjectPlugin(ID_ENVIRONMENTLIGHT,"DL_Environment",OBJECT_GENERATOR,EnvironmentLight::Alloc,"Oenvironmentlight",AutoBitmap("Envlight.tif"),0);
}
