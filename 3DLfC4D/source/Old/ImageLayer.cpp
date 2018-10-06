#include "c4d.h"
#include "IDs.h"
#include "olayer.h"

class ImageLayer : public ObjectData{
public:
	static NodeData *Alloc(void) { return gNew ImageLayer ; }
	virtual Bool Init(GeListNode* node);
};

Bool ImageLayer ::Init(GeListNode* node){
	BaseObject		*op   = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();
	
	//data->SetString(FILENAME,"frame#5f.#d");
	//data->SetLong(DRIVER,DRIVER_TIFF);
	//data->SetString(VARIABLE,"rgba");
	//data->SetLong(DEPTH,DEPTH_16);

	return true;
}





Bool RegisterImageLayer(void)
{
	return RegisterObjectPlugin(ID_IMAGELAYER,"NS_AOV",OBJECT_GENERATOR,ImageLayer ::Alloc,"Olayer",AutoBitmap("Layer.tif"),0);
}
