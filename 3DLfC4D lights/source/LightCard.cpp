#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "olightcard.h"



class LightCard : public ObjectData{
public:
	static NodeData *Alloc(void) { return NewObjClear(LightCard); }
	virtual Bool Init(GeListNode* node);
	virtual DRAWRESULT Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	virtual Int32 GetHandleCount(BaseObject* op);

	virtual void GetHandle(BaseObject* op, Int32 i, HandleInfo& info);
	virtual void SetHandle(BaseObject* op, Int32 i, Vector p, const HandleInfo& info);
	//virtual BaseObject* GetVirtualObjects(PluginObject *op, HierarchyHelp *hh);
};

Bool LightCard::Init(GeListNode* node){
	BaseObject		*op   = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();


	data->SetFloat(LIGHTCARD_WIDTH,200.0);
	data->SetFloat(LIGHTCARD_HEIGHT,200.0);

	data->SetVector(LIGHTCARD_COLOR,Vector(1,1,1));
	data->SetFloat(LIGHTCARD_INTENSITY,1.0);
	data->SetFloat(LIGHTCARD_EXPOSURE,0.0);
	data->SetFloat(LIGHTCARD_SPREAD,1.0);
	//data->SetInt32(LIGHTCARD_SAMPLES,32);
	return true;
}


DRAWRESULT LightCard::Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh){
	if (drawpass==DRAWPASS::OBJECT)
	{ 
		BaseContainer *data = op->GetDataInstance(); 
		float width=data->GetFloat(LIGHTCARD_WIDTH);
		float height=data->GetFloat(LIGHTCARD_HEIGHT);

		Vector color=data->GetVector(LIGHTCARD_COLOR);

		Matrix m=bh->GetMg();
		bd->SetMatrix_Matrix(NULL, m);

		bd->SetPen(color);
		//bd->LineNew(Vector(-width/2.0,-height/2.0,0),Vector(-width/2.0,height/2.0,0),0);
		//bd->LineNew(Vector(-width/2.0,height/2.0,0),Vector(width/2.0,height/2.0,0),0);
		//bd->LineNew(Vector(width/2.0,height/2.0,0),Vector(width/2.0,-height/2.0,0),0);
		//bd->LineNew(Vector(width/2.0,-height/2.0,0),Vector(-width/2.0,-height/2.0,0),0);
		
		//Outline
		bd->DrawLine(Vector(-width/2.0,-height/2.0,0),Vector(-width/2.0,height/2.0,0),0);
		bd->DrawLine(Vector(-width/2.0,height/2.0,0),Vector(width/2.0,height/2.0,0),0);
		bd->DrawLine(Vector(width/2.0,height/2.0,0),Vector(width/2.0,-height/2.0,0),0);
		bd->DrawLine(Vector(width/2.0,-height/2.0,0),Vector(-width/2.0,-height/2.0,0),0);

		//double halfw=width/2.0;
		//double halfh=height/2.0;
		
		//Cross
		bd->DrawLine(Vector(-width/2.0,-height/2.0,0),Vector(width/2.0,height/2.0,0),0);
		bd->DrawLine(Vector(-width/2.0,height/2.0,0),Vector(width/2.0,-height/2.0,0),0);

		//Direction
		bd->DrawLine(Vector(0,0,0),Vector(0,0,width/2.0),0);


		/*Vector p[4];
		Vector f[4];

		f[0]=color;
		f[1]=color;
		f[2]=color;
		f[3]=color;

		p[0]=Vector(-width/2.0,-height/2.0,0);
		p[1]=Vector(-width/2.0,height/2.0,0);
		p[2]=Vector(width/2.0,height/2.0,0);
		p[3]=Vector(width/2.0,-height/2.0,0);*/

		//bd->SetTransparency(128);
		//bd->PolygonNew(p, f, true);
	}
	
	return ObjectData::Draw(op, drawpass, bd, bh);
}

Int32 LightCard::GetHandleCount(BaseObject* op){
	return 4;
}

void LightCard::GetHandle(BaseObject* op, Int32 i, HandleInfo& info){
	BaseContainer *data = op->GetDataInstance(); 
	float width=data->GetFloat(LIGHTCARD_WIDTH);
	float height=data->GetFloat(LIGHTCARD_HEIGHT);
	
	if(i==0){
		info.position = Vector(-width/2.0,0,0);
		info.direction = Vector(-1.0,0,0);
		info.type=HANDLECONSTRAINTTYPE::LINEAR;
		//return (Vector(-width/2.0,0,0));
	}
	else if(i==1){
		info.position = Vector(width/2.0,0,0);
		info.direction = Vector(1.0,0,0);
		info.type=HANDLECONSTRAINTTYPE::LINEAR;
		//return (Vector(width/2.0,0,0));
	}
	else if(i==2){
		info.position = Vector(0,-height/2.0,0);
		info.direction = Vector(0,-1.0,0);
		info.type=HANDLECONSTRAINTTYPE::LINEAR;
		//return (Vector(0,-height/2.0,0));
	}
	else if(i==3){
		info.position = Vector(0,height/2.0,0);
		info.direction = Vector(0,1.0,0);
		info.type=HANDLECONSTRAINTTYPE::LINEAR;
		//return (Vector(0,height/2.0,0));
	}
}

void LightCard::SetHandle(BaseObject* op, Int32 i, Vector p,const HandleInfo& info){
	BaseContainer *data = op->GetDataInstance(); 
	//float width=data->GetFloat(LIGHTCARD_WIDTH);
	//float height=data->GetFloat(LIGHTCARD_HEIGHT);

	if(i==0 || i==1){
		data->SetFloat(LIGHTCARD_WIDTH, abs(p.x)*2);
	}
	else{
		data->SetFloat(LIGHTCARD_HEIGHT, abs(p.y)*2);
	}
}

/*BaseObject *LightCard::GetVirtualObjects(PluginObject *op, HierarchyHelp *hh)
{
	return NULL;
}*/

Bool RegisterLightCard(void)
{
	return RegisterObjectPlugin(ID_LIGHTCARD,"DL_Arealight"_s,OBJECT_GENERATOR,LightCard::Alloc,"Olightcard"_s,AutoBitmap("Lightcard.tif"_s),0);
}
