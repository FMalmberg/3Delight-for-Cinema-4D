#include "RenderOptionsHook.h"
#include "IDs.h"
#include "dlrendersettings.h"
#include "nsi.hpp"
#include <algorithm>
#include <iostream>
#include <Windows.h>
#include "maxon/utilities/undef_win_macros.h"

#include <string.h>
#include <tuple>
#include <assert.h>
#include "DL_TypeConversions.h"


#define ID_SDK_EXAMPLE_CUSTOMGUI_STRING 1034666
#define ID_CUSTOMDATATYPE_LAYERS 1245668
#define ID_CUSTOMGUI_MULTILIGHTS 10346689
#define ID_CUSTOMDATATYPE_MULTILIGHTS 12456689

/* 
	Function to get the selected lights from the multilights UI.
*/
vector<BaseObject*> getSelectedLights(BaseDocument* doc,vector<Int32> objectID,Int32 all_multi_lights)
{
	vector<BaseObject*> light_list;

	BaseObject* object= doc->GetFirstObject();
	BaseObject* Sky = nullptr;
	if (!object)
		return light_list;
	Int32 skyUsed = 0;
	bool process = true;
	Int32 Position = all_multi_lights-1;

	for (int i = 0; i < objectID.size(); i++)
	{
		if (objectID[i] == 0)
		{
			skyUsed = 1;
		}
	}
	while (object)
	{
		if (process &&( object->GetType() == Olight || object->GetType() == ID_LIGHTCARD || object->GetType() == ID_INCANDESCENCELIGHT))
		{
			for (int i = 0; i < objectID.size(); i++)
			{
				/*
					Finding the objects that are selected in the Multi_light UI
					objectID contains the ID of the selected objects in the UI and
					now we are getting the corresponing object for that ID
				*/
				if (objectID[i] == Position)
				{
					light_list.push_back(object);
				}
			}
			Position--;
		}

		if (process && object->GetType() == Osky)
		{
			Sky = object;
		}

		if (object->GetDown() && process)
		{
			object = object->GetDown();
			process = true;
		}

		else if (object->GetNext())
		{
			object = object->GetNext();
			process = true;
		}

		else if (object->GetUp())
		{
			object = object->GetUp();
			process = false;
		}
		else object = NULL;
	}

	if (skyUsed == 1)
	{
		ApplicationOutput("Pusshed");
		light_list.push_back(Sky);
	}
	return light_list;
}

struct DL_LayersData
{
	Int32 id;
	Char	name[50];
	Char    varName[30];
	Char    varSource[30];
};

DL_LayersData s_shading_components[] =
{
	{ 50,	"RGBA (color + alpha)",      "Ci",				"shader"},
	{ 51,	"Diffuse" ,				     "diffuse",			"shader"},
	{ 52,	"Subsurface scattering",     "subsurface",		"shader"},
	{ 53,	"Reflection",				 "reflection",		"shader"},
	{ 54,	"Refraction",				 "refraction",		"shader"},
	{ 55,	"Incandescence",			 "incandescence",	"shader"},
	{ 0,    "" }
};


DL_LayersData s_auxiliary_variables[] =
{
	{ 100,	 "Z (depth)" ,			     "z",						"builtin"},
	{ 101,   "Camera space position",	 "P.camera",				"builtin"},
	{ 102,   "Camera space normal",		 "N.camera",				"builtin"},
	{ 103,   "UV",						 "uv",						"builtin"},
	{ 104,   "Geometry ID" ,			 "id.geometry",				"builtin"},
	{ 105,   "Scene Path ID",			 "id.scenepath",			"builtin"},
	{ 106,   "Relighting Multiplier",	 "relighting_multiplier",	"shader" },
	{ 0,     "" }
};

//Get the variablename to be used on nsi for the selected Layer
string getVariableName(String i_layer) 
{
	string selected_layer;

	for (int i = 0; s_shading_components[i].id; i++)
	{
		if (i_layer == String(s_shading_components[i].name))
			selected_layer = s_shading_components[i].varName;
	}
			
	for (int i = 0; s_auxiliary_variables[i].id; i++)
	{
		if (i_layer == String(s_auxiliary_variables[i].name))
			selected_layer = s_auxiliary_variables[i].varName;
	}
	assert(selected_layer != "");
	return selected_layer;
}

