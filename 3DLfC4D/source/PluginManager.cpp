#include "PluginManager.h"
#include "c4d.h"
using namespace std;

class HookDeleter{
public:
	void operator()(DL_Hook* hook){
		if(hook){
			hook->Free();
		}
	}
};

PluginManager::~PluginManager(){

}

long PluginManager::GetAPIVersion(){
	return DL_API_VERSION;
}

void PluginManager::RegisterHook(HookAllocator allocator){
	HookAllocators.push_back(allocator);
}

bool PluginManager::IsLight(BaseList2D* item) {
	if (!item) return false;
	return LightTypes.find(item->GetType()) != LightTypes.end();
}

void PluginManager::RegisterTranslator(long id, TranslatorAllocator allocator, bool IsLight){
	Allocators[id]=allocator;
	if (IsLight) {
		LightTypes.insert(id);
	}
}

DL_Translator*  PluginManager::GetTranslator(long id){ 
	DL_Translator*  result=0;
	TranslatorMap::iterator it=Allocators.find(id);
	if(it!=Allocators.end()){
		result=it->second();
	}
	return result;
}


std::vector<DL_HookPtr> PluginManager::GetHooks(){ 
	long nhooks=HookAllocators.size();
	vector<DL_HookPtr> hooks(nhooks);

	HookDeleter d;
	for(long i=0; i<nhooks; i++){
		hooks[i]=DL_HookPtr(HookAllocators[i](),d);
	}
	return hooks;
}
