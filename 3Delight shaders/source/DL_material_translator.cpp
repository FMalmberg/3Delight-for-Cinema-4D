#include "DL_material_translator.h"
#include "mdelightmaterial.h"
#include "c4d.h"
#include "nsi.hpp"
#include "DL_TypeConversions.h"
#include <vector>
using namespace std;

void DL_material_translator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Create an attributes node
	attributes_handle = string(parser->GetUniqueName("transform_attributes"));
	ctx.Create(attributes_handle, "attributes");

	BaseMaterial* material = (BaseMaterial*)C4DNode;
	BaseContainer* data = material->GetDataInstance();

	float displacement_bound = data->GetFloat(DISPLACEMENT_BOUND, 0);

	ctx.SetAttribute(attributes_handle, (
	NSI::FloatArg("displacementbound", displacement_bound)
	));

	parser->SetAssociatedHandle(material, attributes_handle.c_str());
}

void DL_material_translator::ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	BaseMaterial * material = (BaseMaterial*)C4DNode;

	BaseContainer* data = material->GetDataInstance();
	
	BaseList2D* surface_shader = data->GetLink(SURFACE_LINK, doc);
	if (surface_shader){
		string surface_handle = string(parser->GetAssociatedHandle(surface_shader));
		if (surface_handle != ""){
			ctx.Connect(surface_handle, "", attributes_handle, "surfaceshader");
		}
	}

	BaseList2D* displacement_shader = data->GetLink(DISPLACEMENT_LINK, doc);
	if (displacement_shader){
		string displacement_handle = string(parser->GetAssociatedHandle(displacement_shader));
		if (displacement_handle != ""){
			ctx.Connect(displacement_handle, "", attributes_handle, "displacementshader");
		}
	}

}
