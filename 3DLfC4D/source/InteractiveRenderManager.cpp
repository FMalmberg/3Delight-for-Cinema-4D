 #include "c4d.h"
#include "SceneParser.h"
#include "IDs.h"
#include "nsi.hpp"

//Global variables for interactive rendering
//bool interactive_rendering_active = false;

 class InteractiveRenderManager : public MessageData{
      //virtual LONG GetTimer(void);
 public:
	 InteractiveRenderManager();
      virtual Bool CoreMessage(Int32 id, const BaseContainer &bc);

 private:
	 SceneParser parser;
	 bool active;
	 NSIContext_t context; 
 };
  
 InteractiveRenderManager::InteractiveRenderManager(){
	 active = false;

 }

 Bool InteractiveRenderManager ::CoreMessage(Int32 id, const BaseContainer &bc){
     
	  if (id==EVMSG_CHANGE && active)
      {
           //Update interactive render here
		  BaseDocument* doc=GetActiveDocument();
		  BaseObject* obj=doc->GetFirstObject();
		  parser.InteractiveUpdate();
		  NSI::Context ctx(context);
		  ctx.RenderControl((
			  NSI::StringArg("action", "synchronize")
			  ));
		  //while(obj!=NULL){
			
			//GePrint(obj->GetName() + ", "+String::IntToString(Int64(obj->GetDirty(  DIRTYFLAGS_ALL ))));
			//obj=obj->GetNext();
		  //}
      }
	  else if (id == DL_START_INTERACTIVE_RENDERING){
		  BaseDocument* doc = GetActiveDocument();
		
		  NSIEnd(context);
		  context = NSIBegin(0, 0);

		  parser = SceneParser(doc, context);
		  parser.InitScene(false, 0);
		  parser.InteractiveUpdate();

		  NSI::Context ctx(context);
		  ctx.SetAttribute(NSI_SCENE_GLOBAL, (
			  NSI::IntegerArg("renderatlowpriority", 1)
			  ));

		  ctx.RenderControl((
			  NSI::StringArg("action", "start"),
			  NSI::IntegerArg("interactive", 1)
			 // NSI::PointerArg("stoppedcallback", (void*)&FrameStoppedCallback),
			  //NSI::PointerArg("stoppedcallbackdata", 0)
			  ));

		  active = true;
	  }
	  else if (id == DL_STOP_INTERACTIVE_RENDERING){
		  active = false;
		  parser = SceneParser();
		  NSIEnd(context);
		  context = NSI_BAD_CONTEXT;
	  }

  
      return TRUE;
  }
  
  
  Bool RegisterInteractiveRenderManager(void)
  {
	  return RegisterMessagePlugin(1234567, String(), 0, NewObjClear(InteractiveRenderManager));
  }