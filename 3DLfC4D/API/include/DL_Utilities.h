#ifndef DL_UTILITIES_H
#define DL_UTILITIES_H

#include "c4d.h"

inline Vector toLinear(Vector color, BaseDocument* doc){
	BaseContainer* data=doc->GetDataInstance();
	long colorprofile=data->GetInt32(DOCUMENT_COLORPROFILE,2); //0=SRGB, 1=LINEAR, 2=DISABLED;
	bool lwf=data->GetBool(DOCUMENT_LINEARWORKFLOW,TRUE);

	if((!lwf) || (colorprofile!=0)){
		return color;
	}
	return TransformColor(color, COLORSPACETRANSFORMATION_SRGB_TO_LINEAR);
}

#endif