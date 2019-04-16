#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "dl_skin.h"

class DL_Skin : public MaterialData
{
	INSTANCEOF(DL_Skin, MaterialData)

private:
	Vector color;

public:
	virtual Bool Init(GeListNode* node);
	virtual	INITRENDERRESULT InitRender(BaseMaterial* mat, const InitRenderStruct& irs);
	virtual Bool GetDEnabling(GeListNode *node, const DescID &id, const GeData &t_data,
		DESCFLAGS_ENABLE flags, const BaseContainer *itemdesc);
	virtual Bool Message(GeListNode *node, Int32 type, void *data);
	virtual	void CalcSurface(BaseMaterial* mat, VolumeData* vd);
	static NodeData* Alloc(void) { return NewObjClear(DL_Skin); }

};

Bool DL_Skin::Init(GeListNode* node)
{	
	const Float step = 360;
	BaseContainer* data = ((BaseMaterial*)node)->GetDataInstance();
	Vector skin_color = HSVToRGB(Vector(21.414/step, 0.377, 0.788));
	data->SetVector(SKIN_COLOR, skin_color);
	data->SetFloat(SKIN_ROUGHNESS, 0.3);
	data->SetFloat(SKIN_SPECULAR_LEVEL, 0.5);
	
	Vector subsurface_color = HSVToRGB(Vector(9.848/step, 0.766, 0.964));
	data->SetVector(SUBSURFACE_COLOR, subsurface_color);
	data->SetFloat(SUBSURFACE_SCALE, 0.5);
	data->SetFloat(SUBSURFACE_IOR, 1.3);

	data->SetInt32(BUMP_TYPE, BUMP_MAP);
	data->SetFloat(BUMP_INTENSITY, 1);
	return true;
}

Bool DL_Skin::GetDEnabling(
	GeListNode *node,
	const DescID &id,
	const GeData &t_data,
	DESCFLAGS_ENABLE flags,
	const BaseContainer *itemdesc)
{
	return true;
}

Bool DL_Skin::Message(GeListNode *node, Int32 type, void *data)
{
	if (!node)
		return false;
	/*BaseDocument* doc = GetActiveDocument();
	BaseContainer* dldata = ((BaseMaterial*)node)->GetDataInstance();
	if (dldata->GetLink(COATING_LAYER_THICKNESS_SHADER, doc))
		dldata->SetFloat(COATING_LAYER_THICKNESS, 0.5);
*/
	return true;
}

//Initializes resources for rendering.
INITRENDERRESULT DL_Skin::InitRender(BaseMaterial* mat, const InitRenderStruct& irs)
{
	BaseContainer* data = mat->GetDataInstance();
	return INITRENDERRESULT::OK;
}

void DL_Skin::CalcSurface(BaseMaterial* mat, VolumeData* vd)
{

	Vector diff, spec;
	vd->IlluminanceSimple(&diff, &spec, 0, 0, 0);
	vd->col = 0.8*diff;
}

Bool RegisterDLSkin(void)
{
	return RegisterMaterialPlugin(DL_SKIN, "Skin Material"_s, PLUGINFLAG_HIDE, DL_Skin::Alloc, "Dl_skin"_s, 0);
}
