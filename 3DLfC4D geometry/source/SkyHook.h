#ifndef SKY_HOOK
#define SKY_HOOK

#include "DL_HookPlugin.h"
#include "c4d.h"
#include <string>

class SkyHook :public DL_HookPlugin {
public:
	
	void Init(BaseDocument* doc, DL_SceneParser* parser);
	void CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser);
	void ConnectNSINodes(BaseDocument* doc, DL_SceneParser* parser);


private:

	BaseObject* sky;
	std::string skyhandle;

};

#endif
