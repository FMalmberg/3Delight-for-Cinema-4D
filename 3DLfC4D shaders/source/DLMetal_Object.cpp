#include "c4d.h"
#include "IDs.h"

class DL_Metal_command : public CommandData
{

public:
	virtual Bool Execute(BaseDocument *doc);
};



Bool DL_Metal_command::Execute(BaseDocument *doc)
{
	Material* material = (Material*)BaseMaterial::Alloc(DL_METAL);
	if (!material)
		return false;
	doc->InsertMaterial(material);
	return true;
}


Bool Register_Metal_Object(void)
{
	return RegisterCommandPlugin(DL_METAL_COMMAND, "Metal Material"_s, PLUGINFLAG_HIDE, 0, String(), NewObjClear(DL_Metal_command));

}
