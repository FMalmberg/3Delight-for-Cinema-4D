#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "olightcard.h"



/**
	Function to hide a zLength from rectangle and disc shaped arealight (lightcard).
*/
Bool ShowDescriptionElement(GeListNode *i_node, Description *i_descr, Int32 i_MyDescID, Bool i_show)
{
	AutoAlloc<AtomArray> ar;
	ar->Append(static_cast<C4DAtom*>(i_node));
	BaseContainer *bc = i_descr->GetParameterI(DescLevel(i_MyDescID), ar);

	if (!bc)
	{
		return FALSE;
	}

	bc->SetBool(DESC_HIDE, !i_show);

	return TRUE;
}


class LightCard : public ObjectData{
public:
	static NodeData *Alloc(void) { return NewObjClear(LightCard); }
	virtual Bool Init(GeListNode* node);
	virtual Bool GetDDescription(GeListNode *node, Description *description, DESCFLAGS_DESC &flags);
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
	data->SetFloat(LIGHTCARD_ZLENGTH,200.0);

	data->SetVector(LIGHTCARD_COLOR,Vector(1,1,1));
	data->SetFloat(LIGHTCARD_INTENSITY,1.0);
	data->SetFloat(LIGHTCARD_EXPOSURE,0.0);
	data->SetFloat(LIGHTCARD_SPREAD,1.0);
	data->SetFloat(LIGHTCARD_DIFFUSE,1.0);
	data->SetFloat(LIGHTCARD_SPECULAR,1.0);
	data->SetFloat(LIGHTCARD_HAIR,1.0);
	data->SetFloat(LIGHTCARD_VOLUME,1.0);
	data->SetBool(LIGHTCARD_TWO_SIDED,false);
	data->SetInt32(LIGHTCARD_DECAY,LIGHTCARD_DECAY_QUADRATIC);
	data->SetBool(LIGHTCARD_NORMALIZE,false);
	data->SetBool(LIGHTCARD_PRELIT,false);
	data->SetInt32(LIGHTCARD_SHAPE,LIGHTCARD_SHAPE_RECTANGLE);

	//data->SetInt32(LIGHTCARD_SAMPLES,32);
	return true;
}


Bool LightCard::GetDDescription(GeListNode *node, Description *description, DESCFLAGS_DESC &flags)
{
	description->LoadDescription(ID_LIGHTCARD);
	flags |= DESCFLAGS_DESC::LOADED;

	BaseObject		*op = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();

	if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_CYLINDER || data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_SPHERE)
		ShowDescriptionElement(node, description, LIGHTCARD_ZLENGTH, true);
	
	return true;
}


DRAWRESULT LightCard::Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh)
{
	if (drawpass==DRAWPASS::OBJECT)
	{ 
		BaseContainer *data = op->GetDataInstance(); 
		float width = data->GetFloat(LIGHTCARD_WIDTH);
		float height = data->GetFloat(LIGHTCARD_HEIGHT);

		if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_RECTANGLE)
		{

			Vector color = HSVToRGB(Vector(0, 0, 1));

			Matrix m = bh->GetMg();

			bd->SetMatrix_Matrix(NULL, m);

			bd->SetPen(color);

			//Outline
			bd->DrawLine(Vector(-width / 2.0, -height / 2.0, 0), Vector(-width / 2.0, height / 2.0, 0), 0);
			bd->DrawLine(Vector(-width / 2.0, height / 2.0, 0), Vector(width / 2.0, height / 2.0, 0), 0);
			bd->DrawLine(Vector(width / 2.0, height / 2.0, 0), Vector(width / 2.0, -height / 2.0, 0), 0);
			bd->DrawLine(Vector(width / 2.0, -height / 2.0, 0), Vector(-width / 2.0, -height / 2.0, 0), 0);

			//Cross
			//bd->DrawLine(Vector(-width / 2.0, -height / 2.0, 0), Vector(width / 2.0, height / 2.0, 0), 0);
			//bd->DrawLine(Vector(-width / 2.0, height / 2.0, 0), Vector(width / 2.0, -height / 2.0, 0), 0);

			//Direction
			bd->DrawLine(Vector(0, 0, 0), Vector(0, 0, width / 2.0), 0);
		}
		else if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_DISC)
		{
			float xLength = data->GetFloat(LIGHTCARD_WIDTH);
			float yLength = data->GetFloat(LIGHTCARD_HEIGHT);
			Vector color = Vector(1,1,1);
			bd->SetPen(color);

			Matrix m = bh->GetMg();
			bd->SetMatrix_Matrix(NULL, m);

			Matrix m_circle;
			m_circle.off = Vector64(0, 0, 0);

			m_circle.sqmat.v1 = Vector64(xLength/2.0, 0, 0);
			m_circle.sqmat.v2 = Vector64(0, yLength/2.0, 0);
			bd->DrawCircle(m_circle);
		}
		else if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_SPHERE)
		{
			float xLength = data->GetFloat(LIGHTCARD_WIDTH);
			float yLength = data->GetFloat(LIGHTCARD_HEIGHT);
			float zLength = data->GetFloat(LIGHTCARD_ZLENGTH);
			Vector color = Vector(1, 1, 1);
			bd->SetPen(color);

			Matrix m = bh->GetMg();
			bd->SetMatrix_Matrix(NULL, m);

			Matrix m_circle;
			m_circle.off = Vector64(0, 0, 0);

			m_circle.sqmat.v1 = Vector64(xLength/2.0, 0, 0);
			m_circle.sqmat.v2 = Vector64(0, yLength/2.0, 0);
			bd->DrawCircle(m_circle);

			m_circle.sqmat.v1 = Vector64(xLength/2.0, 0, 0);
			m_circle.sqmat.v2 = Vector64(0, 0, zLength/2.0);
			bd->DrawCircle(m_circle);

			m_circle.sqmat.v1 = Vector64(0, yLength/2.0, 0);
			m_circle.sqmat.v2 = Vector64(0, 0, zLength/2.0);
			bd->DrawCircle(m_circle);
		}

		else if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_CYLINDER)
		{
			float xLength = data->GetFloat(LIGHTCARD_WIDTH);
			float yLength = data->GetFloat(LIGHTCARD_HEIGHT);
			float zLength = data->GetFloat(LIGHTCARD_ZLENGTH);

			Vector color = Vector(1, 1, 1);
			bd->SetPen(color);

			Matrix m = bh->GetMg();
			bd->SetMatrix_Matrix(NULL, m);

			bd->DrawLine(Vector(0, -height / 2.0, -zLength/2.0), Vector(0, -height / 2.0, zLength/2.0), 0);
			bd->DrawLine(Vector(0, height / 2.0, -zLength/2.0), Vector(0, height / 2.0, zLength/2.0), 0);
			
			bd->DrawLine(Vector(-width / 2.0, 0, -zLength/2.0), Vector(-width / 2.0, 0, zLength/2.0), 0);
			bd->DrawLine(Vector(width / 2.0, 0, -zLength/2.0), Vector(width / 2.0, 0, zLength/2.0), 0);

			Matrix m_circle;
			m_circle.off = Vector64(0, 0, zLength/2.0);
			m_circle.sqmat.v1 = Vector64(xLength / 2.0, 0, 0);
			m_circle.sqmat.v2 = Vector64(0, yLength / 2.0, 0);
			bd->DrawCircle(m_circle);
			
			m_circle.off = Vector64(0, 0, -zLength/2.0);
			m_circle.sqmat.v1 = Vector64(xLength / 2.0, 0, 0);
			m_circle.sqmat.v2 = Vector64(0, yLength / 2.0, 0);
			bd->DrawCircle(m_circle);
		}		
	}
	return ObjectData::Draw(op, drawpass, bd, bh);
}


