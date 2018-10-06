
#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "vprendersettings.h"

class RenderSettings : public VideoPostData
{
	public:
		static NodeData *Alloc(void) { return NewObjClear(RenderSettings); }
		virtual Bool Init(GeListNode *node);
};

Bool RenderSettings::Init(GeListNode *node){


	BaseContainer  *dat = ((BaseVideoPost*)node)->GetDataInstance();

	dat->SetInt32(DL_GENERAL_MODE, DL_MODE_RAYTRACER);

	dat->SetInt32(DL_RENDERMODE, DL_MODE_DIRECT);
	dat->SetBool(DL_SHOW_IMAGE, true);

	dat->SetInt32(DL_PIXELSAMPLES, 9);

	dat->SetInt32(DL_PIXELFILTER, DL_FILTER_SINC);
	dat->SetFloat(DL_FILTERWIDTH, 4);

	dat->SetBool(DL_USE_MOTION_BLUR, true);
	dat->SetInt32(DL_MOTION_SAMPLES, 2);

	dat->SetFloat(DL_SHUTTER_ANGLE, 0.5);
	dat->SetFloat(DL_SHUTTER_OPENING_EFFICIENCY, 0.75);
	dat->SetFloat(DL_SHUTTER_CLOSING_EFFICIENCY, 0.75);

	dat->SetFloat(DL_TRACE_BIAS, 0.01);
	dat->SetBool(DL_RAYCACHE,true);
	dat->SetInt32(DL_SHADING_SAMPLES, 64);
	dat->SetInt32(DL_MAX_DIFFUSE_DEPTH, 3);
	dat->SetInt32(DL_MAX_REFLECTION_DEPTH, 3);
	dat->SetInt32(DL_MAX_REFRACTION_DEPTH, 3);
	dat->SetFloat(DL_GAMMA, 2.2);

	return TRUE;
}



Bool RegisterRenderSettings(void)
{
	return RegisterVideoPostPlugin(ID_RENDERSETTINGS, "3Delight", PLUGINFLAG_VIDEOPOST_ISRENDERER, RenderSettings::Alloc, "vprendersettings", 0, 0);
}

