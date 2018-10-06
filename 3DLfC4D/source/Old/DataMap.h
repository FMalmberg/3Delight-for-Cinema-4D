#ifndef DATA_MAP
#define DATA_MAP

#include <map>
#include <string>

#include "NS_DataMap.h"

//Concrete implementation of interface
class DataMap: public NS_DataMap{
	std::map<std::string, float> FloatParams;
	std::map<std::string, long> LongParams;
	std::map<std::string, bool> BoolParams;
	std::map<std::string, std::string> StringParams;

public:
	virtual void SetFloat(char* name, float value);
	virtual void SetLong(char* name, long value);
	virtual void SetBool(char* name, bool value);
	virtual void SetString(char* name, char* value);

	virtual float GetFloat(char* name, float default_value=0.0);
	virtual long GetLong(char* name, long default_value=0);
	virtual bool GetBool(char* name, bool default_value=false);
	virtual const char* GetString(char* name, char* default_value="");
};

#endif