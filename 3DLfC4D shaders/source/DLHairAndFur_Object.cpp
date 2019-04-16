#include "c4d.h"
#include "IDs.h"

class DLHairAndFur_command : public CommandData
{

public:
	virtual Bool Execute(BaseDocument* doc);
};



Bool DLHairAndFur_command::Execute(BaseDocument* doc)
{
	Material* material = (Material*)BaseMaterial::Alloc(DL_HAIRANDFUR);
	if (!material)
		return false;
	doc->InsertMaterial(material);
	return true;
}


Bool Register_HairAndFur_Object(void)
{
	return RegisterCommandPlugin(DL_HAIRANDFUR_COMMAND, "Hair and Fur Material"_s, PLUGINFLAG_HIDE, 0, String(), NewObjClear(DLHairAndFur_command));
}
