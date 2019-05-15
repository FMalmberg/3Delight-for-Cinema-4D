#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "dl_open_vdb_node.h"
#include <resultbase.h>
#include "DL_TypeConversions.h"
#include "VDBQuery.h"
#define VDB_UNIQUE_ID 500

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


class OpenVDB : public ObjectData {
public:
	static NodeData *Alloc(void) { return NewObj(OpenVDB) iferr_ignore("Wrong Instance"); }
	virtual Bool Init(GeListNode* node);
	virtual DRAWRESULT Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh);
	virtual Int32 GetHandleCount(BaseObject* op);
	virtual Bool GetDEnabling(GeListNode *node, const DescID &id, 
		const GeData &t_data, DESCFLAGS_ENABLE flags, const BaseContainer *itemdesc);
	virtual Bool GetDDescription(GeListNode *node, Description *description, DESCFLAGS_DESC &flags);
	virtual void GetHandle(BaseObject* op, Int32 i, HandleInfo& info);
	virtual void SetHandle(BaseObject* op, Int32 i, Vector p, const HandleInfo& info);
	//virtual BaseObject* GetVirtualObjects(PluginObject *op, HierarchyHelp *hh);
};



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

	data->SetInt32(GRIDS_GROUP_SMOKE, GRIDS_GROUP_SMOKE_NONE);
	data->SetInt32(GRIDS_GROUP_TEMPERATURE, GRIDS_GROUP_TEMPERATURE_NONE);
	data->SetInt32(GRIDS_GROUP_EMISSION_INTENSITY, GRIDS_GROUP_EMISSION_INTENSITY_NONE);
	data->SetInt32(GRIDS_GROUP_VELOCITY, GRIDS_GROUP_VELOCITY_NONE);
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
	initGradient(*data, EMISSION_RAMP);

	return true;
}


DRAWRESULT OpenVDB::Draw(BaseObject* op, DRAWPASS drawpass, BaseDraw* bd, BaseDrawHelp* bh)
{
	
	if (drawpass == DRAWPASS::OBJECT)
	{
		BaseContainer *data = op->GetDataInstance();
		float width = 200;
		float height = 200;

		Vector color =HSVToRGB(Vector(0,0,1));

		Matrix m = bh->GetMg();
		bd->SetMatrix_Matrix(NULL, m);

		bd->SetPen(color);

		bd->DrawLine(Vector(-width / 2.0, -height / 2.0, -100), Vector(-width / 2.0, height / 2.0, -100), 0);
		bd->DrawLine(Vector(-width / 2.0, height / 2.0, -100), Vector(width / 2.0, height / 2.0, -100), 0);
		bd->DrawLine(Vector(width / 2.0, height / 2.0, -100), Vector(width / 2.0, -height / 2.0, -100), 0);
		bd->DrawLine(Vector(width / 2.0, -height / 2.0, -100), Vector(-width / 2.0, -height / 2.0, -100), 0);

		bd->DrawLine(Vector(-width / 2.0, -height / 2.0, 100), Vector(-width / 2.0, height / 2.0, 100), 0);
		bd->DrawLine(Vector(-width / 2.0, height / 2.0, 100), Vector(width / 2.0, height / 2.0, 100), 0);
		bd->DrawLine(Vector(width / 2.0, height / 2.0, 100), Vector(width / 2.0, -height / 2.0, 100), 0);
		bd->DrawLine(Vector(width / 2.0, -height / 2.0, 100), Vector(-width / 2.0, -height / 2.0, 100), 0);

		bd->DrawLine(Vector(-100, -100, -100), Vector(-100, -100, 100), 0);
		bd->DrawLine(Vector(-100, -100, 100), Vector(-100, 100, 100), 0);
		bd->DrawLine(Vector(-100, 100, 100), Vector(-100, 100, -100), 0);
		bd->DrawLine(Vector(-100, 100, -100), Vector(-100, -100, -100), 0);

		bd->DrawLine(Vector(100, -100, -100), Vector(100, -100, 100), 0);
		bd->DrawLine(Vector(100, -100, 100), Vector(100, 100, 100), 0);
		bd->DrawLine(Vector(100, 100, 100), Vector(100, 100, -100), 0);
		bd->DrawLine(Vector(100, 100, -100), Vector(100, -100, -100), 0);

		bd->DrawLine(Vector(-100, -100, -100), Vector(-100, -100, 100), 0);
		bd->DrawLine(Vector(-100, -100, 100), Vector(100, -100, 100), 0);
		bd->DrawLine(Vector(100, -100, 100), Vector(100, -100, -100), 0);
		bd->DrawLine(Vector(100, -100, -100), Vector(-100, -100, -100), 0);


		bd->DrawLine(Vector(-100, 100, -100), Vector(-100, 100, 100), 0);
		bd->DrawLine(Vector(-100, 100, 100), Vector(100, 100, 100), 0);
		bd->DrawLine(Vector(100, 100, 100), Vector(100, 100, -100), 0);
		bd->DrawLine(Vector(100, 100, -100), Vector(-100, 100, -100), 0);
	}

	return ObjectData::Draw(op, drawpass, bd, bh);
}