Int32 LightCard::GetHandleCount(BaseObject* op) {
	BaseContainer *data = op->GetDataInstance();
	if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_RECTANGLE)
		return 4;
	else if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_DISC)
		return 4;
	else if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_SPHERE || data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_CYLINDER)
		return 0;
	return 4;
}

void LightCard::GetHandle(BaseObject* op, Int32 i, HandleInfo& info) 
{
	BaseContainer *data = op->GetDataInstance();
	if (data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_SPHERE || data->GetInt32(LIGHTCARD_SHAPE) == LIGHTCARD_SHAPE_CYLINDER)
		return;

	float width = data->GetFloat(LIGHTCARD_WIDTH);
	float height = data->GetFloat(LIGHTCARD_HEIGHT);

	if (i == 0) 
	{
		info.position = Vector(-width / 2.0, 0, 0);
		info.direction = Vector(-1.0, 0, 0);
		info.type = HANDLECONSTRAINTTYPE::LINEAR;
		//return (Vector(-width/2.0,0,0));
	}
	else if (i == 1) 
	{
		info.position = Vector(width / 2.0, 0, 0);
		info.direction = Vector(1.0, 0, 0);
		info.type = HANDLECONSTRAINTTYPE::LINEAR;
		//return (Vector(width/2.0,0,0));
	}
	else if (i == 2) 
	{
		info.position = Vector(0, -height / 2.0, 0);
		info.direction = Vector(0, -1.0, 0);
		info.type = HANDLECONSTRAINTTYPE::LINEAR;
		//return (Vector(0,-height/2.0,0));
	}
	else if (i == 3) 
	{
		info.position = Vector(0, height / 2.0, 0);
		info.direction = Vector(0, 1.0, 0);
		info.type = HANDLECONSTRAINTTYPE::LINEAR;
		//return (Vector(0,height/2.0,0));
	}
}

void LightCard::SetHandle(BaseObject* op, Int32 i, Vector p, const HandleInfo& info) {
	BaseContainer *data = op->GetDataInstance();
	//float width=data->GetFloat(LIGHTCARD_WIDTH);
	//float height=data->GetFloat(LIGHTCARD_HEIGHT);

	if (i == 0 || i == 1) 
	{
		data->SetFloat(LIGHTCARD_WIDTH, abs(p.x) * 2);
	}
	else 
	{
		data->SetFloat(LIGHTCARD_HEIGHT, abs(p.y) * 2);
	}
}


Bool RegisterLightCard(void)
{
	return RegisterObjectPlugin(ID_LIGHTCARD,"DL_Arealight"_s,OBJECT_GENERATOR|PLUGINFLAG_HIDEPLUGINMENU,LightCard::Alloc,"Olightcard"_s,AutoBitmap("shelf_quadraticAreaLight_200.png"_s),0);
}
