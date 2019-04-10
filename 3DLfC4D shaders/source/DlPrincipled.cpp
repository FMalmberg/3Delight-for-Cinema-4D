#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "dl_principled.h"

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
class DL_Principled : public MaterialData
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
	static NodeData* Alloc(void) { return NewObjClear(DL_Principled);}

};

Bool DL_Principled::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseMaterial*)node)->GetDataInstance();
	data->SetFloat(COATING_LAYER_THICKNESS, 0);
	Vector coating_color(1, 0.5, 0.1);
	data->SetVector(COATING_LAYER_COLOR, coating_color);
	data->SetFloat(COATING_LAYER_ROUGHNESS, 0);
	data->SetFloat(COATING_LAYER_SPECULAR_LEVEL, 0.5);

	Vector base_color(0.8, 0.8, 0.8);
	data->SetVector(BASE_LAYER_COLOR, base_color);
	data->SetFloat(BASE_LAYER_ROUGHNESS, 0.3);
	data->SetFloat(BASE_LAYER_SPECULAR_LEVEL, 0.5);
	data->SetFloat(BASE_LAYER_METALLIC, 0);
	data->SetFloat(BASE_LAYER_ANISOTROPY, 0);
	data->SetFloat(BASE_LAYER_ANISOTROPY, 0);
	Vector anisotropy_direction(0.5, 1, 0);
	data->SetVector(BASE_LAYER_ANISOTROPY_DIRECTION, anisotropy_direction);
	data->SetFloat(BASE_LAYER_OPACITY, 1);

	data->SetBool(USE_SUBSURFACE, false);
	Vector subsurface_color(0.729, 0.874, 1);
	data->SetVector(SUBSURFACE_COLOR, subsurface_color);
	data->SetFloat(SUBSURFACE_SCALE, 0.1);
	data->SetFloat(SUBSURFACE_IOR, 1.6);

	data->SetFloat(INCADESCENCE_INTENSITY, 1);
	data->SetInt32(BUMP_TYPE, BUMP_MAP);
	data->SetFloat(BUMP_INTENSITY, 1);
	data->SetInt32(BUMP_LAYERS_AFFECTED, AFFECT_BOTH_LAYERS);
	//color = data->GetVector(SIMPLEMATERIAL_COLOR);
	return true;
}

Bool DL_Principled::GetDEnabling(
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
						
		case BASE_LAYER_OPACITY:
			return dldata->GetLink(BASE_LAYER_OPACITY_SHADER, GetActiveDocument()) == false;
						
		case BASE_LAYER_ANISOTROPY:
			return dldata->GetLink(BASE_LAYER_ANISOTROPY_SHADER, GetActiveDocument()) == false;
				
		case BASE_LAYER_ANISOTROPY_DIRECTION:
			return dldata->GetLink(BASE_LAYER_ANISOTROPY_DIRECTION_SHADER, GetActiveDocument()) == false;

		case SUBSURFACE_COLOR:
			return (dldata->GetBool(USE_SUBSURFACE) &&
				!dldata->GetLink(SUBSURFACE_COLOR_SHADER, GetActiveDocument())) == true;
			break;

		case SUBSURFACE_SCALE:
			return (dldata->GetBool(USE_SUBSURFACE) &&
				!dldata->GetLink(SUBSURFACE_SCALE_SHADER, GetActiveDocument())) == true;
			break;

		case SUBSURFACE_IOR:
			return (dldata->GetBool(USE_SUBSURFACE) &&
				!dldata->GetLink(SUBSURFACE_IOR_SHADER, GetActiveDocument())) == true;
			break;

		case INCADESCENCE_COLOR:
			return dldata->GetLink(INCADESCENCE_COLOR_SHADER, GetActiveDocument()) == false;
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

Bool DL_Principled::Message(GeListNode *node, Int32 type, void *data)
{
	if (!node)
		return false;
	BaseDocument* doc = GetActiveDocument();
	BaseContainer* dldata = ((BaseMaterial*)node)->GetDataInstance();
	if (dldata->GetLink(COATING_LAYER_THICKNESS_SHADER,doc))
		dldata->SetFloat(COATING_LAYER_THICKNESS, 0.5);

	return true;
}

//Initializes resources for rendering.
INITRENDERRESULT DL_Principled::InitRender(BaseMaterial* mat, const InitRenderStruct& irs)
{
	BaseContainer* data = mat->GetDataInstance();
	return INITRENDERRESULT::OK;
}


Bool RegisterDLPrincipled(void)
{
	return RegisterMaterialPlugin(DL_PRINCIPLED, "DL_Principled"_s, PLUGINFLAG_HIDE, DL_Principled::Alloc, "Dl_principled"_s, 0);
}
