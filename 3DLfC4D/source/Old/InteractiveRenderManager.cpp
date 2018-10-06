 #include "c4d.h"
  
 class InteractiveRenderManager : public MessageData{
      //virtual LONG GetTimer(void);
      virtual Bool CoreMessage(Int32 id, const BaseContainer &bc);
 };
  

  
  Bool InteractiveRenderManager ::CoreMessage(Int32 id, const BaseContainer &bc){
     
	  if (id==EVMSG_CHANGE)
      {
           //Update interactive render here
		  BaseDocument* doc=GetActiveDocument();
		  BaseObject* obj=doc->GetFirstObject();
		 /* while(obj!=NULL){
			
			GePrint(obj->GetName() + LongToString(obj->GetDirty(  DIRTYFLAGS_ALL )));
			obj=obj->GetNext();
		  }*/
      }
  
      return TRUE;
  }
  
  
  Bool RegisterInteractiveRenderManager(void)
  {
	  return RegisterMessagePlugin(1234567, String(), 0, NewObjClear(InteractiveRenderManager));
  }