//Get the variablesource to be used on nsi for the selected Layer
string getVariableSource(String i_layer)
{
	string variable_source;

	for (int i = 0; s_shading_components[i].id; i++)
	{
		if (i_layer == String(s_shading_components[i].name))
			variable_source = s_shading_components[i].varSource;
	}

	for (int i = 0; s_auxiliary_variables[i].id; i++)
	{
		if (i_layer == String(s_auxiliary_variables[i].name))
			variable_source = s_auxiliary_variables[i].varSource;
	}
	assert(variable_source != "");
	return variable_source;
}

//Get the Filter to be used on nsi for the selectedFilter on the render settings
std::string getFilter(Int32 i_id) 
{
	std::string filter;
	switch (i_id)
	{
	case DL_BLACKMAN_HARRIS:
		filter = "blackman-harris"; break;
	case DL_MITCHELL:
		filter = "mitchell"; break;
	case DL_CATMULL_ROM:
		filter = "catmull-rom"; break;
	case DL_SINC:
		filter = "sinc"; break;
	case DL_BOX:
		filter = "box"; break;
	case DL_TRIANGLE:
		filter = "triangle"; break;
	case DL_GAUSSIAN:
		filter = "gaussian"; break;

	default:
		filter = "blackman-harris"; break;
		break;
	}
	assert(filter != "");

	return filter;
}

std::tuple<string,string,string> OutputLayer(DL_SceneParser* i_parser) 
{
	BaseContainer* settings = i_parser->GetSettings();
	string extension;
	string output_format;
	string scalar_format;

	if (settings->GetInt32(DL_DEFAULT_IMAGE_FORMAT) == DL_FORMAT_OPEN_EXR ||
		settings->GetInt32(DL_DEFAULT_IMAGE_FORMAT) == DL_FORMAT_OPEN_EXR_DEEP)
	{
		extension = ".exr";
		if (settings->GetInt32(DL_DEFAULT_IMAGE_FORMAT) == DL_FORMAT_OPEN_EXR)
			output_format = "exr";

		else
			output_format = "deepexr";

		if (settings->GetInt32(DL_DEFAULT_IMAGE_BITDEPTH) == DL_SIXTEEN_BIT_FLOAT)
			scalar_format = "half";

		else if (settings->GetInt32(DL_DEFAULT_IMAGE_BITDEPTH) == DL_THIRTYTWO_BIT)
			scalar_format = "float";
	}

	else if (settings->GetInt32(DL_DEFAULT_IMAGE_FORMAT) == DL_FORMAT_TIFF)
	{
		extension = ".tif";
		output_format = "tiff";

		if (settings->GetInt32(DL_DEFAULT_IMAGE_BITDEPTH) == DL_EIGHT_BIT)
			scalar_format = "uint8";

		else if (settings->GetInt32(DL_DEFAULT_IMAGE_BITDEPTH) == DL_SIXTEEN_BIT)
			scalar_format = "uint16";

		else if (settings->GetInt32(DL_DEFAULT_IMAGE_BITDEPTH) == DL_THIRTYTWO_BIT)
			scalar_format = "float";
	}

	else if (settings->GetInt32(DL_DEFAULT_IMAGE_FORMAT) == DL_FORMAT_PNG)
	{
		extension = ".png";
		output_format = "png";

		if (settings->GetInt32(DL_DEFAULT_IMAGE_BITDEPTH) == DL_EIGHT_BIT)
			scalar_format = "uint8";

		else if (settings->GetInt32(DL_DEFAULT_IMAGE_BITDEPTH) == DL_SIXTEEN_BIT)
			scalar_format = "uint16";
	}

	else if (settings->GetInt32(DL_DEFAULT_IMAGE_FORMAT) == DL_FORMAT_JPEG)
	{
		extension = ".jpg";
		output_format = "jpeg";
		scalar_format = "uint8";
	}
		
	assert( extension != "" && output_format != "" && scalar_format != "" ); 
	return std::make_tuple(extension, output_format, scalar_format);
}


