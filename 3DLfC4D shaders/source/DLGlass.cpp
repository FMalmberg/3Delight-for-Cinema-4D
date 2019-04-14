#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "dl_glass.h"

/*
	Dl_Principled class is inhertied from MaterialData which
	is a class for creating a material plugin in this case 3Delight
	Principled material.

	Init() function is used to initialize the material's UI
	In this function we define the default valuse for each
	attribute that is part of the currently registered material

	GetDEnabling() function is used to unable or disable current
	attributes that we want based on different criteria again decided by us

	RegisterMaterialPlugin() function is used to register a material plugin.
*/
class DL_Glass : public MaterialData
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
	static NodeData* Alloc(void) { return NewObjClear(DL_Glass); }

};

Bool DL_Glass::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseMaterial*)node)->GetDataInstance();
	Vector reflection_color(0.6, 0.6, 0.6);
	data->SetVector(REFLECTION_COLOR, reflection_color);
	data->SetFloat(REFLECTION_ROUGHNESS, 0);
	data->SetFloat(REFLECTION_IOR, 1.3);
	data->SetBool(REFLECTION_THIN_FILM, 0);
	data->SetFloat(REFLECTION_FILM_THICKNESS, 0.250);
	data->SetFloat(REFLECTION_FILM_IOR, 1.5);

	Vector refraction_color(1, 1, 1);
	data->SetVector(REFRACTION_COLOR, refraction_color);
	data->SetFloat(REFRACTION_ROUGHNESS, 0);
	data->SetFloat(REFRACTION_IOR, 1.3);

	data->SetBool(VOLUMETRIC_ENABLE, false);
	data->SetFloat(VOLUMETRIC_DENSITY, 1.0);
	Vector volumetric_scattering(0, 0, 0);
	data->SetVector(VOLUMETRIC_SCATTERING, volumetric_scattering);
	Vector volumetric_transparency(1, 1, 1);
	data->SetVector(VOLUMETRIC_TRANSPARENCY, volumetric_transparency);

	Vector incadescence_color(0, 0, 0);
	data->SetVector(INCADESCENCE_COLOR, incadescence_color);
	data->SetFloat(INCADESCENCE_INTENSITY, 1);
	data->SetInt32(BUMP_TYPE, BUMP_MAP);
	data->SetFloat(BUMP_INTENSITY, 1);
	//color = data->GetVector(SIMPLEMATERIAL_COLOR);
	return true;
}

Bool DL_Glass::GetDEnabling(
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

	case REFLECTION_COLOR:
		return dldata->GetLink(REFLECTION_COLOR_SHADER, GetActiveDocument()) == false;
		
	case REFLECTION_ROUGHNESS:
		return dldata->GetLink(REFLECTION_ROUGHNESS_SHADER, GetActiveDocument()) == false;
		
	case REFLECTION_IOR:
		return dldata->GetLink(REFLECTION_IOR_SHADER, GetActiveDocument()) == false;

	case REFLECTION_FILM_THICKNESS:
		return (dldata->GetBool(REFLECTION_THIN_FILM) &&
			!dldata->GetLink(REFLECTION_FILM_THICKNESS_SHADER, doc)) == true;
		break;

	case REFLECTION_FILM_IOR:
		return (dldata->GetBool(REFLECTION_THIN_FILM) &&
			!dldata->GetLink(REFLECTION_FILM_IOR_SHADER, doc)) == true;
		break;

	case REFRACTION_COLOR:
		return dldata->GetLink(REFRACTION_COLOR_SHADER, GetActiveDocument()) == false;

	case REFRACTION_ROUGHNESS:
		return dldata->GetLink(REFRACTION_ROUGHNESS_SHADER, GetActiveDocument()) == false;

	case REFRACTION_IOR:
		return dldata->GetLink(REFRACTION_IOR_SHADER, GetActiveDocument()) == false;

	case VOLUMETRIC_DENSITY:
	case VOLUMETRIC_SCATTERING:
	case VOLUMETRIC_TRANSPARENCY:
		return dldata->GetBool(VOLUMETRIC_ENABLE) == true;
		break;

	case INCADESCENCE_COLOR:
		return dldata->GetLink(INCADESCENCE_COLOR_SHADER, GetActiveDocument()) == false;
		break;

	default:
		break;

	}
	return true;
}

Bool DL_Glass::Message(GeListNode *node, Int32 type, void *data)
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
INITRENDERRESULT DL_Glass::InitRender(BaseMaterial* mat, const InitRenderStruct& irs)
{
	BaseContainer* data = mat->GetDataInstance();
	return INITRENDERRESULT::OK;
}


Bool RegisterDLGlass(void)
{
	return RegisterMaterialPlugin(DL_GLASS, "DL_Glass"_s, 0, DL_Glass::Alloc, "Dl_glass"_s, 0);
}
