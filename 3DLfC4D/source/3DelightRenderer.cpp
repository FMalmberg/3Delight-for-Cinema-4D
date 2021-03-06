#include "c4d.h"
#include "c4d_symbols.h"
#include "dlrendersettings.h"
#include "3DelightRenderer.h"
#include <iostream>
#include "DL_Render.h"
#include "DL_SceneParser.h"
#include "nsi.hpp"
#include "IDs.h"
#include <assert.h> 
#include <algorithm>
#include "SceneParser.h"
#include <customgui_inexclude.h>


void NSIErrorHandlerC4D(void *userdata, int level, int code, const char *message)
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



Bool AddCycleButton(Description *dc, Int32 id, const DescID &groupid, const String &name, const BaseContainer &itemnames)
{
	const DescID* singleid = dc->GetSingleDescID();

	if (!singleid || ((DescID)id).IsPartOf(*singleid, NULL))
	{
		BaseContainer bc = GetCustomDataTypeDefault(DTYPE_LONG);

		//  Set CycleButton properties
		bc.SetBool(DESC_ANIMATE, false);
		bc.SetInt32(DESC_CUSTOMGUI, CUSTOMGUI_CYCLEBUTTON);
		bc.SetInt32(DESC_DEFAULT, 0);
		bc.SetInt32(DESC_SCALEH, TRUE);
		bc.SetString(DESC_NAME, name);
		bc.SetContainer(DESC_CYCLE, itemnames);

		//  Create CycleButton
		return dc->SetParameter(DescLevel(id, DTYPE_LONG, 0), bc, groupid);
	}

	return TRUE;
}


Bool AddEmpty(Description *dc, Int32 id, const DescID &groupid)
{
	const DescID* singleid = dc->GetSingleDescID();

	if (!singleid || ((DescID)id).IsPartOf(*singleid, NULL))
	{
		BaseContainer bc = GetCustomDataTypeDefault(DTYPE_STATICTEXT);

		bc.SetBool(DESC_ANIMATE, false);
		bc.SetInt32(DESC_SCALEH, TRUE);
		return dc->SetParameter(DescLevel(id, DTYPE_STATICTEXT, 0), bc, groupid);
	}

	return TRUE;
}



bool Render3Dl(BaseDocument* doc, long frame, RENDER_MODE mode, bool progressive, BaseContainer* settings) {
	BaseDocument* renderdoc = (BaseDocument*)doc->GetClone(COPYFLAGS::DOCUMENT, nullptr);

	NSIContext_t context_handle = NSIBegin(0, 0);
	String action = settings->GetString(DL_ISCLICKED);
	if (action == "Render")
	{
		NSIErrorHandler_t eh = NSIErrorHandlerC4D;
		NSIParam_t streamParam;
		streamParam.name = "errorhandler";
		streamParam.data = &eh;
		streamParam.type = NSITypePointer;
		streamParam.count = 1;
		streamParam.flags = 0;
		context_handle = NSIBegin(1, &streamParam);
	}
	else if (action == "Export")
	{
		String file = settings->GetFilename(DL_FOLDER_OUTPUT).GetString();
		std::string exported = file.GetCStringCopy();
		NSIParam_t streamParam;
		const char *output = exported.c_str();
		streamParam.name = "streamfilename";
		streamParam.data = &output;
		streamParam.type = NSITypeString;

		context_handle = NSIBegin(1, &streamParam);
	}

	else if (action == "Cloud")
	{
		int one = 1;
		NSIParam_t cloud[3];
		cloud[0].name = "cloud"; cloud[0].data = &one;
		cloud[0].type = NSITypeInteger; cloud[0].count = 1;
		cloud[0].flags = 0; cloud[0].arraylength = 0;

		NSIErrorHandler_t eh = NSIErrorHandlerC4D;
		cloud[1].name = "errorhandler";
		cloud[1].data = &eh;
		cloud[1].type = NSITypePointer; cloud[1].count = 1;
		cloud[1].flags = 0; cloud[1].arraylength = 0;

		context_handle = NSIBegin(1, cloud);
	}

	NSI::Context context(context_handle);

	SceneParser sp(renderdoc, context_handle);

	sp.SetRenderMode(mode);

	//Render scene
	bool RenderOK = sp.InitScene(true, frame);
	sp.SampleFrameMotion(frame);

	BaseDocument::Free(renderdoc);

	context.SetAttribute(NSI_SCENE_GLOBAL, (
		NSI::IntegerArg("renderatlowpriority", 1)
		));

	context.RenderControl((
		NSI::StringArg("action", "start")
		));
	return RenderOK;
}

