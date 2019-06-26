#ifndef DL_SCENE_PARSER
#define DL_SCENE_PARSER

#include "nsi.h"

//Forward declarations
class BaseDocument;
class BaseContainer;
class BaseList2D;

enum RENDER_MODE{ PREVIEW_RENDER,FINAL_RENDER };

//Pure virtual interface
class DL_SceneParser{
public:
	virtual NSIContext_t GetContext()=0;
	
	virtual const char* GetHandleName(BaseList2D* node) = 0;

	//virtual void RequestMinimumShutterTime(float t)=0;
	//virtual float GetShutterOpen()=0;
	//virtual float GetShutterClose()=0;
	virtual BaseContainer* GetSettings()=0;
	virtual RENDER_MODE GetRenderMode()=0;
	
	
	//virtual const char* GetUniqueName(char* basename)=0;
	//virtual void SetAssociatedHandle(BaseList2D* node, const char* handle)=0;
	//virtual const char* GetAssociatedHandle(BaseList2D* node)=0;
};

#endif