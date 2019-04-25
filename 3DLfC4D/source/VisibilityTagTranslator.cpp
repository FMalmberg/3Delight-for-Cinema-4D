#include  "VisibilityTagTranslator.h"
#include "tvisibility.h"
#include "nsi.hpp"
using namespace std;

void VisibilityTagTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	BaseList2D *obj = (BaseList2D*)C4DNode;
	BaseContainer *data = obj->GetDataInstance();

	Bool seen_by_camera = data->GetBool(SEEN_BY_CAMERA);
	int camera_visibility = 1;
	if (!seen_by_camera){
		camera_visibility = 0;
	}

	Bool seen_by_diffuse = data->GetBool(SEEN_BY_DIFFUSE);
	int diffuse_visibility = 1;
	if (!seen_by_diffuse){
		diffuse_visibility = 0;
	}

	Bool seen_by_reflection = data->GetBool(SEEN_BY_REFLECTION);
	int reflection_visibility = 1;
	if (!seen_by_reflection){
		reflection_visibility = 0;
	}

	Bool seen_by_refraction = data->GetBool(SEEN_BY_REFRACTION);
	int refraction_visibility = 1;
	if (!seen_by_refraction){
		refraction_visibility = 0;
	}

	Bool seen_by_hair = data->GetBool(SEEN_BY_HAIR);
	int hair_visibility = 1;
	if (!seen_by_hair){
		hair_visibility = 0;
	}

	Bool seen_by_volume = data->GetBool(SEEN_BY_VOLUME);
	int volume_visibility = 1;
	if (!seen_by_volume){
		volume_visibility = 0;
	}


	Bool seen_by_shadows = data->GetBool(SEEN_BY_SHADOW);
	int shadow_visibility = 1;
	if (!seen_by_shadows){
		shadow_visibility = 0;
	}

	Bool matte_object = data->GetBool(MATTE_OBJECT);
	int matte = 0;
	if (matte_object){
		matte = 1;
	}

	//Create an attributes node, and connect it to the mesh
	string attributes_handle = string(parser->GetUniqueName("visibility_attributes"));
	ctx.Create(attributes_handle, "attributes");
	ctx.SetAttribute(attributes_handle, (
		NSI::IntegerArg("visibility.camera", camera_visibility), //,
		NSI::IntegerArg("visibility.diffuse", diffuse_visibility),
		NSI::IntegerArg("visibility.reflection", reflection_visibility),
		NSI::IntegerArg("visibility.refraction", refraction_visibility),
		NSI::IntegerArg("visibility.hair", hair_visibility),
		NSI::IntegerArg("visibility.volume", volume_visibility),
		NSI::IntegerArg("visibility.shadow", shadow_visibility),
		NSI::IntegerArg("matte", matte)
		//NSI::IntegerArg("visibility.shadow",-1)
		));
	ctx.Connect(attributes_handle, "", ParentTransformHandle, "geometryattributes");
}