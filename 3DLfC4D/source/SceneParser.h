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

enum ParserStage{
	NONE,
	BUILD_SCENE_TREE,
	CACHE_STATIC_DATA,
	EMIT_OPTIONS,
	EMIT_ATTRIBUTES,
	EMIT_LIGHTS,
	EMIT_SHADERS,
	EMIT_GEOMETRY
};


class SceneParser: public DL_SceneParser{
public:
	SceneParser();
	virtual ~SceneParser();
	NSIContext_t GetContext();
	bool Parse(BaseDocument* doc, long frame);
	//void Initialize(BaseDocument* doc, NSIContext_t context_handle, long frame, RENDER_MODE mode, BaseContainer render_settings);
	//void SampleFrameInterval();
	//void InteractiveUpdate();

	//Old
	void SetMotionSamples(int samples);
	void SetShutter(double open, double close);
	void SetAOVExport(bool export_aovs);

	float* GetMotionSampleTimes();
	int GetMotionSamples();
	float GetShutterOpen();
	float GetShutterClose();

	void RequestMinimumShutterTime(float t);
	RENDER_MODE GetRenderMode();
	void SetRenderMode(RENDER_MODE mode);

	BaseContainer* GetSettings();

	virtual const char* GetUniqueName(char* basename);
	virtual void SetAssociatedHandle(BaseList2D* node, const char* handle);
	virtual const char* GetAssociatedHandle(BaseList2D* node);

private:
	void TraverseShaders(BaseShader* shader, BaseDocument* doc);

	void TraverseScene(BaseObject* obj, BaseDocument* doc,std::string parent_transform, bool visible=true);
	void SampleMotion(long s, BaseDocument* doc);
	void AnimateDoc(BaseDocument* doc,double t);
	void UpdateMotionSampleTimes();
	
	std::vector<float> GetSampleTimes(int nsamples);
	std::string GetIDString(BaseList2D* node);

private:
	NSIContext_t context_handle;
	ParserStage stage;
	BaseContainer* settings;

	RENDER_MODE rendermode;
	BaseContainer DL_settings;

	BaseDocument* render_doc;

	bool ExportAOVs;


	std::vector<DL_HookPtr> hooks;
	std::vector<Node> nodes;
	std::vector<Transform> transforms;
	

	std::map<std::string, std::string> HandleMap;

	int nMotionSamples;

	double shutterOpen;
	double shutterClose;
	double shutterTime;

	std::vector<float> motionSampleTimes;

	std::map<std::string, unsigned long long > names;
	std::string name;

	long nshaders;
};


#endif