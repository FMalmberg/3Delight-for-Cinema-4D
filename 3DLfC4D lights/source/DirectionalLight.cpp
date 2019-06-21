#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "odirectionallight.h"



class DirectionalLight : public ObjectData {
public:
	static NodeData *Alloc(void) { return NewObjClear(DirectionalLight); }
	virtual Bool Init(GeListNode* node);
	virtual DRAWRESULT Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	virtual Int32 GetHandleCount(BaseObject* op);
	virtual void GetHandle(BaseObject* op, Int32 i, HandleInfo& info);
	//virtual void SetHandle(BaseObject* op, Int32 i, Vector p, const HandleInfo& info);
	//virtual BaseObject* GetVirtualObjects(PluginObject *op, HierarchyHelp *hh);
};

Bool DirectionalLight::Init(GeListNode* node) {
	BaseObject		*op = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();

	data->SetVector(DIRECTIONAL_LIGHT_COLOR, Vector(1, 1, 1));
	data->SetFloat(DIRECTIONAL_LIGHT_INTENSITY, 1.0);
	data->SetFloat(DIRECTIONAL_LIGHT_ANGULAR_DIAMETER, 0.0);
	data->SetFloat(DIRECTIONAL_LIGHT_EXPOSURE, 0.0);
	data->SetFloat(DIRECTIONAL_LIGHT_DIFFUSE, 1.0);
	data->SetFloat(DIRECTIONAL_LIGHT_SPECULAR, 1.0);
	data->SetFloat(DIRECTIONAL_LIGHT_HAIR, 1.0);
	data->SetFloat(DIRECTIONAL_LIGHT_VOLUME, 1.0);

	//data->SetInt32(DIRECTIONAL_LIGHT_SAMPLES,32);
	return true;
}


DRAWRESULT DirectionalLight::Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh) {
	if (drawpass == DRAWPASS::OBJECT)
	{
		BaseContainer *data = op->GetDataInstance();
		float Line = 100;
		float arrow = 20;

		Vector color = HSVToRGB(Vector(0, 0, 1));

		Matrix m = bh->GetMg();

		bd->SetMatrix_Matrix(NULL, m);

		bd->SetPen(color);

		//bd->LineNew(Vector(-width/2.0,-height/2.0,0),Vector(-width/2.0,height/2.0,0),0);
		//bd->LineNew(Vector(-width/2.0,height/2.0,0),Vector(width/2.0,height/2.0,0),0);
		//bd->LineNew(Vector(width/2.0,height/2.0,0),Vector(width/2.0,-height/2.0,0),0);
		//bd->LineNew(Vector(width/2.0,-height/2.0,0),Vector(-width/2.0,-height/2.0,0),0);

		//Outline
		bd->DrawLine(Vector(0, 0, -Line), Vector(0, 0, Line), 0);
		bd->DrawLine(Vector(-arrow / 2.0, 0, Line), Vector(0, 0, Line+(2*arrow)), 0);
		bd->DrawLine(Vector(0, 0, Line+(2*arrow)), Vector(arrow / 2.0, 0, Line), 0);
		bd->DrawLine(Vector(-arrow / 2.0, 0, Line), Vector(arrow / 2.0, 0, Line), 0);

		bd->DrawLine(Vector(0, -30, -Line), Vector(0, -30, Line), 0);
		bd->DrawLine(Vector(-arrow/2.0, -30, Line), Vector(0, -30, Line + (2 * arrow)), 0);
		bd->DrawLine(Vector(0, -30, Line + (2 * arrow)), Vector(arrow/2.0, -30, Line), 0);
		bd->DrawLine(Vector(-arrow/2.0, -30, Line), Vector(arrow/2.0, -30, Line), 0);
		
		bd->DrawLine(Vector(30, 10, -Line), Vector(30, 10, Line), 0);
		bd->DrawLine(Vector(27.5, 15, Line), Vector(32.5, 5, Line), 0);
		bd->DrawLine(Vector(32.5, 5, Line), Vector(30, 10, Line + (2 * arrow)), 0);
		bd->DrawLine(Vector(30, 10, Line + (2 * arrow)), Vector(27.5, 15, Line), 0);
		
		bd->DrawLine(Vector(-30, 10, -Line), Vector(-30, 10, Line), 0);
		bd->DrawLine(Vector(-27.5, 15, Line), Vector(-32.5, 5, Line), 0);
		bd->DrawLine(Vector(-32.5, 5, Line), Vector(-30, 10, Line + (2 * arrow)), 0);
		bd->DrawLine(Vector(-30, 10, Line + (2 * arrow)), Vector(-27.5, 15, Line), 0);
		
		

	}

	return ObjectData::Draw(op, drawpass, bd, bh);
}


Int32 DirectionalLight::GetHandleCount(BaseObject* op) {
	return 4;
}

void DirectionalLight::GetHandle(BaseObject* op, Int32 i, HandleInfo& info)
{
	
}



Bool RegisterDirectionalLight(void)
{
	return RegisterObjectPlugin(ID_DIRECTIONAL_LIGHT, "Directional Light"_s, OBJECT_GENERATOR|PLUGINFLAG_HIDEPLUGINMENU, DirectionalLight::Alloc, "odirectionallight"_s, AutoBitmap("kot.png"_s), 0);
}
