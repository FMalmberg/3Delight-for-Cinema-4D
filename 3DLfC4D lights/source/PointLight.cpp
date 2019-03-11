#include "c4d.h"
#include "IDs.h"
#include "opointlight.h"

class PointLight : public ObjectData{
public:
	static NodeData *Alloc(void) { return NewObjClear(PointLight); }
	virtual Bool Init(GeListNode* node);
	virtual DRAWRESULT Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	
	virtual Int32 GetHandleCount(BaseObject* op);
	virtual void GetHandle(BaseObject* op, Int32 i, HandleInfo& info);
	virtual void SetHandle(BaseObject* op, Int32 i, Vector p, const HandleInfo& info);
	
};

Bool PointLight::Init(GeListNode* node){
	BaseObject		*op   = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();

	data->SetFloat(POINTLIGHT_RADIUS,50.0);
	data->SetVector(POINTLIGHT_COLOR, Vector(1, 1, 1));
	data->SetFloat(POINTLIGHT_INTENSITY, 1.0);

	return true;
}


DRAWRESULT PointLight::Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh){
	if (drawpass==DRAWPASS::OBJECT)
	{ 
		BaseContainer *data = op->GetDataInstance();
		float radius = data->GetFloat(POINTLIGHT_RADIUS);
		Vector color = data->GetVector(POINTLIGHT_COLOR);
		bd->SetPen(color);

		Matrix m = bh->GetMg();
		bd->SetMatrix_Matrix(NULL, m);

		Matrix m_circle;
		m_circle.off = Vector64(0, 0, 0);

		m_circle.sqmat.v1 = Vector64(radius, 0, 0);
		m_circle.sqmat.v2 = Vector64(0, radius, 0);
		bd->DrawCircle(m_circle);

		m_circle.sqmat.v1 = Vector64(radius, 0, 0);
		m_circle.sqmat.v2 = Vector64(0, 0, radius);
		bd->DrawCircle(m_circle);

		m_circle.sqmat.v1 = Vector64(0, radius, 0);
		m_circle.sqmat.v2 = Vector64(0, 0, radius);
		bd->DrawCircle(m_circle);
	}
	
	return ObjectData::Draw(op, drawpass, bd, bh);
}


Int32 PointLight::GetHandleCount(BaseObject* op){
	return 1;
}


void PointLight::GetHandle(BaseObject* op, Int32 i, HandleInfo& info){
	BaseContainer *data = op->GetDataInstance(); 
	float radius=data->GetFloat(POINTLIGHT_RADIUS);
	
	if(i==0){
		info.position = Vector(radius,0,0);
		info.direction = Vector(1.0,0,0);
		info.type=HANDLECONSTRAINTTYPE::LINEAR;
	}
}


void PointLight::SetHandle(BaseObject* op, Int32 i, Vector p,const HandleInfo& info){
	BaseContainer *data = op->GetDataInstance(); 

	if(i==0 ){
		data->SetFloat(POINTLIGHT_RADIUS, abs(p.x));
	}

}

Bool RegisterPointLight(void)
{
	return RegisterObjectPlugin(ID_POINTLIGHT,"DL_Pointlight"_s,OBJECT_GENERATOR,PointLight::Alloc,"Opointlight"_s,AutoBitmap("Lightcard.tif"_s),0);
}
