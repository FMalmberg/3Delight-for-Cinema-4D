#include "NS_ShaderHandle.h"
#include <vector>

class ShaderHandle : public NS_ShaderHandle{
public:
	ShaderHandle();
	void Set(long l);
	virtual char* GetHandleString();
	virtual void Free();

private:
	std::vector<char> handlestring;
};