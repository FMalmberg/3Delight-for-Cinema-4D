#include "c4d.h"
#include "IDs.h"

class DL_Substance_command : public CommandData
{

public:
	int shelf_used = 0;
	virtual Bool Execute(BaseDocument *doc);
};



Bool DL_Substance_command::Execute(BaseDocument *doc)
{

	if (shelf_used == 1)
	{
		Material* material = (Material*)BaseMaterial::Alloc(DL_SUBSTANCE);
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
		Material* material = (Material*)BaseMaterial::Alloc(DL_SUBSTANCE);
		if (!material)
			return false;
		doc->InsertMaterial(material);
	}
	EventAdd();

	return true;

}
	

Bool Register_Substance_Object(void)
{
	DL_Substance_command* new_substance = NewObjClear(DL_Substance_command);
	RegisterCommandPlugin(DL_SUBSTANCE_COMMAND, "Substance Material"_s, PLUGINFLAG_HIDEPLUGINMENU, AutoBitmap("shelf_dlSubstance_200.png"_s), String("Assign new Substance"), NewObjClear(DL_Substance_command));
	if (RegisterCommandPlugin(DL_SUBSTANCE_SHELF_COMMAND, "Substance Material"_s, PLUGINFLAG_HIDEPLUGINMENU, AutoBitmap("shelf_dlSubstance_200.png"_s), String("Assign new Substance Material"), new_substance))
		new_substance->shelf_used = 1;
	return true;
}
