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
	return true;
}


Bool Register_Skin_Object(void)
{
	return RegisterCommandPlugin(DL_SKIN_COMMAND, "Skin Material"_s, PLUGINFLAG_HIDE, 0, String(), NewObjClear(DL_Skin_command));

}
