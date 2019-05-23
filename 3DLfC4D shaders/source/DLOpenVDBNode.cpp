#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "dl_open_vdb_node.h"
#include <resultbase.h>
#include "DL_TypeConversions.h"
#include "VDBQuery.h"

/*
	When we use Spline datatype in .res file to represent a splineData Object 
	in a container we do not get the spline so we have to draw it manually using Knots 
	and we are going to use this funtion to draw the spline on the container UI.
*/
static void InitSpline(BaseContainer &bc, Int32 id)
{
	GeData data(CUSTOMDATATYPE_SPLINE, DEFAULTVALUE);
	SplineData* spline = (SplineData*)data.GetCustomDataType(CUSTOMDATATYPE_SPLINE);
	if (spline)
	{
		spline->MakeLinearSplineLinear(2); //Make Linear Spline

		Int32 count = spline->GetKnotCount();
		if (count == 2)
		{
			CustomSplineKnot* knot0 = spline->GetKnot(0);
			if (knot0)
			{
				knot0->vPos = Vector(0, 0, 0);
			}

			CustomSplineKnot* knot1 = spline->GetKnot(1);
			if (knot1)
			{
				knot1->vPos = Vector(1, 1, 0);
			}
		}
	}
	bc.SetData(id, data);
}


//Building the Gradient Colors
static void initGradient(BaseContainer &bc, Int32 id)
{
	GeData data(CUSTOMDATATYPE_GRADIENT, DEFAULTVALUE);


	Gradient* gradient = (Gradient*)data.GetCustomDataType(CUSTOMDATATYPE_GRADIENT);

	if (gradient)
	{
		GradientKnot k1, k2, k3, k4, k5, k6, k7;
		k1.col = HSVToRGB(Vector(177.0/360.0, 1.0, 0.0));
		k1.pos = 0.0;
		k2.col = HSVToRGB(Vector(1.0, 1.0, 1.0));
		k2.pos = 0.07;
        
		k3.col = HSVToRGB(Vector(2.022/360.0, 1.0, 1.0));
		k3.pos = 0.1;
             
		k4.col = HSVToRGB(Vector(13.632/360.0, 1.0, 1.0));
		k4.pos = 0.19;
                
		k5.col = HSVToRGB(Vector(315.892/360.0, 0.055, 1.0));
		k5.pos = 0.65;
                   
		k6.col = HSVToRGB(Vector(261.041/360.0, 0.073, 1.0));
		k6.pos = 0.69;
                           
		k7.col = HSVToRGB(Vector(227.605/360.0, 0.373, 1.0));
		k7.pos = 1;

		gradient->InsertKnot(k1);
		gradient->InsertKnot(k2);
		gradient->InsertKnot(k3);
		gradient->InsertKnot(k4);
		gradient->InsertKnot(k5);
		gradient->InsertKnot(k6);
		gradient->InsertKnot(k7);
	}
	bc.SetData(id, data);
}



bool AddItem(Description *desc, BaseContainer* bc, Int32 Id)
{
	const DescID* singleid = desc->GetSingleDescID();
	BaseContainer names;
	BaseContainer data = GetCustomDataTypeDefault(DTYPE_LONG);
	//  Set CycleButton properties
	BaseContainer grid;

	const DescID cid = DescLevel(Id, DTYPE_LONG, 0);
	if (!singleid || cid.IsPartOf(*singleid, nullptr))
	{

		switch (Id)
		{
		case GRIDS_GROUP_SMOKE:
			data.SetString(DESC_NAME, "Smoke"_s);
			grid.SetString(0, "None"_s);

			break;
		case GRIDS_GROUP_TEMPERATURE:
			data.SetString(DESC_NAME, "Temperature"_s);
			grid.SetString(0, "None"_s);
			break;
		case GRIDS_GROUP_EMISSION_INTENSITY:
			data.SetString(DESC_NAME, "Emission Intensity"_s);
			grid.SetString(0, "None"_s);
			break;
		case GRIDS_GROUP_VELOCITY:
			data.SetString(DESC_NAME, "Velocity"_s);
			grid.SetString(0, "None"_s);
			break;
		default:
			break;
		}

		std::string texturename = "";
		Filename texturefile = bc->GetFilename(FILE_FILENAME);
		if (texturefile.IsPopulated())
		{
			texturename = StringToStdString(texturefile.GetString());
			int num_grids = 0;

			const char *const *grid_names;
			DlVDBGetGridNames(texturename.c_str(), &num_grids, &grid_names);

			ApplicationOutput("NumGrids @", num_grids);
			for (int i = 0; i < num_grids; i++)
			{
				grid.SetString(i + 1, String(grid_names[i]));
			}

		}

		data.SetInt32(DESC_CUSTOMGUI, CUSTOMGUI_CYCLE);
		data.SetContainer(DESC_CYCLE, grid);
		desc->SetParameter(DescLevel(Id, DTYPE_LONG, 0), data, ID_OBJECTPROPERTIES);
	}
	return true;
}



