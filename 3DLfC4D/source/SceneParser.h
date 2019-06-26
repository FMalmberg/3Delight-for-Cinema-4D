#ifndef SCENE_PARSER_H
#define SCENE_PARSER_H

#include "c4d.h"
#include "DL_SceneParser.h"
#include "PluginManager.h"
#include "DL_Hook.h"
#include "Node.h"
#include "Transform.h"
#include <vector>
#include <map>
#include <string>

/*enum ParserStage{
	NONE,
	BUILD_SCENE_TREE,
	CACHE_STATIC_DATA,
	EMIT_OPTIONS,
	EMIT_ATTRIBUTES,
	EMIT_LIGHTS,
	EMIT_SHADERS,
	EMIT_GEOMETRY
};*/


class SceneParser: public DL_SceneParser{
public:
	SceneParser();
	SceneParser(BaseDocument* document, NSIContext_t context);
	virtual ~SceneParser();
	NSIContext_t GetContext();

	//Old method, to be removed
	//bool Parse(BaseDocument* document, long frame);

	//New methods:
	bool InitScene(bool animate, long frame);
	void SampleFrameMotion(long frame);
	void InteractiveUpdate();

	void FillRenderSettings();

	//Old
	//void SetMotionSamples(int samples);
	//void SetShutter(double open, double close);
	//void SetAOVExport(bool export_aovs);

	/*float* GetMotionSampleTimes();
	int GetMotionSamples();
	float GetShutterOpen();
	float GetShutterClose();*/

	//void RequestMinimumShutterTime(float t);
	RENDER_MODE GetRenderMode();
	void SetRenderMode(RENDER_MODE mode);

	BaseContainer* GetSettings();

	//Return the base handle name of any c4d item. 
	//Primarily intended to be used for building connections in the NSI scene, e.g. in ConnectNSINodes()
	//Exception: Not guaranteed to return the correct name for virtual objects
	virtual const char* GetHandleName(BaseList2D* node);


private:
	void TraverseShaders(BaseShader* shader, BaseDocument* doc, std::vector<Node>& nodes);

	//void TraverseScene(BaseObject* obj, BaseDocument* doc,std::string parent_transform, bool visible=true);
	void TraverseObjects(BaseObject* obj, BaseDocument* doc, HierarchyData hdata, std::vector<Node>& nodes, std::vector<Transform>& transforms);

	void GetNodesAndTransforms(BaseDocument* doc, std::vector<Node>& nodes, std::vector<Transform>& transforms);
	//void FillNodeListRecursive(BaseObject* obj, BaseDocument* doc, std::string parent_transform, bool visible = true);

	//void SampleMotion(long s, BaseDocument* doc);
	void AnimateDoc(BaseDocument* doc,double t);

	//void UpdateMotionSampleTimes();
	
    //std::vector<float> GetSampleTimes(int nsamples);
	//std::string GetIDString(BaseList2D* node);

private:
	NSIContext_t context_handle;
	std::string handle; //Internal temporary storage for handle names requested via GetHandleName();

	//ParserStage stage;
	BaseContainer settings;
	BaseDocument* doc;

	RENDER_MODE rendermode;

	bool IsDirty(Node* n);

	std::map<std::string, Int64> dirtystates;
		
	//BaseContainer DL_settings;

	//std::vector<DL_HookPtr> hooks;
	//std::vector<Node> nodes;
	//std::vector<Transform> transforms;
	
	//To be removed:
	//std::map<std::string, std::string> HandleMap;

	int nMotionSamples;

	/*double shutterOpen;
	double shutterClose;
	double shutterTime;

	std::vector<float> motionSampleTimes;*/

	//std::map<std::string, unsigned long long > names;
	//std::string name;

	//long nshaders;
};


#endif