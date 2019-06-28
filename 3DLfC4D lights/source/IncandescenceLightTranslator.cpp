#include "IncandescenceLightTranslator.h"
#include "oincandescencelight.h"
#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include <math.h>
#include <customgui_inexclude.h>


void IncandescenceLightTranslator::CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser) {
	NSI::Context ctx(parser->GetContext());

	handle = string(Handle);
	transform_handle = string(ParentTransformHandle);

	/* This is a transform now, we will change it to a NSI set */
	ctx.Delete(transform_handle);

	//Needed for multi-light layer
	ctx.Create(transform_handle, "set");

	BaseList2D *obj = (BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance();

	
}

void IncandescenceLightTranslator::SampleAttributes(DL_SampleInfo* info, const char* Handle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());

	BaseList2D *obj = (BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance();

	float intensity = data->GetFloat(INCANDESCENCE_LIGHT_INTENSITY);
	float exposure = data->GetFloat(INCANDESCENCE_LIGHT_EXPOSURE);
	Vector color = toLinear(data->GetVector(INCANDESCENCE_LIGHT_COLOR), doc);

	float col[3];
	col[0] = color.x;
	col[1] = color.y;
	col[2] = color.z;

	float outputColor[3];
	outputColor[0] = color.x * intensity * exp2f(exposure);
	outputColor[1] = color.y * intensity * exp2f(exposure);
	outputColor[2] = color.z * intensity * exp2f(exposure);


	InExcludeData*  geometry_list = NULL;
	geometry_list = (InExcludeData*)data->GetCustomDataType(INCANDESCENCE_LIGHT_GEOMETRY, CUSTOMDATATYPE_INEXCLUDE_LIST);

	BaseList2D* geometry_object = NULL;
	if (geometry_list) {

		long nlights = geometry_list->GetObjectCount();
		for (long i = 0; i < nlights; i++) {
			geometry_object = NULL;
			geometry_object = geometry_list->ObjectFromIndex(doc, i);
			if (geometry_object) {
				BaseObject* object = (BaseObject*)geometry_object;
				if (!object)
					continue;

				std::string object_handle = parser->GetHandleName((BaseList2D*)object);
				std::string object_transform = string("X_") + string(object_handle);
				ctx.Connect(object_transform, "", transform_handle, "objects");

				TextureTag* tag = static_cast<TextureTag*>(object->GetTag(Ttexture));
				if (tag == nullptr)
					continue;
				BaseMaterial* const material = tag->GetMaterial();

				if (material != nullptr)
				{
					std::string material_handle = parser->GetHandleName((BaseList2D*)material)+string("shader");
					ctx.SetAttribute(material_handle, NSI::ColorArg("incandescence_multiplier", &outputColor[0]));
				}
			}
		}
	}
}
