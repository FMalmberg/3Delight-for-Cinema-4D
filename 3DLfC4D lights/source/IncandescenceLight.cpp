#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "oincandescencelight.h"



class IncandescenceLight : public ObjectData {
public:
	static NodeData *Alloc(void) { return NewObjClear(IncandescenceLight); }
	virtual Bool Init(GeListNode* node);
};

Bool IncandescenceLight::Init(GeListNode* node) {
	BaseObject		*op = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();

	Vector color = HSVToRGB(Vector(0, 0, 1));
	data->SetVector(INCANDESCENCE_LIGHT_COLOR, color);
	data->SetFloat(INCANDESCENCE_LIGHT_INTENSITY, 1);
	data->SetFloat(INCANDESCENCE_LIGHT_EXPOSURE, 0);
	return true;
}

Bool RegisterIncandescenceLight(void)
{
	return RegisterObjectPlugin(ID_INCANDESCENCELIGHT, "Incandescence Light"_s, OBJECT_GENERATOR|PLUGINFLAG_HIDEPLUGINMENU, IncandescenceLight::Alloc, "oincandescencelight"_s, AutoBitmap("shelf_dlIncandescenceLightShape_200.png"_s), 0);
}
