#include "c4d.h"
#include "IDs.h"

class DL_Glass_command : public CommandData
{

public:
	int shelf_used = 0;
	virtual Bool Execute(BaseDocument* doc);
};


Bool DL_Glass_command::Execute(BaseDocument* doc)
{
	if (shelf_used == 1)
	{
		Material* material = (Material*)BaseMaterial::Alloc(DL_GLASS);
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
		Material* material = (Material*)BaseMaterial::Alloc(DL_GLASS);
		if (!material)
			return false;
		doc->InsertMaterial(material);
	}
	EventAdd();

	return true;
}

Bool Register_DlGlass_Object(void)
{
	DL_Glass_command* new_glass = NewObjClear(DL_Glass_command);
	RegisterCommandPlugin(DL_GLASS_COMMAND, "Glass Material"_s, PLUGINFLAG_HIDEPLUGINMENU|PLUGINFLAG_TOOL_SINGLECLICK, AutoBitmap("shelf_dlGlass_200.png"_s), String("Assign new Glass"), NewObjClear(DL_Glass_command));
	if (RegisterCommandPlugin(DL_GLASS_SHELF_COMMAND, "Glass Material"_s, PLUGINFLAG_HIDEPLUGINMENU | PLUGINFLAG_TOOL_SINGLECLICK, AutoBitmap("shelf_dlGlass_200.png"_s), String("Assign new Glass Material"), new_glass))
		new_glass->shelf_used = 1;
	return true;
}
