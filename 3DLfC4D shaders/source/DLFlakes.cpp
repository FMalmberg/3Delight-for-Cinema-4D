// example for a complex channel shader with custom areas
// and animated preview
#include "c4d.h"
#include "c4d_symbols.h"
#include "dl_flakes.h"
#include "IDs.h"

class DLFlakes : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	virtual Bool Draw(BaseShader* sh, BaseObject* op, BaseTag* tag, BaseDraw* bd, BaseDrawHelp* bh);
	static NodeData* Alloc(void) { return NewObjClear(DLFlakes); }
};

Bool DLFlakes::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();
	data->SetInt32(FLAKES_DENSITY, 0.5);
	data->SetInt32(FLAKES_SCALE, 1.0);
	data->SetFloat(FLAKES_RANDOMNESS, 0.5);
	data->SetInt32(FLAKES_LAYERS, 1);
	
	return true;
}

Bool DLFlakes::Draw(BaseShader* sh, BaseObject* op, BaseTag* tag, BaseDraw* bd, BaseDrawHelp* bh)
{ 
		BaseContainer *data = op->GetDataInstance();
		float width = 200;
		float height = 200;

		Vector color = HSVToRGB(Vector(0, 0, 1));

		Matrix m = bh->GetMg();
		bd->SetMatrix_Matrix(NULL, m);

		bd->SetPen(color);

		//bd->LineNew(Vector(-width/2.0,-height/2.0,0),Vector(-width/2.0,height/2.0,0),0);
		//bd->LineNew(Vector(-width/2.0,height/2.0,0),Vector(width/2.0,height/2.0,0),0);
		//bd->LineNew(Vector(width/2.0,height/2.0,0),Vector(width/2.0,-height/2.0,0),0);
		//bd->LineNew(Vector(width/2.0,-height/2.0,0),Vector(-width/2.0,-height/2.0,0),0);

		//Outline
		bd->DrawLine(Vector(-width / 2.0, -height / 2.0, -100), Vector(-width / 2.0, height / 2.0, -100), 0);
		bd->DrawLine(Vector(-width / 2.0, height / 2.0, -100), Vector(width / 2.0, height / 2.0, -100), 0);
		bd->DrawLine(Vector(width / 2.0, height / 2.0, -100), Vector(width / 2.0, -height / 2.0, -100), 0);
		bd->DrawLine(Vector(width / 2.0, -height / 2.0, -100), Vector(-width / 2.0, -height / 2.0, -100), 0);

		bd->DrawLine(Vector(-width / 2.0, -height / 2.0, 100), Vector(-width / 2.0, height / 2.0, 100), 0);
		bd->DrawLine(Vector(-width / 2.0, height / 2.0, 100), Vector(width / 2.0, height / 2.0, 100), 0);
		bd->DrawLine(Vector(width / 2.0, height / 2.0, 100), Vector(width / 2.0, -height / 2.0, 100), 0);
		bd->DrawLine(Vector(width / 2.0, -height / 2.0, 100), Vector(-width / 2.0, -height / 2.0, 100), 0);

		bd->DrawLine(Vector(-100, -100, -100), Vector(-100, -100, 100), 0);
		bd->DrawLine(Vector(-100, -100, 100), Vector(-100, 100, 100), 0);
		bd->DrawLine(Vector(-100, 100, 100), Vector(-100, 100, -100), 0);
		bd->DrawLine(Vector(-100, 100, -100), Vector(-100, -100, -100), 0);

		bd->DrawLine(Vector(100, -100, -100), Vector(100, -100, 100), 0);
		bd->DrawLine(Vector(100, -100, 100), Vector(100, 100, 100), 0);
		bd->DrawLine(Vector(100, 100, 100), Vector(100, 100, -100), 0);
		bd->DrawLine(Vector(100, 100, -100), Vector(100, -100, -100), 0);

		bd->DrawLine(Vector(-100, -100, -100), Vector(-100, -100, 100), 0);
		bd->DrawLine(Vector(-100, -100, 100), Vector(100, -100, 100), 0);
		bd->DrawLine(Vector(100, -100, 100), Vector(100, -100, -100), 0);
		bd->DrawLine(Vector(100, -100, -100), Vector(-100, -100, -100), 0);


		bd->DrawLine(Vector(-100, 100, -100), Vector(-100, 100, 100), 0);
		bd->DrawLine(Vector(-100, 100, 100), Vector(100, 100, 100), 0);
		bd->DrawLine(Vector(100, 100, 100), Vector(100, 100, -100), 0);
		bd->DrawLine(Vector(100, 100, -100), Vector(-100, 100, -100), 0);

		//double halfw=width/2.0;
		//double halfh=height/2.0;

		//Cross


		//Direction
		bd->DrawLine(Vector(0, 0, 0), Vector(0, 0, width / 2.0), 0);


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
	

		return true;
}


Vector DLFlakes::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterFlakesTexture(void)
{
	return RegisterShaderPlugin(DL_Flakes, "Flakes"_s, 0, DLFlakes::Alloc, "dl_flakes"_s, 0);
}