/**
	Function to hide a specific element from the RenderSettings GUI
*/
Bool ShowDescriptionElement(GeListNode *i_node, Description *i_descr, Int32 i_MyDescID, Bool i_show)
{
	AutoAlloc<AtomArray> ar;
	ar->Append(static_cast<C4DAtom*>(i_node));
	BaseContainer *bc = i_descr->GetParameterI(DescLevel(i_MyDescID), ar);

	if (!bc)
	{
		return FALSE;
	}

	bc->SetBool(DESC_HIDE, !i_show);

	return TRUE;
}

/**
	Gets the currently used parameter description of the entity
	which in this case is the plugin with Id ID_RENDERSETTINGS.
*/
Bool RenderSettings::GetDDescription(
	GeListNode *i_node,
	Description *i_description, DESCFLAGS_DESC &i_flags)
{
	i_description->LoadDescription(ID_RENDERSETTINGS);
	i_flags |= DESCFLAGS_DESC::LOADED;

	BaseContainer names;
	names.SetString(0, "Render"_s);
	names.SetString(1, "Export to NSI File..."_s);
	names.SetString(2, "Render in Cloud"_s);

	AddCycleButton(i_description, RENDER_CYCLEBUTTON, DescID(RENDER), "", names);
	AddEmpty(i_description, RENDER_CYCLEBUTTON+1, DescID(RENDER));
	AddEmpty(i_description, RENDER_CYCLEBUTTON+2, DescID(RENDER));

	BaseContainer *bc = ((BaseTag*)i_node)->GetDataInstance();

	Bool OPEN_EXR_FORMAT =
		(bc->GetInt32(DL_DEFAULT_IMAGE_FORMAT) == DL_FORMAT_OPEN_EXR ||
			bc->GetInt32(DL_DEFAULT_IMAGE_FORMAT) == DL_FORMAT_OPEN_EXR_DEEP);

	Bool TIFF_FORMAT = bc->GetInt32(DL_DEFAULT_IMAGE_FORMAT) == DL_FORMAT_TIFF;
	Bool JPG_FORMAT = bc->GetInt32(DL_DEFAULT_IMAGE_FORMAT) == DL_FORMAT_JPEG;
	Bool PNG_FORMAT = bc->GetInt32(DL_DEFAULT_IMAGE_FORMAT) == DL_FORMAT_PNG;

	/**
		Based on the selected Image format we use ShowDescriptionElement function
		which is implemented above to hide the dropdowns that does not correspond
		with the selected format, and show just the one which represents the selected image format.
		A description contains information about the parameters of a node object.
	*/
	if (OPEN_EXR_FORMAT)
	{
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT, OPEN_EXR_FORMAT);
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT_TIFF, !OPEN_EXR_FORMAT);
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT_PNG, !OPEN_EXR_FORMAT);
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT_JPG, !OPEN_EXR_FORMAT);
		bc->SetInt32(DL_DEFAULT_IMAGE_BITDEPTH, bc->GetInt32(DL_DEFAULT_IMAGE_OUTPUT));
	}
	else if (TIFF_FORMAT)
	{
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT, !TIFF_FORMAT);
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT_TIFF, TIFF_FORMAT);
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT_PNG, !TIFF_FORMAT);
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT_JPG, !TIFF_FORMAT);
		bc->SetInt32(DL_DEFAULT_IMAGE_BITDEPTH, bc->GetInt32(DL_DEFAULT_IMAGE_OUTPUT_TIFF));

	}
	else if (JPG_FORMAT)
	{
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT, !JPG_FORMAT);
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT_TIFF, !JPG_FORMAT);
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT_PNG, !JPG_FORMAT);
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT_JPG, JPG_FORMAT);
		bc->SetInt32(DL_DEFAULT_IMAGE_BITDEPTH, bc->GetInt32(DL_DEFAULT_IMAGE_OUTPUT_JPG));

	}
	else if (PNG_FORMAT)
	{
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT, !PNG_FORMAT);
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT_TIFF, !PNG_FORMAT);
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT_PNG, PNG_FORMAT);
		ShowDescriptionElement(i_node, i_description, DL_DEFAULT_IMAGE_OUTPUT_JPG, !PNG_FORMAT);
		bc->SetInt32(DL_DEFAULT_IMAGE_BITDEPTH, bc->GetInt32(DL_DEFAULT_IMAGE_OUTPUT_PNG));
	}

	return TRUE;
}



