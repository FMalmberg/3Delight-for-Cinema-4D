#ifndef TAG_NODE_H
#define TAG_NODE_H

#include "NS_TagNode.h"
#include "NS_TagTranslator.h"
#include "c4d.h"
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<NS_TagTranslator> NS_TagTranslatorPtr;

class TagNode: public NS_TagNode{
private:
	BaseTag* c4d_tag;
	NS_TagTranslatorPtr translator;

public:

	TagNode(BaseTag* tag); 

	virtual NS_TagTranslator* GetTranslator();	
	virtual BaseTag* GetC4DNode();
};

#endif