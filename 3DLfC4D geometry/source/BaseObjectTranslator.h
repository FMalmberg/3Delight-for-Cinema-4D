#ifndef BASEOBJECTTRANSLATOR_H_
#define BASEOBJECTTRANSLATOR_H_
#include "c4d.h"
#include "DL_API.h"
#include <vector>
//#include "Edge.h"
#include <string>
#include <map>
class BaseObjectTranslator : public DL_TranslatorPlugin {
public:
	virtual void CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void SampleMotion(double t, long i, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	PolygonObject* ParseObjectToPolygon(BaseObject* obj);
};
#endif // !BASEOBJECTTRANSLATOR_H_