class OpenVDB : public ObjectData {
	INSTANCEOF(OpenVDB, ObjectData);
public:
	static NodeData *Alloc(void) { return NewObj(OpenVDB) iferr_ignore("Wrong Instance"); }
	virtual Bool Message(GeListNode *node, Int32 type, void *data);
	virtual Bool Init(GeListNode* node);
	virtual DRAWRESULT Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	virtual Int32 GetHandleCount(BaseObject* op);
	virtual Bool GetDEnabling(GeListNode *node, const DescID &id, 
		const GeData &t_data, DESCFLAGS_ENABLE flags, const BaseContainer *itemdesc);
	virtual Bool GetDDescription(GeListNode *node, Description *description, DESCFLAGS_DESC &flags);
	virtual void GetHandle(BaseObject* op, Int32 i, HandleInfo& info);
	virtual void SetHandle(BaseObject* op, Int32 i, Vector p, const HandleInfo& info);
	//virtual BaseObject* GetVirtualObjects(PluginObject *op, HierarchyHelp *hh);
private:

};

Bool OpenVDB::GetDDescription(GeListNode *node, Description *description, DESCFLAGS_DESC &flags)
{
	if (!description->LoadDescription(node->GetType()))
		return false;
	flags |= DESCFLAGS_DESC::LOADED;

	BaseContainer *bc = ((BaseObject*)node)->GetDataInstance();

	AddItem(description, bc, GRIDS_GROUP_SMOKE);
	AddItem(description, bc, GRIDS_GROUP_TEMPERATURE);
	AddItem(description, bc, GRIDS_GROUP_EMISSION_INTENSITY);
	AddItem(description, bc, GRIDS_GROUP_VELOCITY);
	return SUPER::GetDDescription(node, description, flags);
}

Bool OpenVDB::Init(GeListNode* node) {
	BaseObject		*op = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();
	
	data->SetBool(FILE_USE_FILE_SEQUENCE, FALSE);
	data->SetInt32(FILE_INDEX_RANGE_START, 1);
	data->SetInt32(FILE_INDEX_RANGE_END, 1);
	data->SetInt32(FILE_OUTSIDE_RANGE, FILE_OUTSIDE_RANGE_HOLD_FIRST);
	data->SetInt32(FILE_START_OFFSET, 0);
	data->SetInt32(FILE_INCREMENT, 1);
	data->SetBool(FILE_CONFINE_TO_FRAME_RANGE, FALSE);
	data->SetInt32(FILE_FRAME_RANGE_START, 1);
	data->SetInt32(FILE_FRAME_RANGE_END, 1);
	node->SetParameter(DescID(GRIDS_GROUP_SMOKE), GeData(0), DESCFLAGS_SET::NONE);
	node->SetParameter(DescID(GRIDS_GROUP_TEMPERATURE), GeData(0), DESCFLAGS_SET::NONE);
	node->SetParameter(DescID(GRIDS_GROUP_EMISSION_INTENSITY), GeData(0), DESCFLAGS_SET::NONE);
	node->SetParameter(DescID(GRIDS_GROUP_VELOCITY), GeData(0), DESCFLAGS_SET::NONE);

	data->SetFloat(GRIDS_GROUP_VELOCITY_SCALE, 1.0);
	data->SetBool(OBJECT_DISPLAY_DRAW_POINTS, FALSE);
	data->SetBool(VISIBILITY_VISSIBLE_IN_DIFFUSE, TRUE);

	//Setting VDB Shader Attributes
	data->SetFloat(SCATTERING_DENSITY, 1.0);
	data->SetVector(SCATTERING_COLOR, HSVToRGB(Vector(0,0,1)));
	data->SetFloat(SCATTERING_ANISOTROPY, 0.0);
	data->SetBool(SCATTERING_MULTIPLE_SCATTERING, TRUE);

	data->SetBool(DENSITY_RAMP_ENABLE, FALSE);
	data->SetFloat(DENSITY_RAMP_RANGE_START, 0.0);
	data->SetFloat(DENSITY_RAMP_RANGE_END, 1.0);
	InitSpline(*data, DENSITY_RAMP_RAMP);

	data->SetVector(TRANSPARENCY_COLOR, HSVToRGB(Vector(0, 0, 0)));
	data->SetFloat(TRANSPARENCY_SCALE, 1.0);
	data->SetFloat(EMMISSION_SCALE, 1.0);
	
	data->SetBool(INTENSITY_USE_GRID, FALSE);
	data->SetFloat(INTENSITY_RANGE_START, 0.0);
	data->SetFloat(INTENSITY_RANGE_END, 1.0);
	InitSpline(*data, INTENSITY_RAMP);

	data->SetFloat(BLACKBODY_INTENSITY, 0.0);
	data->SetInt32(BLACKBODY_MODE, BLACKBODY_ARTISTIC);
	data->SetFloat(BLACKBODY_KELVIN, 5000.0);
	data->SetVector(BLACKBODY_TINT, HSVToRGB(Vector(0, 0, 1)));
	data->SetFloat(BLACKBODY_RANGE_START, 0.0);
	data->SetFloat(BLACKBODY_RANGE_END, 1.0);
	InitSpline(*data, BLACKBODY_RAMP);

	data->SetFloat(RAMP_INTENSITY, 0.0);
	data->SetVector(RAMP_TINT, HSVToRGB(Vector(0, 0, 1)));
	data->SetFloat(RAMP_RANGE_START, 0.0);
	data->SetFloat(RAMP_RANGE_END, 1.0);
	data->SetFloat(xMin, -100.0);
	data->SetFloat(yMin, -100.0);
	data->SetFloat(zMin, -100.0);
	data->SetFloat(xMax, 100.0);
	data->SetFloat(yMax, 100.0);
	data->SetFloat(zMax, 100.0);

	initGradient(*data, EMISSION_RAMP);
	return true;
}