/**
	This function is executed each time we render a scene on 3Delight
	Here we get the information from the render settings and according to
	the retrieved information we pass these values with on nsi based on its criteria.
*/
void RenderOptionsHook::CreateNSINodes(BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());
	BaseContainer* settings= parser->GetSettings();
	
	const CustomDataType* dt = settings->
		GetCustomDataType(DL_CUSTOM_AOV_LAYER, ID_CUSTOMDATATYPE_LAYERS);
	const CustomDataType* multi_light_dt = settings->
		GetCustomDataType(DL_CUSTOM_MULTI_LIGHT, ID_CUSTOMDATATYPE_MULTILIGHTS);

	iCustomDataTypeLayers* data = (iCustomDataTypeLayers*)(dt);
	iCustomDataTypeLights* multi_light_data = (iCustomDataTypeLights*)(multi_light_dt);

	if (!data)
	{ 
		data = NewObjClear(iCustomDataTypeLayers);
		data->m_selected_layers.Append(String("RGBA (color + alpha)"));
		data->m_selected_id.Append(50);
		data->m_output_to_framebuffer.Append(TRUE);
		data->m_output_to_file.Append(TRUE);
		data->m_output_to_jpeg.Append(FALSE);		
	}

	if (!multi_light_data)
	{
		multi_light_data = NewObjClear(iCustomDataTypeLights);
	}

	double samplingReduceFactor = 1;
	bool use_displacement = 1;
	bool use_subsurface = 1;
	if (settings->GetBool(DL_ENABLE_INTERACTIVE_PREVIEW) == true)
	{
		int sampling = settings->GetInt32(DL_SAMPLING);
		if (sampling == DL_ONE_PERCENT) 
			samplingReduceFactor = 0.01;
		else if (sampling == DL_FOUR_PERCENT) 
			samplingReduceFactor = 0.04;
		else if (sampling == DL_TEN_PERCENT) 
			samplingReduceFactor = 0.10;
		else if (sampling == DL_TWENTYFIVE_PERCENT) 
			samplingReduceFactor = 0.25;
		else if (sampling == DL_HUNDRED_PERCENT) 
			samplingReduceFactor = 1;

		use_displacement = !settings->GetBool(DL_DISABLE_DISPLACEMENT);
		use_subsurface = !settings->GetBool(DL_DISABLE_SUBSURFACE);
	}

	
	NSI::Context context(parser->GetContext());
	
	int shading_samples = max(1, (int)(settings->GetInt32(DL_SHADING_SAMPLES)
		*samplingReduceFactor + 0.5));

	int volume_samples = max(1, (int)(settings->GetInt32(DL_VOLUME_SAMPLES)
		*samplingReduceFactor + 0.5));

	int diffuse_depth = settings->GetInt32(DL_MAX_DIFFUSE_DEPTH);
	int reflection_depth = settings->GetInt32(DL_MAX_REFLECTION_DEPTH);
	int refraction_depth = settings->GetInt32(DL_MAX_REFRACTION_DEPTH);
	int hair_depth = settings->GetInt32(DL_MAX_HAIR_DEPTH);
	double max_distance = settings->GetFloat(DL_MAX_DISTANCE);

	string docName = doc->GetDocumentName().GetString().GetCStringCopy();
	
	int pos = -1;
	pos = docName.find(".");

	if(pos!=-1)
	docName = docName.substr(0, pos);
	//ApplicationOutput("Document @", docName.c_str());

	Filename fn = settings->GetFilename(DL_DEFAULT_IMAGE_FILENAME);
	if (!fn.IsPopulated())
		fn = GeGetPluginPath();
	std::string dir = fn.GetString().GetCStringCopy();
	dir = dir + "\\" + docName;


	ctx.SetAttribute(NSI_SCENE_GLOBAL,(
		NSI::IntegerArg("quality.shadingsamples",shading_samples),
		NSI::IntegerArg("quality.volumesamples", volume_samples),
		NSI::IntegerArg("maximumraydepth.diffuse", diffuse_depth),
		NSI::IntegerArg("maximumraydepth.reflection", reflection_depth),
		NSI::IntegerArg("maximumraydepth.refraction", refraction_depth),
		NSI::IntegerArg("maximumraydepth.hair", hair_depth)
		));


	ctx.SetAttribute(NSI_SCENE_GLOBAL, (
		NSI::DoubleArg("maximumraylength.specular", max_distance),
		NSI::DoubleArg("maximumraylength.diffuse", max_distance),
		NSI::DoubleArg("maximumraylength.reflection", max_distance),
		NSI::DoubleArg("maximumraylength.refraction", max_distance),
		NSI::DoubleArg("maximumraylength.hair", max_distance)
		));
	
	ctx.SetAttribute(NSI_SCENE_GLOBAL, (
		NSI::IntegerArg("show.displacement", use_displacement),
		NSI::IntegerArg("show.osl.subsurface", use_subsurface)
		));
	
	vector<Int32> selected_lights_guid;
	for (int i = 0; i < multi_light_data->m_selected_lights_itemID.GetCount(); i++)
		selected_lights_guid.push_back(multi_light_data->m_selected_lights_itemID[i]);

	//Getting selected objects
	ApplicationOutput("Count @", selected_lights_guid.size());
	vector<BaseObject*> selected_lights = getSelectedLights(doc, selected_lights_guid, multi_light_data->m_all_multi_lights.GetCount());
	//This returns 0 because the GUID provided by the aov layer differs from the GUI of the object here.

	string filter_output = getFilter(settings->GetInt32(DL_PIXEL_FILTER));
	int m_layer_number = data->m_selected_layers.GetCount();
	int m_multi_lights = multi_light_data->m_multi_light_selected_layers.GetCount();
	int sortkey = 0;

	auto layeroutput = OutputLayer(parser);
	m_extension = get<0>(layeroutput);
	m_output_format = get<1>(layeroutput);
	m_scalar_format = get<2>(layeroutput);

	m_display_driver_handle = "displayDriver";
	ctx.Create(m_display_driver_handle, "outputdriver");
	ctx.SetAttribute(m_display_driver_handle, (
		NSI::StringArg("drivername", "idisplay"),
		NSI::StringArg("imagefilename", "Display")
		));

    fn = settings->GetFilename(DL_DEFAULT_IMAGE_FILENAME);
	if (!fn.IsPopulated())
		fn = GeGetPluginPath();
	dir = fn.GetString().GetCStringCopy();

	std::string filename = dir + "\\filename" + m_extension;
	m_file_driver_handle = "fileDriver";
	ctx.Create(m_file_driver_handle, "outputdriver");

	ctx.SetAttribute(m_file_driver_handle, (
		NSI::StringArg("drivername", m_output_format.c_str()),
		NSI::StringArg("imagefilename", filename.c_str())
		));

	for (int i = 0; i < m_layer_number; i++)
	{
		m_aov = getVariableName(data->m_selected_layers[i]);
		m_variable_source = getVariableSource(data->m_selected_layers[i]);

		Bool ouputLayerImage = data->m_output_to_jpeg[i];
		Bool outputLayerFile = data->m_output_to_file[i];
		Bool ouputLayerDisplay = data->m_output_to_framebuffer[i];
		String nam = data->m_selected_layers[i];
		Char* name = nam.GetCStringCopy();
		string extenstion = ".jpg";
		string ouptut_image = dir + "\\" + name + extenstion;
		/**
			If the m_output_to_framebuffer icon is on then we
			connect the iDisplay driver with the layer that we are currently on
		*/
		if (ouputLayerDisplay)
		{
			//m_layer_handle = string(i_parser->GetUniqueName("DisplayLayer"));
			m_layer_handle = string("3dlfc4d::DisplayLayer") + std::to_string(i);
			ctx.Create(m_layer_handle, "outputlayer");

			ctx.SetAttribute(m_layer_handle, (
				NSI::StringArg("variablename", m_aov.c_str()),
				NSI::StringArg("layername", "display"),
				NSI::StringArg("layertype", "color"),
				NSI::StringArg("variablesource", m_variable_source.c_str()),
				NSI::StringArg("scalarformat", m_output_format.c_str()),
				NSI::IntegerArg("withalpha", 1),
				NSI::IntegerArg("sortkey", sortkey++),
				NSI::StringArg("filter", filter_output.c_str())
				));
			ctx.Connect(m_layer_handle, "", "3dlfc4d::scene_camera_screen", "outputlayers");
			ctx.Connect(m_display_driver_handle, "", m_layer_handle, "outputdrivers");
			
			//Outputing Multilights in the Rendering.
			for (int j = 0; j < selected_lights.size(); j++)
			{
				string multi_light_handle = string("3dlfc4d::multi_light" + std::to_string(sortkey));
				ctx.Create(multi_light_handle, "outputlayer");
				ctx.SetAttribute(multi_light_handle, (
					NSI::StringArg("variablename", m_aov.c_str()),
					NSI::StringArg("layertype", "color"),
					NSI::StringArg("scalarformat", m_output_format.c_str()),
					NSI::IntegerArg("withalpha", 1),
					NSI::StringArg("filter", filter_output.c_str()),
					NSI::IntegerArg("sortkey", sortkey++),
					NSI::StringArg("variablesource", m_variable_source.c_str())
					));

				ctx.SetAttribute(multi_light_handle,
					NSI::IntegerArg("drawoutlines", 1));
				ctx.Connect(multi_light_handle, "", "3dlfc4d::scene_camera_screen", "outputlayers");
				
					BaseList2D* passed_light = (BaseList2D*)selected_lights[j];
					if (passed_light) {
						string light_handle = string("X_") + parser->GetHandleName(passed_light);
						ctx.Connect(light_handle, "", multi_light_handle, "lightset");
						ctx.Connect(m_display_driver_handle, "", multi_light_handle, "outputdrivers");
					}
			}
		}

		/**
			If the m_output_to_file icon is on then we
			connect the selected output driver from render settings
			with the layer that we are currently on
		*/
		if (outputLayerFile)
		{
			//m_layer_file = string(i_parser->GetUniqueName("file_outputlayer"));
			m_layer_file = string("3dlfc4d::file_outputlayer") + std::to_string(i);
			ctx.Create(m_layer_file, "outputlayer");

			ctx.SetAttribute(m_layer_file, (
				NSI::StringArg("variablename", m_aov.c_str()),
				NSI::StringArg("layertype", "color"),
				NSI::StringArg("variablesource", m_variable_source.c_str()),
				NSI::StringArg("scalarformat", m_scalar_format.c_str()),
				NSI::IntegerArg("withalpha", 1),
				NSI::IntegerArg("sortkey", sortkey++),
				NSI::StringArg("filter", filter_output.c_str())
				));

			if (m_scalar_format == "uint8")
			{
				ctx.SetAttribute(m_layer_file, (
					NSI::StringArg("colorprofile", "srgb")
					));
			}
			ctx.Connect(m_layer_file, "", "3dlfc4d::scene_camera_screen", "outputlayers");
			ctx.Connect(m_file_driver_handle, "", m_layer_file, "outputdrivers");
		}

		/**
			If the m_output_to_jpg icon is on then we
			create a jpeg driver and connect each of them
			with their corresponding layer.
		*/
		if (ouputLayerImage)
		{
			//m_layer_jpg = string(i_parser->GetUniqueName("image_outputlayer"));
			m_layer_jpg = string("3dlfc4d::image_outputlayer") + std::to_string(i);
			ctx.Create(m_layer_jpg, "outputlayer");

			ctx.SetAttribute(m_layer_jpg, (
				NSI::StringArg("variablename", m_aov.c_str()),
				NSI::StringArg("layertype", "color"),
				NSI::StringArg("variablesource", m_variable_source.c_str()),
				NSI::StringArg("scalarformat", "uint8"),
				NSI::IntegerArg("withalpha", 1),
				NSI::IntegerArg("sortkey", sortkey++),
				NSI::StringArg("filter", filter_output.c_str()),
				NSI::StringArg("colorprofile", "srgb")
				));


			//m_output_driver_handle = string(i_parser->GetUniqueName("driver"));
			m_output_driver_handle = string("3dlfc4d::jpg_driver") + std::to_string(i);
			ctx.Create(m_output_driver_handle, "outputdriver");

			ctx.SetAttribute(m_output_driver_handle, (
				NSI::StringArg("drivername", "jpeg"),
				NSI::StringArg("imagefilename", ouptut_image.c_str())
				));

			ctx.Connect(m_output_driver_handle, "", m_layer_jpg, "outputdrivers");
			ctx.Connect(m_layer_jpg, "", "3dlfc4d::scene_camera_screen", "outputlayers");
		}
	}
}

void RenderOptionsHook::ConnectNSINodes(BaseDocument* doc, DL_SceneParser* parser)
{

	NSI::Context ctx(parser->GetContext());
	BaseContainer* settings = parser->GetSettings();
}
