#include "c4d.h"
#include "IDs.h"

class DL_CarPaint_command : public CommandData
{

public:
	virtual Bool Execute(BaseDocument* doc);
};



Bool DL_CarPaint_command::Execute(BaseDocument* doc)
{
	Material* material = (Material*)BaseMaterial::Alloc(DL_CARPAINT);
	if (!material)
		return false;
	doc->InsertMaterial(material);
	return true;
}


Bool Register_CarPaint_Object(void)
{
	return RegisterCommandPlugin(DL_CARPAINT_COMMAND, "Car Paint Material"_s, PLUGINFLAG_HIDE, 0, String(), NewObjClear(DL_CarPaint_command));
}