Bool OpenVDB::Message(GeListNode *node, Int32 type, void *data)
{
	switch (type)
	{
		case MSG_DESCRIPTION_CHECKUPDATE:
		{
			DescriptionCheckUpdate* dc = (DescriptionCheckUpdate*)data;
			const DescID* Id = dc->descid;
			GeData data;
			Int32 Type = 0;
			if (node->GetParameter(*Id, data, DESCFLAGS_GET::NONE))
				Type = data.GetType();
			
			if (Type == DA_FILENAME)
			{
				node->SetParameter(DescID(GRIDS_GROUP_SMOKE), GeData(0), DESCFLAGS_SET::NONE);
				node->SetParameter(DescID(GRIDS_GROUP_TEMPERATURE), GeData(0), DESCFLAGS_SET::NONE);
				node->SetParameter(DescID(GRIDS_GROUP_EMISSION_INTENSITY), GeData(0), DESCFLAGS_SET::NONE);
				node->SetParameter(DescID(GRIDS_GROUP_VELOCITY), GeData(0), DESCFLAGS_SET::NONE);

				string texturename = "";
				Filename texturefile = data.GetFilename();
				if (texturefile.IsPopulated())
				{
					texturename = StringToStdString(texturefile.GetString());
					int num_grids = 0;

					const char *const *grid_names;
					DlVDBGetGridNames(texturename.c_str(), &num_grids, &grid_names);

					for (int i = 0; i < num_grids; i++)
					{
						if (String(grid_names[i]) == "density")
							node->SetParameter(DescID(GRIDS_GROUP_SMOKE), GeData(i+1), DESCFLAGS_SET::NONE);
						
						else if(String(grid_names[i]) == "temperature")
							node->SetParameter(DescID(GRIDS_GROUP_TEMPERATURE), GeData(i + 1), DESCFLAGS_SET::NONE);

						else if (String(grid_names[i]) == "intensity")
							node->SetParameter(DescID(GRIDS_GROUP_EMISSION_INTENSITY), GeData(i + 1), DESCFLAGS_SET::NONE);

						else if (String(grid_names[i]) == "v" || grid_names[i] == "veclocity")
							node->SetParameter(DescID(GRIDS_GROUP_VELOCITY), GeData(i + 1), DESCFLAGS_SET::NONE);

					}

					double bbox[6];
					if (DlVDBGetFileBBox(texturename.c_str(), bbox))
					{
						node->SetParameter(DescID(xMin), GeData(bbox[0]), DESCFLAGS_SET::NONE);
						node->SetParameter(DescID(yMin), GeData(bbox[1]), DESCFLAGS_SET::NONE);
						node->SetParameter(DescID(zMin), GeData(bbox[2]), DESCFLAGS_SET::NONE);
						node->SetParameter(DescID(xMax), GeData(bbox[3]), DESCFLAGS_SET::NONE);
						node->SetParameter(DescID(yMax), GeData(bbox[4]), DESCFLAGS_SET::NONE);
						node->SetParameter(DescID(zMax), GeData(bbox[5]), DESCFLAGS_SET::NONE);
					}

				}
			}
			break;
		}
	}
	EventAdd();
	return SUPER::Message(node, type, data);
}

