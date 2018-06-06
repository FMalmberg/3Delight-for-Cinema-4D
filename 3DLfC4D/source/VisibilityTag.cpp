#include "c4d.h"
#include "tvisibility.h"
#include "IDs.h"


class DL_VisibilityTag : public TagData
{
public:
	virtual Bool Init(GeListNode *node);

	static NodeData *Alloc(void) { return NewObjClear(DL_VisibilityTag); }
};

Bool DL_VisibilityTag::Init(GeListNode *node)
{
	BaseTag				*tag = (BaseTag*)node;
	BaseContainer *data = tag->GetDataInstance();
	data->SetBool(SEEN_BY_CAMERA, true);
	data->SetBool(SEEN_BY_DIFFUSE, true);
	data->SetBool(SEEN_BY_REFLECTION, true);
	data->SetBool(SEEN_BY_REFRACTION, true);
	data->SetBool(SEEN_BY_HAIR, true);
	data->SetBool(SEEN_BY_SHADOW, true);
	data->SetBool(SEEN_BY_VOLUME, true);
	data->SetBool(MATTE_OBJECT, false);

	/*data->SetFloat(SHUTTER_ANGLE, 0.5);
	data->SetFloat(SHUTTER_OPENING_EFFICIENCY, 0.75);
	data->SetFloat(SHUTTER_CLOSING_EFFICIENCY, 0.75);
	data->SetBool(ENABLE_DOF, false);
	data->SetFloat(F_STOP, 2);
	data->SetInt32(NUMBER_OF_BLADES, 6);*/

	return TRUE;
}

Bool RegisterDL_VisibilityTag(void)
{
	return RegisterTagPlugin(ID_DL_VISIBILITYTAG, "DL_CompositingTag", TAG_VISIBLE, DL_VisibilityTag::Alloc, "tvisibility", AutoBitmap("motionblur.tif"), 0);
}

