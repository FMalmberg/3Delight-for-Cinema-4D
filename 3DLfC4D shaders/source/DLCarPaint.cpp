#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "dl_car_paint.h"

class DL_CarPaint : public MaterialData
{
	INSTANCEOF(DL_CarPaint, MaterialData)

private:
	Vector color;

public:
	virtual Bool Init(GeListNode* node);
	virtual	INITRENDERRESULT InitRender(BaseMaterial* mat, const InitRenderStruct& irs);
	virtual Bool GetDEnabling(GeListNode *node, const DescID &id, const GeData &t_data,
		DESCFLAGS_ENABLE flags, const BaseContainer *itemdesc);
	virtual Bool Message(GeListNode *node, Int32 type, void *data);
	virtual	void CalcSurface(BaseMaterial* mat, VolumeData* vd);
	static NodeData* Alloc(void) { return NewObjClear(DL_CarPaint); }

};

Bool DL_CarPaint::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseMaterial*)node)->GetDataInstance();
	data->SetFloat(COATING_LAYER_THICKNESS, 0.050);
	Vector coating_color = HSVToRGB(Vector(39.231, 0, 0.9));
	data->SetVector(COATING_LAYER_COLOR, coating_color);
	data->SetFloat(COATING_LAYER_ROUGHNESS, 0.050);
	data->SetFloat(COATING_LAYER_SPECULAR_LEVEL, 0.5);

	data->SetFloat(FLAKES_DENSITY, 0.3);
	Vector flakes_color = HSVToRGB(Vector(39.231, 0, 1));
	data->SetVector(FLAKES_COLOR, flakes_color);
	data->SetFloat(FLAKES_ROUGHNESS, 0.3);
	data->SetFloat(FLAKES_SCALE, 0.1);
	data->SetFloat(FLAKES_RANDOMNESS, 0.3);


	Vector base_color = HSVToRGB(Vector(39.231, 0, 0.8));
	data->SetVector(BASE_LAYER_COLOR, base_color);
	data->SetFloat(BASE_LAYER_ROUGHNESS, 0.3);
	data->SetFloat(BASE_LAYER_SPECULAR_LEVEL, 0.5);
	data->SetFloat(BASE_LAYER_METALLIC, 0);
	
	data->SetInt32(BUMP_TYPE, BUMP_MAP);
	data->SetFloat(BUMP_INTENSITY, 1);
	data->SetInt32(BUMP_LAYERS_AFFECTED, AFFECT_BOTH_LAYERS);

	//color = data->GetVector(SIMPLEMATERIAL_COLOR);
	return true;
}

Bool DL_CarPaint::GetDEnabling(
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

	case BASE_LAYER_COLOR:
		return dldata->GetLink(BASE_LAYER_COLOR_SHADER, GetActiveDocument()) == false;

	case BASE_LAYER_ROUGHNESS:
		return dldata->GetLink(BASE_LAYER_ROUGHNESS_SHADER, GetActiveDocument()) == false;

	case BASE_LAYER_SPECULAR_LEVEL:
		return dldata->GetLink(BASE_LAYER_SPECULAR_LEVEL_SHADER, GetActiveDocument()) == false;

	case BASE_LAYER_METALLIC:
		return dldata->GetLink(BASE_LAYER_METALLIC_SHADER, GetActiveDocument()) == false;

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

Bool DL_CarPaint::Message(GeListNode *node, Int32 type, void *data)
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
INITRENDERRESULT DL_CarPaint::InitRender(BaseMaterial* mat, const InitRenderStruct& irs)
{
	BaseContainer* data = mat->GetDataInstance();
	return INITRENDERRESULT::OK;
}


void DL_CarPaint::CalcSurface(BaseMaterial* mat, VolumeData* vd)
{

	Vector diff, spec;
	vd->IlluminanceSimple(&diff, &spec, 0, 0, 0);

	vd->col = 0.8*diff;
}

Bool RegisterDLCarPaint(void)
{
	return RegisterMaterialPlugin(DL_CARPAINT, "Car Paint"_s, PLUGINFLAG_HIDE, DL_CarPaint::Alloc, "dl_car_paint"_s, 0);
}
