#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "dl_metal.h"

class DL_Metal : public MaterialData
{
	INSTANCEOF(DL_Material, MaterialData)

private:
	Vector color;

public:
	virtual Bool Init(GeListNode* node);
	virtual	INITRENDERRESULT InitRender(BaseMaterial* mat, const InitRenderStruct& irs);
	virtual Bool GetDEnabling(GeListNode *node, const DescID &id, const GeData &t_data,
		DESCFLAGS_ENABLE flags, const BaseContainer *itemdesc);
	virtual Bool Message(GeListNode *node, Int32 type, void *data);
	virtual	void CalcSurface(BaseMaterial* mat, VolumeData* vd);
	static NodeData* Alloc(void) { return NewObjClear(DL_Metal); }

};

Bool DL_Metal::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseMaterial*)node)->GetDataInstance();
	data->SetFloat(COATING_LAYER_THICKNESS, 0);
	Vector coating_color(1, 0.5, 0.1);
	data->SetVector(COATING_LAYER_COLOR, HSVToRGB(Vector(26.667/360.0, 0.9, 1.0)));
	data->SetFloat(COATING_LAYER_ROUGHNESS, 0);
	data->SetFloat(COATING_LAYER_SPECULAR_LEVEL, 0.5);

	data->SetVector(METAL_LAYER_COLOR, HSVToRGB(Vector(9.071 / 360.0, 0.431, 0.947)));
	data->SetVector(METAL_LAYER_EDGE_COLOR, HSVToRGB(Vector(15.914 / 360.0, 0.204, 1.0)));
	data->SetFloat(METAL_LAYER_ROUGHNESS, 0.2);
	data->SetFloat(METAL_LAYER_ANISOTROPY, 0);
	Vector anisotropy_direction(0.5, 1, 0);
	data->SetVector(METAL_LAYER_ANISOTROPY_DIRECTION, HSVToRGB(Vector(90 / 360.0, 1.0, 1.0)));
	data->SetFloat(METAL_LAYER_OPACITY, 1);

	data->SetBool(METAL_LAYER_TEMPERED_METAL, false);
	data->SetFloat(METAL_LAYER_OXIDE_THICKNESS, 0.250);
	data->SetFloat(METAL_LAYER_OXIDE_IOR, 2.705);

	data->SetInt32(BUMP_TYPE, BUMP_MAP);
	data->SetFloat(BUMP_INTENSITY, 1);
	data->SetInt32(BUMP_LAYERS_AFFECTED, AFFECT_BOTH_LAYERS);
	//color = data->GetVector(SIMPLEMATERIAL_COLOR);
	return true;
}

Bool DL_Metal::GetDEnabling(
	GeListNode *node,
	const DescID &id,
	const GeData &t_data,
	DESCFLAGS_ENABLE flags,
	const BaseContainer *itemdesc)
{

	if (!node)
		return false;
	BaseDocument* doc = GetActiveDocument();
	BaseContainer* dldata = ((BaseObject*)node)->GetDataInstance();
	switch (id[0].id)
	{
	case COATING_LAYER_THICKNESS:
		return dldata->GetLink(COATING_LAYER_THICKNESS_SHADER, doc) == false;
		break;

	case COATING_LAYER_COLOR:
		return (dldata->GetFloat(COATING_LAYER_THICKNESS) > 0 &&
			!dldata->GetLink(COATING_LAYER_COLOR_SHADER, doc)) == true;
		break;

	case COATING_LAYER_ROUGHNESS:
		return (dldata->GetFloat(COATING_LAYER_THICKNESS) > 0 &&
			!dldata->GetLink(COATING_LAYER_ROUGHNESS_SHADER, doc)) == true;
		break;

	case COATING_LAYER_SPECULAR_LEVEL:
		return (dldata->GetFloat(COATING_LAYER_THICKNESS) > 0 &&
			!dldata->GetLink(COATING_LAYER_SPECULAR_LEVEL_SHADER, doc)) == true;
		break;

	case COATING_LAYER_COLOR_SHADER:
	case COATING_LAYER_ROUGHNESS_SHADER:
	case COATING_LAYER_SPECULAR_LEVEL_SHADER:
		return dldata->GetFloat(COATING_LAYER_THICKNESS) > 0;
		break;
	default:
		break;

	}
	return true;
}

Bool DL_Metal::Message(GeListNode *node, Int32 type, void *data)
{
	if (!node)
		return false;
	BaseDocument* doc = GetActiveDocument();
	BaseContainer* dldata = ((BaseMaterial*)node)->GetDataInstance();
	if (dldata->GetLink(COATING_LAYER_THICKNESS_SHADER, doc))
		dldata->SetFloat(COATING_LAYER_THICKNESS, 0.5);

	return true;
}

//Initializes resources for rendering.
INITRENDERRESULT DL_Metal::InitRender(BaseMaterial* mat, const InitRenderStruct& irs)
{
	BaseContainer* data = mat->GetDataInstance();
	return INITRENDERRESULT::OK;
}

void DL_Metal::CalcSurface(BaseMaterial* mat, VolumeData* vd)
{

	Vector diff, spec;
	vd->IlluminanceSimple(&diff, &spec, 0, 0, 0);

	vd->col = 0.8*diff;
}

Bool RegisterDLMetal(void)
{
	return RegisterMaterialPlugin(DL_METAL, "Metal Material"_s, PLUGINFLAG_HIDE, DL_Metal::Alloc, "dl_metal"_s, 0);
}
