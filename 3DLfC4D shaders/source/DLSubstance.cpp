#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "dl_substance.h"

class DL_Substance : public MaterialData
{
	INSTANCEOF(DL_Substance, MaterialData)

private:
	Vector color;

public:
	virtual Bool Init(GeListNode* node);
	virtual	INITRENDERRESULT InitRender(BaseMaterial* mat, const InitRenderStruct& irs);
	virtual Bool GetDEnabling(GeListNode *node, const DescID &id, const GeData &t_data,
		DESCFLAGS_ENABLE flags, const BaseContainer *itemdesc);
	virtual Bool Message(GeListNode *node, Int32 type, void *data);
	virtual	void CalcSurface(BaseMaterial* mat, VolumeData* vd);
	static NodeData* Alloc(void) { return NewObjClear(DL_Substance); }

};

Bool DL_Substance::Init(GeListNode* node)
{
	const Float step = 360;
	BaseContainer* data = ((BaseMaterial*)node)->GetDataInstance();
	Vector substance_color = HSVToRGB(Vector(38.182 / step, 0, 0.5));
	data->SetVector(SUBSTANCE_COLOR, substance_color);
	data->SetFloat(SUBSTANCE_ROUGHNESS, 0.3);
	data->SetFloat(SUBSTANCE_SPECULAR_LEVEL, 0.5);
	data->SetFloat(SUBSTANCE_METALLIC, 0);
	data->SetFloat(SUBSTANCE_OPACITY, 1);

	Vector emissive_color = HSVToRGB(Vector(39.231 / step, 0, 0));
	data->SetVector(EMISSIVE_COLOR, emissive_color);
	data->SetFloat(EMISSIVE_INTENSITY, 1.0);

	data->SetInt32(BUMP_TYPE, BUMP_MAP);
	data->SetFloat(BUMP_INTENSITY, 1);
	return true;
}

Bool DL_Substance::GetDEnabling(
	GeListNode *node,
	const DescID &id,
	const GeData &t_data,
	DESCFLAGS_ENABLE flags,
	const BaseContainer *itemdesc)
{
	return true;
}

Bool DL_Substance::Message(GeListNode *node, Int32 type, void *data)
{
	return true;
}

//Initializes resources for rendering.
INITRENDERRESULT DL_Substance::InitRender(BaseMaterial* mat, const InitRenderStruct& irs)
{
	BaseContainer* data = mat->GetDataInstance();
	return INITRENDERRESULT::OK;
}

void DL_Substance::CalcSurface(BaseMaterial* mat, VolumeData* vd)
{

	Vector diff, spec;
	vd->IlluminanceSimple(&diff, &spec, 0, 0, 0);
	vd->col = 0.8*diff;
}

Bool RegisterDLSubstance(void)
{
	return RegisterMaterialPlugin(DL_SUBSTANCE, "Substance Material"_s, PLUGINFLAG_HIDE, DL_Substance::Alloc, "DL_Substance"_s, 0);
}
