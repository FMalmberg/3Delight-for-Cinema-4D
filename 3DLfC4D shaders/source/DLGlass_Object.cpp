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
	return true;
}


Bool Register_DlGlass_Object(void)
{
	return RegisterCommandPlugin(DL_GLASS_COMMAND, "Glass Material"_s, PLUGINFLAG_HIDE, 0, String(), NewObjClear(DL_Glass_command));
}
