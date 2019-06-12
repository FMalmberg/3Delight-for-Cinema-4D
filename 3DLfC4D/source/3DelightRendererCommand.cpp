#include "c4d.h"
#include "IDs.h"
#include "nsi.hpp"
#include "SceneParser.h"
#include "3DelightRenderer.h"

class DL_3DelightRender_command : public CommandData
{

public:
	virtual Bool Execute(BaseDocument* doc);
};


void NSIErrorHandlerForC4D(void *userdata, int level, int code, const char *message)
{
	const char *pre = (const char *)userdata;
	std::string buffer("3Delight");

	if (pre)
	{
		buffer += " (" + std::string(pre) + "): ";
	}
	else
		buffer += ": ";

	buffer += message;

	switch (level)
	{
	case NSIErrMessage:
		ApplicationOutput(buffer.c_str());
		break;
	case NSIErrInfo:
		ApplicationOutput(buffer.c_str());
		break;
	case NSIErrWarning:
		ApplicationOutput(buffer.c_str());
		break;
	default:
	case NSIErrError:
		ApplicationOutput(buffer.c_str());
		break;
	}
}


Bool DL_3DelightRender_command::Execute(BaseDocument* doc)
{
	BaseDocument* renderdoc = (BaseDocument*)doc->GetClone(COPYFLAGS::DOCUMENT, nullptr);

	NSIErrorHandler_t eh = NSIErrorHandlerForC4D;
	NSIParam_t streamParam;
	streamParam.name = "errorhandler";
	streamParam.data = &eh;
	streamParam.type = NSITypePointer;
	streamParam.count = 1;
	streamParam.flags = 0;
	NSIContext_t context_handle = NSIBegin(1, &streamParam);

	NSI::Context context(context_handle);

	SceneParser sp(renderdoc, context_handle);

	sp.SetRenderMode(PREVIEW_RENDER);

	//Render scene
	BaseTime t = doc->GetTime();
	long frame = t.GetFrame(doc->GetFps());
	bool RenderOK = sp.InitScene(true, frame);
	sp.SampleFrameMotion();

	BaseDocument::Free(renderdoc);

	context.SetAttribute(NSI_SCENE_GLOBAL, (
		NSI::IntegerArg("renderatlowpriority", 1)
		));

	context.RenderControl((
		NSI::StringArg("action", "start")
		));
	return RenderOK;
	return true;
}


Bool Register3DelightCommand(void)
{
	return RegisterCommandPlugin(DL_RENDER_COMMAND, "3Delight Render"_s, PLUGINFLAG_HIDEPLUGINMENU, AutoBitmap("shelf_launchRender_200.png"_s), String("Render"_s), NewObjClear(DL_3DelightRender_command));
}

