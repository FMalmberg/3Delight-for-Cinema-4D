#include "c4d.h"
#include "IDs.h"

class DL_Skin_command : public CommandData
{

public:
	virtual Bool Execute(BaseDocument *doc);
};



Bool DL_Skin_command::Execute(BaseDocument *doc)
{
	Material* material = (Material*)BaseMaterial::Alloc(DL_SKIN);
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


Bool Register_Skin_Object(void)
{
	return RegisterCommandPlugin(DL_SKIN_COMMAND, "Skin Material"_s, PLUGINFLAG_HIDEPLUGINMENU, AutoBitmap("shelf_dlSkin_200.png"_s), String("Assign new Skin"), NewObjClear(DL_Skin_command));

}