/**
	Overriden function which is called to decide which description parameters
	should be enabled or disabled after a specific action. Code is still just for testing
*/
Bool RenderSettings::GetDEnabling(
	GeListNode* i_node,
	const DescID& i_id,
	const GeData& i_data,
	DESCFLAGS_ENABLE i_flags,
	const BaseContainer* i_itemdesc)
{
	//Will implement this function later
	return true;
}


/**
	This Overridden function is called when a node receives messages. e.g when a button is clicked
	or a checkbox is checked etc. In this case we check if the user wants to render the scene or just
	export it to .nsi file and according to users choice we do the required implementation.
*/
Bool RenderSettings::Message(GeListNode* i_node, Int32 i_type, void* i_data)
{
	switch (i_type)
	{
	case MSG_DESCRIPTION_COMMAND:
	{
		DescriptionCommand* dc = (DescriptionCommand*)i_data;
		BaseContainer* dldata = ((BaseVideoPost*)i_node)->GetDataInstance();
		BaseDocument* doc = GetActiveDocument();
		BaseTime t = doc->GetTime();
		if (dc->_descId[0].id == RENDER_CYCLEBUTTON)
		{
			Int32 action = dldata->GetInt32(RENDER_CYCLEBUTTON);
			long frame = t.GetFrame(doc->GetFps());
			if (action==0) //If render button is clicked
			{
				dldata->SetString(DL_ISCLICKED, "Render"_s);
				Render3Dl(doc, frame, PREVIEW_RENDER, true, dldata);
			}
			//If Export to NSI File button is clicked
			else if (action==1)
			{
				dldata->SetString(DL_ISCLICKED, "Export"_s);
				if (dldata->GetFilename(DL_FOLDER_OUTPUT).GetString() == "")
				{
					Filename folder;
					String directory = GeGetStartupApplication().GetString() + "/NSI";
					folder.SetDirectory(directory);
					folder.SetSuffix("nsi"_s);
					folder.FileSelect(FILESELECTTYPE::ANYTHING, FILESELECT::SAVE, "Select Folder"_s);
					dldata->SetFilename(DL_FOLDER_OUTPUT, folder);
				}
				Render3Dl(doc, frame, PREVIEW_RENDER, true, dldata);
			}

			else if (action == 2)
			{
				dldata->SetString(DL_ISCLICKED, "Cloud"_s);
				Render3Dl(doc, frame, PREVIEW_RENDER, true, dldata);
			}

			
		}
		
	}
	}
	return true;
}

Bool RenderSettings::Init(GeListNode *i_node)
{
	BaseContainer* dldata = ((BaseVideoPost*)i_node)->GetDataInstance();
	dldata->SetBool(DL_TEXT_CHECKER, false);
	dldata->SetInt32(DL_SHADING_SAMPLES, 64);
	dldata->SetInt32(DL_PIXEL_SAMPLES, 8);
	dldata->SetInt32(DL_VOLUME_SAMPLES, 16);
	dldata->SetInt32(DL_PIXEL_FILTER, DL_BLACKMAN_HARRIS);
	dldata->SetFloat(DL_FILTER_WIDTH, 2.000);
	dldata->SetBool(DL_MOTION_BLUR, true);
	dldata->SetInt32(DL_MAX_DIFFUSE_DEPTH, 2);
	dldata->SetInt32(DL_MAX_REFLECTION_DEPTH, 2);
	dldata->SetInt32(DL_MAX_REFRACTION_DEPTH, 4);
	dldata->SetInt32(DL_MAX_HAIR_DEPTH, 5);
	dldata->SetFloat(DL_MAX_DISTANCE, 1000);

	dldata->SetInt32(DL_CAMERA_VIEW, DL_CAMERA_PERPSHAPE);
	dldata->SetInt32(DL_ENVIRONMENT_VIEW, DL_ENVIRONMENT_NONE);
	dldata->SetInt32(DL_ATMOSPHERE_VIEW, DL_ATMOSPHERE_NONE);
	dldata->SetInt32(DL_OBJECTS_TO_RENDER_VIEW, DL_OBJECTS_TO_RENDER_ALL);
	dldata->SetInt32(DL_LIGHTS_TO_RENDER_VIEW, DL_LIGHTS_TO_RENDER_ALL);

	dldata->SetBool(DL_FRAME_RANGE_CHECK, true);
	dldata->SetFloat(DL_FRAME_RANGE, 1.000);
	dldata->SetFloat(DL_FRAME_RANGE_MAX, 1.000);
	dldata->SetFloat(DL_FRAME_INCREMENT, 1.000);

	dldata->SetBool(DL_USE_RENDER_IMAGE_SIZE, true);
	dldata->SetFloat(DL_CROP_MIN_FROM, 0.000);
	dldata->SetFloat(DL_CROP_MIN_TO, 0.000);
	dldata->SetFloat(DL_CROP_MAX_FROM, 1.000);
	dldata->SetFloat(DL_CROP_MAX_TO, 1.000);

	dldata->SetInt32(DL_DEFAULT_IMAGE_FORMAT, DL_FORMAT_OPEN_EXR);
	dldata->SetInt32(DL_DEFAULT_IMAGE_OUTPUT, DL_SIXTEEN_BIT_FLOAT);
	dldata->SetInt32(DL_DEFAULT_IMAGE_BITDEPTH, DL_SIXTEEN_BIT_FLOAT);

	dldata->SetInt32(DL_DEFAULT_IMAGE_OUTPUT_TIFF, DL_THIRTYTWO_BIT);
	dldata->SetInt32(DL_DEFAULT_IMAGE_OUTPUT_JPG, DL_EIGHT_BIT);
	dldata->SetInt32(DL_DEFAULT_IMAGE_OUTPUT_PNG, DL_EIGHT_BIT);
	dldata->SetInt32(DL_BATCH_OUTPUT_MODE, DL_ENABLE_AS_SELECTED);
	dldata->SetInt32(DL_INTERACTIVE_OUTPUT_MODE, DL_ENABLE_AS_SELECTED_INTERACTIVE);


	Filename file = GeGetPluginPath() + Filename("NSI");
	dldata->SetFilename(DL_DEFAULT_IMAGE_FILENAME, file);

	dldata->SetInt32(DL_RESOLUTION, DL_HALF);
	dldata->SetInt32(DL_SAMPLING, DL_TEN_PERCENT);
	dldata->SetFloat(DL_PIXEL_ASPECT_RATIO, 0);

	dldata->RemoveData(DL_EIGHT_BIT);

	return TRUE;
}

