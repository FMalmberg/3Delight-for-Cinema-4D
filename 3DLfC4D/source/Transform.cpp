#include "Transform.h"
#include "nsi.hpp"
#include "DL_TypeConversions.h"

Transform::Transform(BaseObject* object, DL_SceneParser* parser, std::string parent){
	obj=object;
	
	NSI::Context ctx(parser->GetContext());

	//handle=std::string(parser->GetUniqueName("transform"));
	handle = std::string("X_")+std::string(parser->GetHandleName((BaseList2D*)object));
	ctx.Create(handle, "transform");
	ctx.Connect(handle, "", parent, "objects");
}

void Transform::SampleAttributes(DL_SampleInfo* info,  BaseDocument* document, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Sample transform matrix
	Matrix m=obj->GetMl();
	vector<double> v=MatrixToNSIMatrix(m);

	NSI::Argument xform("transformationmatrix");
	xform.SetType(NSITypeDoubleMatrix);
	xform.SetValuePointer((void*)&v[0]);

	ctx.SetAttributeAtTime(handle, info->sample_time, (
			xform
		));
}
	
std::string Transform::GetHandle(){
	return handle;
}