#include "c4d.h"
#include "IDs.h"

class DL_Glass_command : public CommandData
{

public:
	virtual Bool Execute(BaseDocument* doc);
};


Bool DL_Glass_command::Execute(BaseDocument* doc)
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
	EventAdd();
	return true;
}


Bool Register_DlGlass_Object(void)
{
	return RegisterCommandPlugin(DL_GLASS_COMMAND, "Glass Material"_s, PLUGINFLAG_HIDEPLUGINMENU|PLUGINFLAG_TOOL_SINGLECLICK, AutoBitmap("shelf_dlGlass_200.png"_s), String("Assign new Glass"), NewObjClear(DL_Glass_command));
}

/*

	#include "c4d.h"
	#include "IDs.h"

	class DL_Glass_command : public ObjectData
	{
	public:
	virtual Bool Init(GeListNode *node);
	static NodeData *Alloc(void) { return NewObj(DL_Glass_command) iferr_ignore("Wrong Instance"); }

	};



	Bool DL_Glass_command::Init(GeListNode* node)
	{
	BaseDocument* doc = GetActiveDocument();
	Material* material = (Material*)BaseMaterial::Alloc(DL_GLASS);
	if (!material)
	return false;
	doc->InsertMaterial(material);
	return true;
	}


	Bool Register_DlGlass_Object(void)
	{
	return RegisterObjectPlugin(DL_GLASS_COMMAND, "Glass Material"_s, PLUGINFLAG_HIDEPLUGINMENU, DL_Glass_command::Alloc ,String(), AutoBitmap("shelf_dlGlass_200.png"_s),0);
	}


*/