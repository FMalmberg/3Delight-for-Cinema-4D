#include "c4d.h"
#include "IDs.h"

class DL_CarPaint_command : public CommandData
{

public:
	int shelf_used = 0;
	virtual Bool Execute(BaseDocument* doc);
};



Bool DL_CarPaint_command::Execute(BaseDocument* doc)
{
	if (shelf_used == 1)
	{
		Material* material = (Material*)BaseMaterial::Alloc(DL_CARPAINT);
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
		Material* material = (Material*)BaseMaterial::Alloc(DL_CARPAINT);
		if (!material)
			return false;
		doc->InsertMaterial(material);
	}
	EventAdd();

	return true;
}


Bool Register_CarPaint_Object(void)
{
	DL_CarPaint_command* new_car_paint = NewObjClear(DL_CarPaint_command);
	RegisterCommandPlugin(DL_CARPAINT_COMMAND, "Car Paint Material"_s, PLUGINFLAG_HIDEPLUGINMENU, AutoBitmap("shelf_car_material.png"_s), String("Assign new Car Paint"_s), NewObjClear(DL_CarPaint_command));
	if (RegisterCommandPlugin(DL_CARPAINT_SHELF_COMMAND, "Car Paint Material"_s, PLUGINFLAG_HIDEPLUGINMENU, AutoBitmap("shelf_car_material.png"_s), String("Assign new Car Paint Material"_s), new_car_paint))
		new_car_paint->shelf_used = 1;
	return true;
}
