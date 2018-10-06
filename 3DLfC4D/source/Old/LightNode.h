#ifndef LIGHT_NODE_H
#define LIGHT_NODE_H

#include "NS_LightNode.h"
#include "NS_LightTranslator.h"
#include "c4d.h"
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<NS_LightTranslator> NS_LightTranslatorPtr;

class LightNode: public NS_LightNode{
private:
	BaseObject* c4d_object;
	NS_LightTranslatorPtr translator;

public:

	LightNode(BaseObject* object); 

	virtual NS_LightTranslator* GetTranslator();	
	virtual BaseObject* GetC4DNode();
};

#endif