#include "TextureTagTranslator.h"
#include "c4d.h"
#include "nsi.hpp"
#include "DL_TypeConversions.h"
#include <vector>
using namespace std;

void TextureTagTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	//NSI::Context ctx(parser->GetContext());
	transform_handle = std::string(ParentTransformHandle);

}

void TextureTagTranslator::ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	TextureTag* tag = (TextureTag*)C4DNode;

	BaseMaterial * material = tag->GetMaterial();
	if (material){
		string attributes_handle = string(parser->GetAssociatedHandle((BaseList2D*)material));
		if (attributes_handle != ""){
			ctx.Connect(attributes_handle, "", transform_handle, "geometryattributes");
		}
	}
}
