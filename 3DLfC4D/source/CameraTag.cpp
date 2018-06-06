#include "c4d.h"
#include "tcamera.h"
#include "IDs.h"


class DL_CameraTag : public TagData
{
public:
	virtual Bool Init(GeListNode *node);

	static NodeData *Alloc(void) { return NewObjClear(DL_CameraTag); }
};

Bool DL_CameraTag::Init(GeListNode *node)
{
	BaseTag				*tag = (BaseTag*)node;
	BaseContainer *data = tag->GetDataInstance();

	data->SetFloat(SHUTTER_ANGLE, 0.5);
	data->SetFloat(SHUTTER_OPENING_EFFICIENCY, 0.75);
	data->SetFloat(SHUTTER_CLOSING_EFFICIENCY, 0.75);
	data->SetBool(ENABLE_DOF, false);
	data->SetFloat(F_STOP, 2);
	data->SetInt32(NUMBER_OF_BLADES, 6);

	return TRUE;
}

Bool RegisterDL_CameraTag(void)
{
	return RegisterTagPlugin(ID_DL_CAMERATAG, "DL_CameraProperties", TAG_VISIBLE, DL_CameraTag::Alloc, "tcamera", AutoBitmap("filmcamera.tif"), 0);
}

