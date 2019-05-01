#include "RenderOptionsHook.h"
#include "IDs.h"
#include "myres.h"
#include "nsi.hpp"
#include <algorithm>
#include <iostream>
#include <Windows.h>
#include "maxon/utilities/undef_win_macros.h"

#include <string.h>
#include <assert.h>
#include "DL_TypeConversions.h"


#define ID_SDK_EXAMPLE_CUSTOMGUI_STRING 1034666
#define ID_CUSTOMDATATYPE_LAYERS 1245668


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
void RenderOptionsHook::CreateNSINodes(BaseDocument* i_doc, DL_SceneParser* i_parser)
{
	NSI::Context ctx(i_parser->GetContext());
	BaseContainer* settings= i_parser->GetSettings();
	
	const CustomDataType* dt = settings->
		GetCustomDataType(DL_CUSTOM_AOV_LAYER, ID_CUSTOMDATATYPE_LAYERS);

	iCustomDataTypeLayers* data = (iCustomDataTypeLayers*)(dt);

	if (!data)
	{ 
		data = NewObjClear(iCustomDataTypeLayers);
		data->m_selected_layers.Append(String("RGBA (color + alpha)"));
		data->m_selected_id.Append(50);
		data->m_output_to_framebuffer.Append(TRUE);
		data->m_output_to_file.Append(TRUE);
		data->m_output_to_jpeg.Append(FALSE);
	}

	double samplingReduceFactor = 1;
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
	}

	
	NSI::Context context(i_parser->GetContext());
	
	int shading_samples = max(1, (int)(settings->GetInt32(DL_SHADING_SAMPLES)
		*samplingReduceFactor + 0.5));

	int volume_samples = max(1, (int)(settings->GetInt32(DL_VOLUME_SAMPLES)
		*samplingReduceFactor + 0.5));

	int diffuse_depth = settings->GetInt32(DL_MAX_DIFFUSE_DEPTH);
	int reflection_depth = settings->GetInt32(DL_MAX_REFLECTION_DEPTH);
	int refraction_depth = settings->GetInt32(DL_MAX_REFRACTION_DEPTH);


	BaseDocument *doc = GetActiveDocument();
	string docName = doc->GetDocumentName().GetString().GetCStringCopy();
	
	int pos = -1;
	pos = docName.find(".");

	if(pos!=-1)
	docName = docName.substr(0, pos);
	ApplicationOutput("Document @", docName.c_str());

	Filename fn = settings->GetFilename(DL_DEFAULT_IMAGE_FILENAME);
	std::string dir = fn.GetString().GetCStringCopy();
	dir = dir + "\\" + docName;


	ctx.SetAttribute(NSI_SCENE_GLOBAL,(
		NSI::IntegerArg("quality.shadingsamples",shading_samples),
		NSI::IntegerArg("quality.volumesamples", volume_samples),
		NSI::IntegerArg("maximumraydepth.diffuse", diffuse_depth),
		NSI::IntegerArg("maximumraydepth.reflection", reflection_depth),
		NSI::IntegerArg("maximumraydepth.refraction", refraction_depth)
		));

	string filter_output = getFilter(settings->GetInt32(DL_PIXEL_FILTER));
	int m_layer_number = data->m_selected_layers.GetCount();
	int sortkey = 0;

	auto layeroutput = OutputLayer(i_parser);
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
			m_layer_handle = string(i_parser->GetUniqueName("DisplayLayer"));
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

			ctx.Connect(m_layer_handle, "", "scene_camera_screen", "outputlayers");
			ctx.Connect(m_display_driver_handle, "", m_layer_handle, "outputdrivers");
		}

		/**
			If the m_output_to_file icon is on then we
			connect the selected output driver from render settings
			with the layer that we are currently on
		*/
		if (outputLayerFile)
		{
			m_layer_file = string(i_parser->GetUniqueName("file_outputlayer"));
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
			ctx.Connect(m_layer_file, "", "scene_camera_screen", "outputlayers");
			ctx.Connect(m_file_driver_handle, "", m_layer_file, "outputdrivers");
		}

		/**
			If the m_output_to_jpg icon is on then we
			create a jpeg driver and connect each of them
			with their corresponding layer.
		*/
		if (ouputLayerImage)
		{
			m_layer_jpg = string(i_parser->GetUniqueName("image_outputlayer"));
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


			m_output_driver_handle = string(i_parser->GetUniqueName("driver"));
			ctx.Create(m_output_driver_handle, "outputdriver");

			ctx.SetAttribute(m_output_driver_handle, (
				NSI::StringArg("drivername", "jpeg"),
				NSI::StringArg("imagefilename", ouptut_image.c_str())
				));

			ctx.Connect(m_output_driver_handle, "", m_layer_jpg, "outputdrivers");
			ctx.Connect(m_layer_jpg, "", "scene_camera_screen", "outputlayers");
		}
	}
}

void RenderOptionsHook::ConnectNSINodes(BaseDocument* i_doc, DL_SceneParser* i_parser)
{
	NSI::Context ctx(i_parser->GetContext());
}
