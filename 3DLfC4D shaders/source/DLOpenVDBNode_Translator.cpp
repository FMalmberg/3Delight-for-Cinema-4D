#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLOpenVDBNode_Translator.h"
#include <assert.h>
#include <map>
#include "dl_open_vdb_node.h"
#include "VDBQuery.h"

//Use this function to get the Text value from the Dropdown (CYCLE)
String getDropDownNames(Description* const description, Int32 group_id, Int32 SelectedItem)
{
	const DescID* singleid = description->GetSingleDescID();
	const DescID cid = DescLevel(group_id, DTYPE_LONG, 0);
	String selected_name = "";
	if (!singleid || cid.IsPartOf(*singleid, nullptr))
	{
		AutoAlloc<AtomArray> arr;
		BaseContainer* selectionParameter = description->GetParameterI(DescLevel(group_id, DTYPE_LONG, 0), arr);

		if (selectionParameter != nullptr)
		{
			BaseContainer* items = selectionParameter->GetContainerInstance(DESC_CYCLE);
			if (items != nullptr)
			{
				selected_name = items->GetData(SelectedItem).GetString();
			}
		}
	}
	return selected_name;
}


void Delight_OpenVDBTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());
	m_handle = string(parser->GetUniqueName("OpenVDBObject"));
	m_transform_handle = string(ParentTransformHandle);
	ctx.Create(m_handle, "volume");
	ctx.Connect(m_handle, "", m_transform_handle, "objects");

	m_attributes_handle = string(parser->GetUniqueName("OpenVDBAttributes"));
	ctx.Create(m_attributes_handle, "attributes");

	BaseList2D* baselist = (BaseList2D*)C4DNode;
	BaseContainer* data = baselist->GetDataInstance();

	AutoAlloc<Description> description;
	if (description == nullptr)
		return;

	if (!baselist->GetDescription(description, DESCFLAGS_DESC::NONE))
		return;

	string texturename = "";
	Filename texturefile = data->GetFilename(FILE_FILENAME);
	if (texturefile.IsPopulated())
	{
		texturename = StringToStdString(texturefile.GetString());
		int num_grids = 0;
		const char *const *grid_names;
		DlVDBGetGridNames(texturename.c_str(), &num_grids, &grid_names);

		ApplicationOutput("NumGrids @", num_grids);
		for (int i = 0; i < num_grids; i++)
		{
			ApplicationOutput("Grid @", grid_names[i]);
		}
	}
	double velocity_scale = data->GetFloat(GRIDS_GROUP_VELOCITY_SCALE);
	/*
		Here we get the selected ID for the grids but we need the text
		that it contains. Get String won't work as dropdown can be only
		of LONG type
	*/
	int smoke = data->GetInt32(GRIDS_GROUP_SMOKE); 
	int temperature = data->GetInt32(GRIDS_GROUP_TEMPERATURE);
	int emission = data->GetInt32(GRIDS_GROUP_EMISSION_INTENSITY);
	int velocity = data->GetInt32(GRIDS_GROUP_VELOCITY);

	std::string smoke_text= 
		getDropDownNames(description, GRIDS_GROUP_SMOKE,smoke).GetCStringCopy();

	std::string temperature_text= 
		getDropDownNames(description, GRIDS_GROUP_TEMPERATURE,temperature).GetCStringCopy();

	std::string emission_text= 
		getDropDownNames(description, GRIDS_GROUP_EMISSION_INTENSITY,emission).GetCStringCopy();

	std::string velocity_text= 
		getDropDownNames(description, GRIDS_GROUP_VELOCITY,velocity).GetCStringCopy();
	
	
	NSI::ArgumentList args;
	args.Add(new NSI::StringArg("vdbfilename", texturename));
	args.Add(new NSI::StringArg("densitygrid", smoke_text.c_str()));
	args.Add(new NSI::StringArg("temperaturegrid", temperature_text.c_str()));
	args.Add(new NSI::StringArg("emissionintensitygrid", emission_text.c_str()));
	args.Add(new NSI::StringArg("velocitygrid", velocity_text.c_str()));
	args.Add(new NSI::DoubleArg("velocityscale", velocity_scale));
	ctx.SetAttribute(m_handle, args);

	m_shader_handle = string(parser->GetUniqueName("OpenVDBShader"));
	ctx.Create(m_shader_handle, "shader");
	args.clear();

	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("vdbVolume.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	//Scattering Group
	float density = data->GetFloat(SCATTERING_DENSITY);
	Vector c4dScatteringColor = toLinear(data->GetVector(SCATTERING_COLOR), doc);
	float scattering_color[3]{ c4dScatteringColor.x,c4dScatteringColor.y,c4dScatteringColor.z };
	float scattering_anisotropy = data->GetFloat(SCATTERING_ANISOTROPY);
	bool multiple_scattering = data->GetBool(SCATTERING_MULTIPLE_SCATTERING);

	args.Add(new NSI::StringArg("shaderfilename", &c_shaderpath[0]));
	args.Add(new NSI::FloatArg("density", density));
	args.Add(new NSI::ColorArg("scattering_color", &scattering_color[0]));
	args.Add(new NSI::FloatArg("scattering_anisotropy", scattering_anisotropy));
	args.Add(new NSI::IntegerArg("multiple_scattering", multiple_scattering));

	//Density Ramp Group
	bool density_ramp_enable = data->GetBool(DENSITY_RAMP_ENABLE);
	float density_range_start = data->GetFloat(DENSITY_RAMP_RANGE_START);
	float density_range_end = data->GetFloat(DENSITY_RAMP_RANGE_END);

	GeData density_ramp_data = data->GetData(DENSITY_RAMP_RAMP);
	SplineData* density_ramp = (SplineData*)density_ramp_data.GetCustomDataType(CUSTOMDATATYPE_SPLINE);
	int density_knot_count = density_ramp->GetKnotCount();
	vector<float> density_curve_knots(density_knot_count);
	vector<float> density_curve_floats(density_knot_count);
	vector<int> density_curve_interpolations(density_knot_count);

	for (int i = 0; i < density_knot_count; i++)
	{
		CustomSplineKnot* Knot = density_ramp->GetKnot(i);
		density_curve_knots[i] = Knot->vPos.x;
		density_curve_floats[i] = Knot->vPos.y;
		density_curve_interpolations[i] = Knot->interpol;
	}

	args.Add(new NSI::IntegerArg("density_remap_enable", density_ramp_enable));
	args.Add(new NSI::FloatArg("density_remap_range_start", density_range_start));
	args.Add(new NSI::FloatArg("density_remap_range_end", density_range_end));

	args.Add(NSI::Argument::New("density_remap_curve_Knots")
			->SetType(NSITypeFloat)
			->SetCount(density_knot_count)
			->SetValuePointer(&density_curve_knots[0]));

	args.Add(NSI::Argument::New("density_remap_curve_Floats")
			->SetType(NSITypeFloat)
			->SetCount(density_knot_count)
			->SetValuePointer(&density_curve_floats[0]));
    
	args.Add(NSI::Argument::New("density_remap_curve_interpolations")
			->SetType(NSITypeInteger)
			->SetCount(density_knot_count)
			->SetValuePointer(&density_curve_interpolations[0]));

	//Transparency Group
	Vector c4dTransparencyColor = toLinear(data->GetVector(TRANSPARENCY_COLOR), doc);
	float transparency_color[3]{ c4dTransparencyColor.x,c4dTransparencyColor.y,c4dTransparencyColor.z };
	float transparency_scale = data->GetFloat(TRANSPARENCY_SCALE);

	args.Add(new NSI::ColorArg("transparency_color", &transparency_color[0]));
	args.Add(new NSI::FloatArg("transparency_scale", transparency_scale));

	//Emission Group
	float emmission_scale = data->GetFloat(EMMISSION_SCALE);
	bool intensity_use_grid = data->GetBool(INTENSITY_USE_GRID);
	float intensity_range_start = data->GetFloat(INTENSITY_RANGE_START);
	float intensity_range_end = data->GetFloat(INTENSITY_RANGE_END);

	GeData intensity_ramp_data = data->GetData(INTENSITY_RAMP);
	SplineData* intensity_ramp = (SplineData*)intensity_ramp_data.GetCustomDataType(CUSTOMDATATYPE_SPLINE);
	int intensity_knot_count = intensity_ramp->GetKnotCount();
	vector<float> intensity_curve_knots(intensity_knot_count);
	vector<float> intensity_curve_floats(intensity_knot_count);
	vector<int> intensity_curve_interpolations(intensity_knot_count);

	for (int i = 0; i < intensity_knot_count; i++)
	{
		CustomSplineKnot* Knot = intensity_ramp->GetKnot(i);
		intensity_curve_knots[i] = Knot->vPos.x;
		intensity_curve_floats[i] = Knot->vPos.y;
		intensity_curve_interpolations[i] = Knot->interpol;
	}

	args.Add(new NSI::FloatArg("emission_intensity_scale", emmission_scale));
	args.Add(new NSI::IntegerArg("emission_intensity_grid_enable", intensity_use_grid));
	args.Add(new NSI::FloatArg("emission_intensity_range_start", intensity_range_start));
	args.Add(new NSI::FloatArg("emission_intensity_range_end", intensity_range_end));

	args.Add(NSI::Argument::New("emission_intensity_curve_Knots")
		->SetType(NSITypeFloat)
		->SetCount(intensity_knot_count)
		->SetValuePointer(&intensity_curve_knots[0]));

	args.Add(NSI::Argument::New("emission_intensity_curve_Floats")
		->SetType(NSITypeFloat)
		->SetCount(intensity_knot_count)
		->SetValuePointer(&intensity_curve_floats[0]));

	args.Add(NSI::Argument::New("emission_intensity_curve_interpolations")
		->SetType(NSITypeInteger)
		->SetCount(intensity_knot_count)
		->SetValuePointer(&intensity_curve_interpolations[0]));

	//Blackbody Group
	float blackbody_intensity = data->GetFloat(BLACKBODY_INTENSITY);
	int blackbody_mode = data->GetInt32(BLACKBODY_MODE);
	float blackbody_kelvin = data->GetFloat(BLACKBODY_KELVIN);
	Vector c4dBlackBodyTint = toLinear(data->GetVector(BLACKBODY_TINT), doc);
	float blackbody_tint[3]{ c4dBlackBodyTint.x,c4dBlackBodyTint.y,c4dBlackBodyTint.z };
	float blackbody_range_start = data->GetFloat(BLACKBODY_RANGE_START);
	float blackbody_range_end = data->GetFloat(BLACKBODY_RANGE_END);

	GeData blackbody_ramp_data = data->GetData(BLACKBODY_RAMP);
	SplineData* blackbody_ramp = (SplineData*)blackbody_ramp_data.GetCustomDataType(CUSTOMDATATYPE_SPLINE);
	int blackbody_knot_count = blackbody_ramp->GetKnotCount();
	vector<float> blackbody_curve_knots(blackbody_knot_count);
	vector<float> blackbody_curve_floats(blackbody_knot_count);
	vector<int> blackbody_curve_interpolations(blackbody_knot_count);

	for (int i = 0; i < blackbody_knot_count; i++)
	{
		CustomSplineKnot* Knot = blackbody_ramp->GetKnot(i);
		blackbody_curve_knots[i] = Knot->vPos.x;
		blackbody_curve_floats[i] = Knot->vPos.y;
		blackbody_curve_interpolations[i] = Knot->interpol;
	}

	args.Add(new NSI::FloatArg("blackbody_intensity", blackbody_intensity));
	args.Add(new NSI::IntegerArg("blackbody_mode", blackbody_mode));
	args.Add(new NSI::FloatArg("blackbody_kelvin", blackbody_kelvin));
	args.Add(new NSI::ColorArg("blackbody_tint", &blackbody_tint[0]));
	args.Add(new NSI::FloatArg("blackbody_temperature_range_start", blackbody_range_start));
	args.Add(new NSI::FloatArg("blackbody_temperature_range_end", blackbody_range_end));

	args.Add(NSI::Argument::New("blackbody_temperature_curve_Knots")
		->SetType(NSITypeFloat)
		->SetCount(blackbody_knot_count)
		->SetValuePointer(&blackbody_curve_knots[0]));

	args.Add(NSI::Argument::New("blackbody_temperature_curve_Floats")
		->SetType(NSITypeFloat)
		->SetCount(blackbody_knot_count)
		->SetValuePointer(&blackbody_curve_floats[0]));

	args.Add(NSI::Argument::New("blackbody_temperature_curve_interpolations")
		->SetType(NSITypeInteger)
		->SetCount(blackbody_knot_count)
		->SetValuePointer(&blackbody_curve_interpolations[0]));

	//Ramp Group
	float ramp_intensity = data->GetFloat(RAMP_INTENSITY);
	Vector c4dRampTint = toLinear(data->GetVector(RAMP_TINT), doc);
	float ramp_tint[3]{ c4dRampTint.x,c4dRampTint.y,c4dRampTint.z };
	float ramp_range_start = data->GetFloat(RAMP_RANGE_START);
	float ramp_range_end = data->GetFloat(RAMP_RANGE_END);

	GeData ramp_data = data->GetData(EMISSION_RAMP);
	Gradient* emmission_ramp = (Gradient*)ramp_data.GetCustomDataType(CUSTOMDATATYPE_GRADIENT);
	int ramp_knot_count = emmission_ramp->GetKnotCount();
	vector<float> ramp_curve_knots(ramp_knot_count);
	vector<float> ramp_curve_colors(ramp_knot_count*3);
	vector<int> ramp_curve_interpolations(ramp_knot_count);

	for (int i = 0; i < ramp_knot_count; i++)
	{
		int j = i * 3;
		GradientKnot Knot = emmission_ramp->GetKnot(i);
		ramp_curve_knots[i] = Knot.pos;
		Vector KnotColor = (Knot.col),doc;
		//float knot_color[3]{ KnotColor.x, KnotColor.y, KnotColor.z };
		//ramp_curve_colors[i] = {knot_color[0], knot_color[1], knot_color[2], };
		ramp_curve_colors[j] = KnotColor.x;
		ramp_curve_colors[j+1] = KnotColor.y;
		ramp_curve_colors[j+2] = KnotColor.z;
		ramp_curve_interpolations[i] = Knot.interpolation;
	}

	args.Add(new NSI::FloatArg("emissionramp_intensity", ramp_intensity));
	args.Add(new NSI::ColorArg("emissionramp_tint", &ramp_tint[0]));
	args.Add(new NSI::FloatArg("emissionramp_temperature_range_start", ramp_range_start));
	args.Add(new NSI::FloatArg("emissionramp_temperature_range_end", ramp_range_end));
	
	args.Add(NSI::Argument::New("emissionramp_color_curve_Knots")
		->SetType(NSITypeFloat)
		->SetCount(ramp_knot_count)
		->SetValuePointer(&ramp_curve_knots[0]));

	args.Add(NSI::Argument::New("emissionramp_color_curve_Colors")
		->SetType(NSITypeColor)
		->SetCount(ramp_knot_count)
		->SetValuePointer(&ramp_curve_colors[0]));

	args.Add(NSI::Argument::New("emissionramp_color_curve_interpolations")
		->SetType(NSITypeInteger)
		->SetCount(ramp_knot_count)
		->SetValuePointer(&ramp_curve_interpolations[0]));

	ctx.SetAttribute(m_shader_handle, args);
	parser->SetAssociatedHandle((BaseList2D*)C4DNode, m_shader_handle.c_str());
}

void Delight_OpenVDBTranslator::ConnectNSINodes(GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser)
{
	NSI::Context ctx(parser->GetContext());
	ctx.Connect(m_shader_handle, "", m_attributes_handle, "volumeshader");
	ctx.Connect(m_attributes_handle, "", m_handle, "geometryattributes");
}
