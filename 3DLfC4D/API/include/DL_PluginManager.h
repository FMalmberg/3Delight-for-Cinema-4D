#ifndef DL_PLUGIN_MANAGER_H
#define DL_PLUGIN_MANAGER_H

#include "DL_Hook.h"
#include "DL_Translator.h"

#define DL_LOAD_PLUGINS 1037338 //ID acquired from PluginCafe

//virtual interface
class DL_PluginManager{
public:
	//virtual ~DL_PluginManager() = 0;
	virtual long GetAPIVersion()=0;

	virtual void RegisterHook(HookAllocator allocator) = 0;
	virtual void RegisterTranslator(long id, TranslatorAllocator allocator) = 0;

};



#endif