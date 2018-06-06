#ifndef TYPECONVERSIONS_H
#define TYPECONVERSIONS_H

#include "c4d.h"
#include <vector>
using namespace std;

inline vector<double> MatrixToNSIMatrix(Matrix m){
	vector<double> v(16);

	v[0] = m.v1.x;
	v[1] = m.v1.y;
	v[2] = m.v1.z;
	v[3] = 0.0;
	v[4] = m.v2.x;
	v[5] = m.v2.y;
	v[6] = m.v2.z;
	v[7] = 0.0;
	v[8] = m.v3.x;
	v[9] = m.v3.y;
	v[10] = m.v3.z;
	v[11] = 0.0;
	v[12] = m.off.x;
	v[13] = m.off.y;
	v[14] = m.off.z;
	v[15] = 1.0;

	return v;
}


inline vector<char> StringToChars(String s){
	long max= s.GetCStringLen();
	vector<char> cstr(max+1);
	s.GetCString(&cstr[0],max+1);
	return cstr;
}

inline string StringToStdString(String s){
	vector<char> chars=StringToChars(s);
	string result;
	result.resize(chars.size());
	for(int i=0; i<chars.size(); i++){
		result[i]=chars[i];
	}
	return result;
}

#endif