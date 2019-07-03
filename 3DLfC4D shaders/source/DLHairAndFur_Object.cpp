#include "c4d.h"
#include "IDs.h"

class DLHairAndFur_command : public CommandData
{

public:
	int shelf_used = 0;
	virtual Bool Execute(BaseDocument* doc);
};



Bool DLHairAndFur_command::Execute(BaseDocument* doc)
{
	if (shelf_used == 1)
	{
		Material* material = (Material*)BaseMaterial::Alloc(DL_HAIRANDFUR);
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
		Material* material = (Material*)BaseMaterial::Alloc(DL_HAIRANDFUR);
		if (!material)
			return false;
		doc->InsertMaterial(material);
	}
	EventAdd();

	return true;
}


Bool Register_HairAndFur_Object(void)
{
	DLHairAndFur_command* new_hair_and_fur = NewObjClear(DLHairAndFur_command);
	RegisterCommandPlugin(DL_HAIRANDFUR_COMMAND, "Hair and Fur Material"_s, PLUGINFLAG_HIDEPLUGINMENU, AutoBitmap("shelf_dlHairAndFur_200.png"_s), String("Assign new Hair and Fur"_s), NewObjClear(DLHairAndFur_command));
	if (RegisterCommandPlugin(DL_HAIRANDFUR_SHELF_COMMAND, "Hair and Fur Material"_s, PLUGINFLAG_HIDEPLUGINMENU, AutoBitmap("shelf_dlHairAndFur_200.png"_s), String("Assign new Hair and Fur Material"_s), new_hair_and_fur))
		new_hair_and_fur->shelf_used = 1;
	return true;
}
