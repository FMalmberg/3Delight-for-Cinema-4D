#include "c4d.h"
#include "c4d_symbols.h"
#include "myres.h"
#include "3DelightRenderer.h"
#include <iostream>
#include "DL_Render.h"
#include "DL_SceneParser.h"
#include "nsi.hpp"
#include "IDs.h"
#include <assert.h> 
#include <algorithm>

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
			if (dc->_descId[0].id == DL_CREATE_RENDER_SETTINGS) //If render button is clicked
			{
				dldata->SetString(DL_ISCLICKED, "Render"_s);
				//Executes this command which will run the plugin with the ID_RENDERFRAME ID.
				CallCommand(ID_RENDERFRAME);
			}
			//If Export to NSI File button is clicked
			if (dc->_descId[0].id == DL_EXPORT_RENDER_SETTINGS) 
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
				CallCommand(ID_RENDERFRAME);
			}
			break;
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

	dldata->SetInt32(DL_RESOLUTION, DL_HALF);
	dldata->SetInt32(DL_SAMPLING, DL_TEN_PERCENT);
	dldata->SetFloat(DL_PIXEL_ASPECT_RATIO, 0);
	dldata->RemoveData(DL_EIGHT_BIT);

	Filename fn = GeGetStartupPath();
	fn = fn + "\NSI";
	dldata->SetFilename(DL_DEFAULT_IMAGE_FILENAME, fn);


	return TRUE;
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

	return RegisterVideoPostPlugin(ID_RENDERSETTINGS, "3Delight Renderer"_s, PLUGINFLAG_VIDEOPOST_ISRENDERER, RenderSettings::Alloc, "myres"_s, 0, 0);
}

