#include "c4d.h"
#include "IDs.h"
#include "ostandardmaterial.h"

class StandardMaterial : public ObjectData{
public:
	static NodeData *Alloc(void) { return NewObjClear(StandardMaterial); }
	virtual Bool Init(GeListNode* node);
	//virtual DRAWRESULT Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	//virtual LONG GetHandleCount(BaseObject* op);

	//virtual void GetHandle(BaseObject* op, LONG i, HandleInfo& info);
	//virtual void SetHandle(BaseObject* op, LONG i, Vector p, const HandleInfo& info);
	//virtual BaseObject* GetVirtualObjects(PluginObject *op, HierarchyHelp *hh);
};

Bool StandardMaterial::Init(GeListNode* node){
	BaseObject		*op   = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();

	//Coating default values
	data->SetBool(COATING_ON, false);
	data->SetVector(COATING_COLOR, Vector(1,1,1));
	data->SetVector(COATING_TRANSMITTANCE, Vector(1,1,1));
	data->SetFloat(COATING_THICKNESS, 0.01);
	data->SetFloat(COATING_ROUGHNESS, 0.05);
	data->SetFloat(COATING_FR, 0.05);

	//Diffuse default values
	data->SetFloat(DIFFUSE_WEIGHT, 0.7);
	data->SetVector(DIFFUSE_COLOR, Vector(1,1,1));
	data->SetFloat(DIFFUSE_ROUGHNESS, 0.0);

	//Specular default values
	data->SetFloat(SPECULAR_WEIGHT, 0.3);
	data->SetVector(SPECULAR_COLOR, Vector(1,1,1));
	data->SetFloat(SPECULAR_ROUGHNESS, 0.05);
	data->SetFloat(SPECULAR_FR, 0.05);
	data->SetVector(SPECULAR_ANISOTROPY_DIRECTION, Vector(1, 0, 0));


	//Emission default values
	data->SetFloat(EMISSION_WEIGHT, 1.0);
	data->SetVector(EMISSION_COLOR, Vector(1,1,1));

	//Refraction default values
	data->SetBool(REFRACTION_ON,false);
	data->SetFloat(REFRACTION_WEIGHT, 1.0);
	data->SetVector(REFRACTION_COLOR, Vector(1,1,1));
	data->SetFloat(REFRACTION_ROUGHNESS, 0.05);
	data->SetFloat(REFRACTION_IOR, 1.5);

	/*

	SPECULAR_GROUP			=3000,
	SPECULAR_WEIGHT			=3001,
	SPECULAR_COLOR			=3002,
	SPECULAR_COLOR_SHADER		=3003,
	SPECULAR_ROUGHNESS		=3004,
	SPECULAR_ROUGHNESS_SHADER	=3005,
	SPECULAR_FR	*/

	/*data->SetFloat(LIGHTCARD_WIDTH,200.0);
	data->SetFloat(LIGHTCARD_HEIGHT,200.0);

	data->SetVector(LIGHTCARD_COLOR,Vector(1,1,1));
	data->SetFloat(LIGHTCARD_INTENSITY,1.0);
	
	data->SetLong(LIGHTCARD_SAMPLES,32);*/
	return true;
}





Bool RegisterStandardMaterial(void)
{
	return RegisterObjectPlugin(ID_STANDARDMATERIAL,"DL_StandardMaterial",OBJECT_GENERATOR,StandardMaterial::Alloc,"Ostandardmaterial",AutoBitmap("Standardmaterial.tif"),0);
}