Int32 OpenVDB::GetHandleCount(BaseObject* op) {
	return 4;
}

void OpenVDB::GetHandle(BaseObject* op, Int32 i, HandleInfo& info) {
	BaseContainer *data = op->GetDataInstance();
	float width = 200;
	float height = 200;

	if (i == 0) {
		info.position = Vector(-width / 2.0, 0, 0);
		info.direction = Vector(-1.0, 0, 0);
		info.type = HANDLECONSTRAINTTYPE::LINEAR;
		//return (Vector(-width/2.0,0,0));
	}
	else if (i == 1) {
		info.position = Vector(width / 2.0, 0, 0);
		info.direction = Vector(1.0, 0, 0);
		info.type = HANDLECONSTRAINTTYPE::LINEAR;
		//return (Vector(width/2.0,0,0));
	}
	else if (i == 2) {
		info.position = Vector(0, -height / 2.0, 0);
		info.direction = Vector(0, -1.0, 0);
		info.type = HANDLECONSTRAINTTYPE::LINEAR;
		//return (Vector(0,-height/2.0,0));
	}
	else if (i == 3) {
		info.position = Vector(0, height / 2.0, 0);
		info.direction = Vector(0, 1.0, 0);
		info.type = HANDLECONSTRAINTTYPE::LINEAR;
		//return (Vector(0,height/2.0,0));
	}
}

void OpenVDB::SetHandle(BaseObject* op, Int32 i, Vector p, const HandleInfo& info) {
	BaseContainer *data = op->GetDataInstance();	
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


Bool AddItem(Description *desc, BaseContainer grids, BaseContainer* bc, Int32 Id)
{

	BaseContainer names;
	BaseContainer data = GetCustomDataTypeDefault(DTYPE_LONG);
	//  Set CycleButton properties
	data.SetInt32(DESC_CUSTOMGUI, CUSTOMGUI_CYCLE);
	data.SetInt32(DESC_DEFAULT, 0);
	data.SetInt32(DESC_ANIMATE, DESC_ANIMATE_OFF);
	data.SetContainer(DESC_CYCLE, grids);
	switch (Id)
	{
		case GRIDS_GROUP_SMOKE:
			data.SetString(DESC_NAME, "Smoke"_s);
			break;
        case GRIDS_GROUP_TEMPERATURE:
			data.SetString(DESC_NAME, "Temperature"_s);
			break;
        case GRIDS_GROUP_EMISSION_INTENSITY:
			data.SetString(DESC_NAME, "Emission Intensity"_s);
			break;
        case GRIDS_GROUP_VELOCITY	:
			data.SetString(DESC_NAME, "Velocity"_s);
			break;
	default:
		break;
	}

	bc->SetInt32(Id, VDB_UNIQUE_ID - 1);

	desc->SetParameter(DescLevel(Id, DTYPE_LONG, 0), data, DescID(ID_OBJECTPROPERTIES));
	return TRUE;
}


Bool OpenVDB::GetDDescription(GeListNode *node, Description *description, DESCFLAGS_DESC &flags)
{
	description->LoadDescription(DL_OPENVDB);
	flags |= DESCFLAGS_DESC::LOADED;
	BaseContainer *bc = ((BaseTag*)node)->GetDataInstance();
	BaseContainer grid;
	grid.SetString(VDB_UNIQUE_ID-1, "None"_s);
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
			grid.SetString(VDB_UNIQUE_ID+i, String(grid_names[i]));
		}
		AddItem(description, grid, bc,GRIDS_GROUP_SMOKE);
		AddItem(description, grid, bc,GRIDS_GROUP_TEMPERATURE);
		AddItem(description, grid, bc,GRIDS_GROUP_EMISSION_INTENSITY);
		AddItem(description, grid, bc,GRIDS_GROUP_VELOCITY);
		Int32 i = 0;

		while (true)
		{
			const Int32 id = grid.GetIndexId(i++);
			if (id == NOTOK)
				break;
			else
			{
				String name = grid.GetData(id).GetString();
				if(name=="density")
					bc->SetInt32(GRIDS_GROUP_SMOKE, id);
				else if (name == "temperature")
					bc->SetInt32(GRIDS_GROUP_TEMPERATURE, id);
				else if (name == "intensity")
					bc->SetInt32(GRIDS_GROUP_EMISSION_INTENSITY, id);
				else if (name == "v" || name == "velocity")
					bc->SetInt32(GRIDS_GROUP_VELOCITY, id);
			}
		}
	}
	return TRUE;
}
/*BaseObject *LightCard::GetVirtualObjects(PluginObject *op, HierarchyHelp *hh)
{
return NULL;
}*/

Bool RegisterOpenVDB(void)
{
	return RegisterObjectPlugin(DL_OPENVDB, "Open VDB"_s, OBJECT_GENERATOR, OpenVDB::Alloc, "dl_open_vdb_node"_s, AutoBitmap("out_dlOpenVDBShape_150.png"_s), 0);
}
