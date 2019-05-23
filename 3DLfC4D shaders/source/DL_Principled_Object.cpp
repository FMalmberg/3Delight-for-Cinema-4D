#include "c4d.h"
#include "IDs.h"
/*
	We want our shaders to be directly on the create menu of materials
	instead of being on create->shader->YourShaderDir->yourShader
	to do so we use C4DPL_BUILDMENU to dynamically enhance the menu

	In C4D you can only call objects or commands to C4DPL_BUILDMENU.
	But	cinema4d register an object or command using RegisterObjectPlugin()
	and RegisterCommnadPlugin() and we have used RegisterMaterialPlugin()
	to register our Dl_principled shader so we can not call it on C4DPL_BUILDMENU
	because it does not have a command created for it/
	
	Thus we create this Command plugin and we use it on C4DPL_BUILDMENU to add it
	to the create menu of materials. And each time this class is called we create a new
	Dl_Principled material. Meanwhile we have hidden Dl_Principled material from
	create->shader->3Delight->Dl_Principled because we are calling it directly 
	using this class and in an easier way from UI part.
*/
class DL_Principled_command : public CommandData
{

public:
	virtual Bool Execute(BaseDocument* doc);
};



Bool DL_Principled_command::Execute(BaseDocument* doc)
{
	Material* material = (Material*)BaseMaterial::Alloc(DL_PRINCIPLED);
	if (!material)
		return false;
	doc->InsertMaterial(material);
	AutoAlloc<AtomArray>selected_objects;
	doc->GetActiveObjects(*selected_objects, GETACTIVEOBJECTFLAGS::NONE);
	if (selected_objects)
	{
		Int32 object_count = selected_objects->GetCount();
		for (int i = 0; i < object_count; i++)
		{
			BaseObject* object = (BaseObject*)selected_objects->GetIndex(i);
			if (object)
			{
				TextureTag* const textureTag = static_cast<TextureTag*>(object->MakeTag(Ttexture));
				textureTag->SetMaterial(material);
			}
		}

	}
	EventAdd();
	return true;
}


Bool Register_DlPrincipled_Object(void)
{
	return RegisterCommandPlugin(DL_PRINCIPLED_COMMAND, "3Delight Principled"_s, PLUGINFLAG_HIDEPLUGINMENU, AutoBitmap("shelf_dlPrincipled_200.png"_s), String("Assign new Principled"_s),NewObjClear(DL_Principled_command));
}
