#include "ShaderHandle.h"
#include "NS_TypeConversions.h"

ShaderHandle::ShaderHandle(){
	handlestring=StringToChars(String(""));
}

void ShaderHandle::Set(long l){
		handlestring=StringToChars(LongToString(l));	
}

char* ShaderHandle::GetHandleString(){
	return &handlestring[0];
}

void ShaderHandle::Free(){
	delete this;
}