DRAWRESULT OpenVDB::Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh)
{

	if (drawpass == DRAWPASS::OBJECT)
	{
		BaseContainer* data = op->GetDataInstance();
		Vector min(data->GetFloat(xMin), data->GetFloat(yMin), data->GetFloat(zMin));
		Vector max(data->GetFloat(xMax), data->GetFloat(yMax), data->GetFloat(zMax));

		float xMin = min[0];
		float yMin = min[1];
		float zMin = min[2];
		float xMax = max[0];
		float yMax = max[1];
		float zMax = max[2];

		Vector color = HSVToRGB(Vector(0, 0, 1));

		Matrix m = bh->GetMg();
		bd->SetMatrix_Matrix(NULL, m);

		bd->SetPen(color);

		bd->DrawLine(Vector(xMin, yMin, zMin), Vector(xMin, yMax, zMin), 0);
		bd->DrawLine(Vector(xMin, yMax, zMin), Vector(xMax, yMax, zMin), 0);
		bd->DrawLine(Vector(xMax, yMax, zMin), Vector(xMax, yMin, zMin), 0);
		bd->DrawLine(Vector(xMax, yMin, zMin), Vector(xMin, yMin, zMin), 0);

		bd->DrawLine(Vector(xMin, yMin, zMax), Vector(xMin, yMax, zMax), 0);
		bd->DrawLine(Vector(xMin, yMax, zMax), Vector(xMax, yMax, zMax), 0);
		bd->DrawLine(Vector(xMax, yMax, zMax), Vector(xMax, yMin, zMax), 0);
		bd->DrawLine(Vector(xMax, yMin, zMax), Vector(xMin, yMin, zMax), 0);

		bd->DrawLine(Vector(xMin, yMin, zMin), Vector(xMin, yMin, zMax), 0);
		bd->DrawLine(Vector(xMin, yMin, zMax), Vector(xMin, yMax, zMax), 0);
		bd->DrawLine(Vector(xMin, yMax, zMax), Vector(xMin, yMax, zMin), 0);
		bd->DrawLine(Vector(xMin, yMax, zMin), Vector(xMin, yMin, zMin), 0);

		bd->DrawLine(Vector(xMax, yMin, zMin), Vector(xMax, yMin, zMax), 0);
		bd->DrawLine(Vector(xMax, yMin, zMax), Vector(xMax, yMax, zMax), 0);
		bd->DrawLine(Vector(xMax, yMax, zMax), Vector(xMax, yMax, zMin), 0);
		bd->DrawLine(Vector(xMax, yMax, zMin), Vector(xMax, yMin, zMin), 0);

		bd->DrawLine(Vector(xMin, yMin, zMin), Vector(xMin, yMin, zMax), 0);
		bd->DrawLine(Vector(xMin, yMin, zMax), Vector(xMax, yMin, zMax), 0);
		bd->DrawLine(Vector(xMax, yMin, zMax), Vector(xMax, yMin, zMin), 0);
		bd->DrawLine(Vector(xMax, yMin, zMin), Vector(xMin, yMin, zMin), 0);

		bd->DrawLine(Vector(xMin, yMax, zMin), Vector(xMin, yMax, zMax), 0);
		bd->DrawLine(Vector(xMin, yMax, zMax), Vector(xMax, yMax, zMax), 0);
		bd->DrawLine(Vector(xMax, yMax, zMax), Vector(xMax, yMax, zMin), 0);
		bd->DrawLine(Vector(xMax, yMax, zMin), Vector(xMin, yMax, zMin), 0);
	}

	return ObjectData::Draw(op, drawpass, bd, bh);
}

Int32 OpenVDB::GetHandleCount(BaseObject* op) {
	return 3;
}

