#include "c4d.h"
#include "tmotionsamples.h"
#include "IDs.h"


class DL_MotionBlurTag : public TagData
{
public:
	virtual Bool Init(GeListNode *node);

	static NodeData *Alloc(void) { return NewObjPtr(DL_MotionBlurTag); }
};

Bool DL_MotionBlurTag::Init(GeListNode *node)
{
	BaseTag				*tag = (BaseTag*)node;
	BaseContainer *data = tag->GetDataInstance();
	data->SetBool(USE_TRANSFORMATION_BLUR, true);
	data->SetBool(USE_DEFORMATION_BLUR, true);

	return TRUE;
}

Bool RegisterDL_MotionBlurTag(void)
{
	return RegisterTagPlugin(ID_DL_MOTIONBLURTAG, "DL_MotionblurTag"_s, TAG_VISIBLE, DL_MotionBlurTag::Alloc, "tmotionsamples"_s, AutoBitmap("motionblur.tif"_s), 0);
}

