#include "BaseObjectTranslator.h"
#include "nsi.hpp"
#include <map>
#include <vector>

PolygonObject* BaseObjectTranslator::ParseObjectToPolygon(BaseObject* obj){
	ModelingCommandData mcd;
	mcd.op = obj;
	mcd.doc = obj->GetDocument();
	if (!SendModelingCommand(MCOMMAND_CURRENTSTATETOOBJECT, mcd)) return nullptr;
	ModelingCommandData mcd2;
	mcd2.op = static_cast<BaseObject*>(mcd.result->GetIndex(0));
	PolygonObject *Dobj;
	Dobj = static_cast<PolygonObject*>(mcd2.op);

	return Dobj;
}