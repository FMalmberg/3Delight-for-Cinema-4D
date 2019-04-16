#include "c4d.h"
#include "IDs.h"

class DL_Substance_command : public CommandData
{

public:
	virtual Bool Execute(BaseDocument *doc);
};



Bool DL_Substance_command::Execute(BaseDocument *doc)
{
	Material* material = (Material*)BaseMaterial::Alloc(DL_SUBSTANCE);
	if (!material)
		return false;
	doc->InsertMaterial(material);
	return true;
}


Bool Register_Substance_Object(void)
{
	return RegisterCommandPlugin(DL_SUBSTANCE_COMMAND, "Substance Material"_s, PLUGINFLAG_HIDE, 0, String(), NewObjClear(DL_Substance_command));

}
