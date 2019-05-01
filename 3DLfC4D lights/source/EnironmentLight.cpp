#include "c4d.h"
#include "IDs.h"
#include "oenvironmentlight.h"

class EnvironmentLight : public ObjectData{
public:
	static NodeData *Alloc(void) { return NewObj(EnvironmentLight) iferr_ignore("Instance not created"); }
	virtual Bool Init(GeListNode* node);
	//virtual Bool Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	//virtual BaseObject* GetVirtualObjects(PluginObject *op, HierarchyHelp *hh);
};

Bool EnvironmentLight::Init(GeListNode* node){
	BaseObject		*op   = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();

	data->SetFloat(ENVIRONMENT_INTENSITY,1.0);
	data->SetFloat(ENVIRONMENT_EXPOSURE,1.0);
	data->SetVector(ENVIRONMENT_TINT,Vector(1,1,1));
	data->SetInt32(ENVIRONMENT_MAPPING,ENVIRONMENT_MAPPING_SPHERICAL);
	data->SetBool(ENVIRONMENT_SEEN_BY_CAMERA,false);
	data->SetBool(ENVIRONMENT_PRELIT,false);
	data->SetBool(ENVIRONMENT_ILLUMINATES_BY_DEFAULT,true);
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
	return RegisterObjectPlugin(ID_ENVIRONMENTLIGHT,"DL_Environment"_s,OBJECT_GENERATOR,EnvironmentLight::Alloc,"Oenvironmentlight"_s,AutoBitmap("Envlight.tif"_s),0);
}
