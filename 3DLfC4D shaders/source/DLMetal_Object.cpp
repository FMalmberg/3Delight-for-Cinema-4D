#include "c4d.h"
#include "IDs.h"

class DL_Metal_command : public CommandData
{

public:
	int shelf_used = 0;
	virtual Bool Execute(BaseDocument *doc);
};



Bool DL_Metal_command::Execute(BaseDocument *doc)
{

	if (shelf_used == 1)
	{
		Material* material = (Material*)BaseMaterial::Alloc(DL_METAL);
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
	}

	else if (shelf_used == 0)
	{
		Material* material = (Material*)BaseMaterial::Alloc(DL_METAL);
		if (!material)
			return false;
		doc->InsertMaterial(material);
	}
	EventAdd();

	return true;
}


Bool Register_Metal_Object(void)
{
	DL_Metal_command* new_metal = NewObjClear(DL_Metal_command);
	RegisterCommandPlugin(DL_METAL_COMMAND, "Metal Material"_s, PLUGINFLAG_HIDEPLUGINMENU, AutoBitmap("shelf_dlMetal_200.png"_s), String("Assign new Metal"), NewObjClear(DL_Metal_command));
	if (RegisterCommandPlugin(DL_METAL_SHELF_COMMAND, "Metal Material"_s, PLUGINFLAG_HIDEPLUGINMENU, AutoBitmap("shelf_dlMetal_200.png"_s), String("Assign new Metal Material"), new_metal));
		new_metal->shelf_used = 1;
	return true;

}
