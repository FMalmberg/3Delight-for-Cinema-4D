#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <map>
#include <vector>
#include "c4d.h"
#include "DL_API.h"

#include <memory> //Shared pointer

typedef std::map<long, TranslatorAllocator> TranslatorMap;
typedef std::shared_ptr<DL_Hook> DL_HookPtr;


class PluginManager : public DL_PluginManager{
private:
	TranslatorMap Allocators;
	std::vector<HookAllocator> HookAllocators;
	
public:
	virtual ~PluginManager();

	virtual long GetAPIVersion();

	virtual void RegisterHook(HookAllocator allocator);
	virtual void RegisterTranslator(long id, TranslatorAllocator allocator);

	DL_Translator*  GetTranslator(long id);

	std::vector<DL_HookPtr> GetHooks();

};


#endif