#include "c4d.h"
#include "c4d_symbols.h"
#include "dl_hair_and_fur.h"
#include "customgui_matpreview.h"
#include "IDs.h"

class DL_Hair_Fur : public MaterialData
{
	INSTANCEOF(DL_Hair_Fur, MaterialData)

private:
	Vector color;

public:
	virtual Bool Init(GeListNode* node);
	virtual	void CalcSurface(BaseMaterial* mat, VolumeData* vd);
	virtual	INITRENDERRESULT InitRender(BaseMaterial* mat, const InitRenderStruct& irs);

	static NodeData* Alloc(void) { return NewObjClear(DL_Hair_Fur); }

	//Int32 updatecount;
};

Bool DL_Hair_Fur::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseMaterial*)node)->GetDataInstance();
	data->SetFloat(HAIR_COLOR_MELANIN, 0.2);
	data->SetFloat(HAIR_COLOR_MELANIN_RED, 0.1);
	data->SetVector(HAIR_COLOR_DYE, HSVToRGB(Vector(38.182/360.0, 0.234,0.94)));
	data->SetFloat(HAIR_COLOR_DYE_WEIGHT, 0);

	data->SetFloat(HAIR_LOOK_REFLECTIVITY, 0.047);
	data->SetFloat(HAIR_LOOK_ROUGHNESS_ALONG, 0.2);
	data->SetFloat(HAIR_LOOK_ROUGHNESS_AROUND, 0.2);
	data->SetFloat(HAIR_LOOK_SYNTHETIC_FIBER, 0);

	data->SetFloat(HAIR_VARIATION_MELANIN, 0);
	data->SetFloat(HAIR_VARIATION_MELANIN_RED, 0);
	data->SetFloat(HAIR_VARIATION_WHITE_HAIR, 0);
	data->SetFloat(HAIR_VARIATION_DYE_HUE, 0);
	data->SetFloat(HAIR_VARIATION_DYE_SATURATION, 0);
	data->SetFloat(HAIR_VARIATION_DYE_VALUE, 0);
	data->SetFloat(HAIR_VARIATION_ROUGHNESS, 0);
	data->SetFloat(HAIR_VARIATION_REFLECTIVITY, 0);

	data->SetFloat(HAIR_BOOST_GLOSSINESS, 0);
	data->SetFloat(HAIR_BOOST_REFLECTION, 0);
	data->SetFloat(HAIR_BOOST_TRANSMISSION, 0);

	return true;
}

INITRENDERRESULT DL_Hair_Fur::InitRender(BaseMaterial* mat, const InitRenderStruct& irs)
{
	BaseContainer* data = mat->GetDataInstance();
	//color = data->GetVector(SIMPLEMATERIAL_COLOR);
	return INITRENDERRESULT::OK;
}

void DL_Hair_Fur::CalcSurface(BaseMaterial* mat, VolumeData* vd)
{

	Vector diff, spec;
	vd->IlluminanceSimple(&diff, &spec, 0, 0, 0);

	vd->col = 0.8*diff;
}

Bool RegisterDLHairAndFur(void)
{
	return RegisterMaterialPlugin(DL_HAIRANDFUR, "Hair and Fur material"_s, PLUGINFLAG_HIDE, DL_Hair_Fur::Alloc, "dl_hair_and_fur"_s, 0);
}
