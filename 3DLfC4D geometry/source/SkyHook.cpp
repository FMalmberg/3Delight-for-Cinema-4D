#include "SkyHook.h"
#include "nsi.hpp"

using namespace std;

void SkyHook::Init(BaseDocument* doc, DL_SceneParser* parser) {
	sky = doc->GetHighest(Osky, false); //Find the first active sky object
	skyhandle = "3dlfc4d_geometry::environment";
}


void SkyHook::CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser) {

	if (!sky) { return; }

	NSI::Context ctx(parser->GetContext());

	ctx.Create(skyhandle, "environment");

	int isSky = 1;
	ctx.SetAttribute(skyhandle, 
		NSI::IntegerArg("IsSkyObject", isSky)
		);
}

void SkyHook::ConnectNSINodes(BaseDocument* doc, DL_SceneParser* parser) {
	NSI::Context ctx(parser->GetContext());
	if (!sky) { return; }
	string transform_handle = std::string("X_")+string(parser->GetHandleName(sky));
	ctx.Connect(skyhandle, "", transform_handle, "objects");
}


