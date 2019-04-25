#include "Transform.h"
#include "nsi.hpp"
#include "DL_TypeConversions.h"

Transform::Transform(BaseObject* object, DL_SceneParser* parser, std::string parent){
	obj=object;
	
	NSI::Context ctx(parser->GetContext());

	handle=std::string(parser->GetUniqueName("transform"));
	ctx.Create(handle, "transform");
	ctx.Connect(handle, "", parent, "objects");
}

void Transform::SampleMotion(double t, long i,  BaseDocument* document, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Sample transform matrix
	Matrix m=obj->GetMl();
	vector<double> v=MatrixToNSIMatrix(m);

	NSI::Argument xform("transformationmatrix");
	xform.SetType(NSITypeDoubleMatrix);
	xform.SetValuePointer((void*)&v[0]);

	

	ctx.SetAttributeAtTime(handle, t, (
			xform
		));
}
	
std::string Transform::GetHandle(){
	return handle;
}