void OpenVDB::GetHandle(BaseObject* op, Int32 i, HandleInfo& info) {
	BaseContainer *data = op->GetDataInstance();
	Vector min(data->GetFloat(xMin), data->GetFloat(yMin), data->GetFloat(zMin));
	Vector max(data->GetFloat(xMax), data->GetFloat(yMax), data->GetFloat(zMax));
	
	float xMin = min[0];
	float yMin = min[1];
	float zMin = min[2];
	float xMax = max[0];
	float yMax = max[1];
	float zMax = max[2];

	if (i == 0) {
		info.position = Vector(xMax, 0, 0);
		info.direction = Vector(1.0, 0, 0);
		info.type = HANDLECONSTRAINTTYPE::LINEAR;
	}
	else if (i == 1) {
		info.position = Vector(0, yMax, 0);
		info.direction = Vector(0, 1.0, 0);
		info.type = HANDLECONSTRAINTTYPE::LINEAR;
	}
	else if (i == 2) {
		info.position = Vector(0, 0, zMax);
		info.direction = Vector(0, 0, 1.0);
		info.type = HANDLECONSTRAINTTYPE::LINEAR;
	}
}

void OpenVDB::SetHandle(BaseObject* op, Int32 i, Vector p, const HandleInfo& info) {
		BaseContainer *data = op->GetDataInstance();
		
		if (!data)
			return;

		HandleInfo inf;
		GetHandle(op, i, inf);
		Float value = Dot(p - inf.position, info.direction);

		if (i == 0) {
			data->SetFloat(xMax, ClampValue(data->GetFloat(xMax) + value, 0.0_f, (Float)MAXRANGE));
			data->SetFloat(xMin, ClampValue(data->GetFloat(xMin) - value, (Float)-MAXRANGE,0.0_f));
		}

		if (i == 1) {
			data->SetFloat(yMax, ClampValue(data->GetFloat(yMax) + value, 0.0_f, (Float)MAXRANGE));
			data->SetFloat(yMin, ClampValue(data->GetFloat(yMin) - value, (Float)-MAXRANGE, 0.0_f));
		}

		if (i == 2) {
			data->SetFloat(zMax, ClampValue(data->GetFloat(zMax) + value, 0.0_f, (Float)MAXRANGE));
			data->SetFloat(zMin, ClampValue(data->GetFloat(zMin) - value, (Float)-MAXRANGE, 0.0_f));
		}
		
}


Bool OpenVDB::GetDEnabling(
	GeListNode *node,
	const DescID &id,
	const GeData &t_data,
	DESCFLAGS_ENABLE flags,
	const BaseContainer *itemdesc)
{

	if (!node)
		return false;
	BaseDocument* doc = GetActiveDocument();
	BaseContainer* dldata = ((BaseObject*)node)->GetDataInstance();
	switch (id[0].id)
	{
	case FILE_INDEX_RANGE_START:
	case FILE_INDEX_RANGE_END:
	case FILE_OUTSIDE_RANGE:
	case FILE_INCREMENT:
	case FILE_CONFINE_TO_FRAME_RANGE:
		return dldata->GetBool(FILE_USE_FILE_SEQUENCE, doc) == true;
		break;

	case FILE_START_OFFSET:
		return dldata->GetBool(FILE_USE_FILE_SEQUENCE, doc) == true &&
			dldata->GetInt32(FILE_OUTSIDE_RANGE)==FILE_OUTSIDE_RANGE_REPEAT;
		break;

	case FILE_FRAME_RANGE_START:
	case FILE_FRAME_RANGE_END:
		return dldata->GetBool(FILE_USE_FILE_SEQUENCE, doc) == true &&
			dldata->GetBool(FILE_CONFINE_TO_FRAME_RANGE,doc)==true;
		break;

	case GRIDS_GROUP_VELOCITY_SCALE:
		return 	dldata->GetInt32(GRIDS_GROUP_VELOCITY) != GRIDS_GROUP_VELOCITY_NONE;;
		break;

	case DENSITY_RAMP_RANGE_START:
	case DENSITY_RAMP_RANGE_END:
	case DENSITY_RAMP_RAMP:
		return 	dldata->GetBool(DENSITY_RAMP_ENABLE) == TRUE;
		break;
	case INTENSITY_RANGE_START:
	case INTENSITY_RANGE_END:
	case INTENSITY_RAMP:
		return 	dldata->GetBool(INTENSITY_USE_GRID) == TRUE;
		break;
	default:
		break;

	}
	return true;
}

Bool RegisterOpenVDB(void)
{
	return RegisterObjectPlugin(DL_OPENVDB, "Open VDB"_s, OBJECT_GENERATOR, OpenVDB::Alloc, "dl_open_vdb_node"_s, AutoBitmap("out_dlOpenVDBShape_150.png"_s), 0);
}
