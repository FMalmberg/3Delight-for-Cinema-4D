#include "DataMap.h"
using namespace std;
#include "c4d.h"

void DataMap::SetFloat(char* name, float value){
	string s(name);
	FloatParams[s]=value;
}

void DataMap::SetLong(char* name, long value){
	string s(name);
	LongParams[s]=value;
}

void DataMap::SetBool(char* name, bool value){
	string s(name);
	BoolParams[s]=value;
}

void DataMap::SetString(char* name, char* value){
	string s(name);
	string valuestring(value);
	StringParams[s]=valuestring;

}

float DataMap::GetFloat(char* name, float default_value){
	float result=default_value;
	map<string,float>::iterator it=FloatParams.find(name);
	if(it!=FloatParams.end()){
		result=it->second;
	}
	return result;
}

long DataMap::GetLong(char* name, long default_value){
	long result=default_value;
	map<string,long>::iterator it=LongParams.find(name);
	if(it!=LongParams.end()){
		result=it->second;
	}
	return result;
}

bool DataMap::GetBool(char* name, bool default_value){
	bool result=default_value;
	map<string,bool>::iterator it=BoolParams.find(name);
	if(it!=BoolParams.end()){
		result=it->second;
	}
	return result;
}

const char* DataMap::GetString(char* name, char* default_value){
	const char* result=default_value;
	map<string,string>::iterator it=StringParams.find(name);
	if(it!=StringParams.end()){
		result=it->second.c_str();
	}
	return result;
}