RENDERRESULT RenderSettings::Execute(BaseVideoPost * node, VideoPostStruct * vps) 
{
	if (vps->vp == VIDEOPOSTCALL::RENDER && !vps->open) 
	{

		VolumeData* vd = vps->vd;
		if (!vd) return RENDERRESULT::OK;

		ApplicationOutput("Rendering with 3delight");

		VPBuffer * buffer = vps->render->GetBuffer(VPBUFFER_RGBA, NOTOK); //Get the default image buffer. We will render to this buffer.
		long width = buffer->GetBw();
		long height = buffer->GetBh();
		ApplicationOutput("W: " + String::IntToString(width));
		ApplicationOutput("H: " + String::IntToString(height));
		Int32 bit_depth = buffer->GetInfo(VPGETINFO::BITDEPTH);

		MultipassBitmap* im = (MultipassBitmap*)buffer; //Cast VPbuffer to MultiPassBitmap is safe, they are the same type internally
		im->Clear(255, 0, 0); //Set all pixels to red, just to demonstrate that we can actually manipulate the buffer

		BaseDocument* doc = vps->doc; //Document to be rendered
		//...Do rendering with 3delight here, following what we do in the RenderFrame command. //Donee
		//...We need to add a special display driver, that writes the rendered image data from 3delight to "RGBA_buffer"




		//vd->SkipRenderProcess(); 
		//Skip normal c4d rendering process, since we do all rendering ourselves using 3delight
		//return RENDERRESULT::USERBREAK;
	}
	return RENDERRESULT::OK;
}


Bool Register3DelightPlugin(void)
{
	//When we register the plugin we also register the icons for the aov layers 
	RegisterIcon(DL_DISPLAY_ON, GeGetPluginResourcePath() + "display-on.png");
	RegisterIcon(DL_DISPLAY_OFF, GeGetPluginResourcePath() + "display.png");
	RegisterIcon(DL_FOLDER_ON, GeGetPluginResourcePath() + "folder-on.png");
	RegisterIcon(DL_FOLDER_OFF, GeGetPluginResourcePath() + "folder.png");
	RegisterIcon(DL_JPG_ON, GeGetPluginResourcePath() + "jpg-on.png");
	RegisterIcon(DL_JPG_OFF, GeGetPluginResourcePath() + "jpg.png");

	return RegisterVideoPostPlugin(ID_RENDERSETTINGS, "3Delight"_s, PLUGINFLAG_VIDEOPOST_ISRENDERER, RenderSettings::Alloc, "dlrendersettings"_s, 0, 0);
	//PLUGINFLAG_VIDEOPOST_ISRENDERER
}
