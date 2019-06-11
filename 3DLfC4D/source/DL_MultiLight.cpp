#include "DL_Multi_Light.h"
#include "c4d_basebitmap.h"
#include "c4d_symbols.h"
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <tuple>
#include "3DelightRenderer.h"

using namespace std;

#define ID_CUSTOMGUI_MULTILIGHTS 10346689
#define ID_CUSTOMDATATYPE_MULTILIGHTS 12456689

//Class to modify the TreeView dynimaclly in the way we want(define class)
class MultiLightCustomTree
{
public:
	String Name = "";
	Bool _selected = FALSE;
	Int32 ListPosition;
	Int32 objectGUID;
	AutoAlloc<BaseLink> link;
	//Gets the main category string ex(Lights) for light objects
	MultiLightCustomTree(String Name)
	{
		this->Name = Name;
	}

	MultiLightCustomTree(BaseObject* object, Int position)
	{
		this->Name = object->GetName();
		this->ListPosition = position;
		this->objectGUID = object->GetGUID();
	}

	Bool isSelected()
	{
		return this->_selected;
	}

	Bool Select()
	{
		this->_selected = TRUE;
	}

	Bool Deselect()
	{
		this->_selected = FALSE;
	}

	String TO_STRING()
	{
		return (this->Name);
	}
};

/*
	In this function we get all the light objects that are used in the scene,
	and store them within a vector of our custom treeview class. The light objects
	order depends on their order in the scene.
*/
vector<MultiLightCustomTree*> Populate_TreeView()
{
	vector<MultiLightCustomTree*> light_list;
	AutoAlloc<BaseLink> link;
	BaseObject* object = GetActiveDocument()->GetFirstObject();
	if (!object)
		return light_list;

	MultiLightCustomTree* light = new MultiLightCustomTree("Lights");
	light_list.push_back(light);
	bool process = true;
	Int32 listPosition = 1;
	while (object)
	{
		if (process && object->GetType() == Olight)
		{
			MultiLightCustomTree* light_object = new MultiLightCustomTree(object, listPosition++);
			light_list.push_back(light_object);
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
	/*
		Since the insertion of the objects is done like in a stack where the new
		object goes to the top of the view, and we want to display them in the
		opposite way, we are reversing the vectorto get the desired result.
		Starting from the second items as the first one is the light group.
	*/
	reverse(light_list.begin() + 1, light_list.begin() + light_list.size());
	return light_list;
}


static void ApplicationOutputF(const Char* i_format, ...)
{
	va_list arp;
	Char		buf[1024];

	va_start(arp, i_format);
	vsprintf_safe(buf, sizeof(buf), i_format, arp);
	va_end(arp);
}

//class to supply data and view model for TreeViewCustomGui
class MultiLightTreeView : public TreeViewFunctions
{
public:
	vector<MultiLightCustomTree*> mylist;

	//Filling the vector with objects to display in the treeview
	void Populate()
	{
		mylist = Populate_TreeView();
	}

	//Called to retrieve the first object of the tree
	void* GetFirst(void* root, void* userdata)
	{
		if (mylist.size() > 0)
			return mylist[0];
		return nullptr;
	}

	//Called to retrieve the first child of @formatParam{obj}
	void* GetDown(void* root, void* userdata, void* obj)
	{
		MultiLightCustomTree* object = nullptr;
		MultiLightCustomTree* currentObject = (MultiLightCustomTree*)obj;
		std::vector<MultiLightCustomTree*>::iterator it = std::find(mylist.begin(), mylist.end(), currentObject);
		Int32 index = std::distance(mylist.begin(), it);
		Int32 nextIndex = index + 1;
		if (currentObject == mylist[0] && nextIndex < mylist.size())
			return mylist[nextIndex];
		return nullptr;
	}

	//Called to retrieve the next object after @formatParam{obj}
	void* GetNext(void* root, void* userdata, void* obj)
	{
		MultiLightCustomTree* object = nullptr;
		MultiLightCustomTree* currentObject = (MultiLightCustomTree*)obj;
		std::vector<MultiLightCustomTree*>::iterator it = std::find(mylist.begin(), mylist.end(), currentObject);
		Int32 index = std::distance(mylist.begin(), it);
		Int32 nextIndex = index + 1;

		if (currentObject == mylist[0])
			return NULL;

		if (nextIndex < mylist.size())
			object = mylist[nextIndex];
		return object;
	}

	// Called to retrieve the column width of object @formatParam{obj} in column @formatParam{col}
	Int32 GetColumnWidth(void* root, void* userdata, void* obj, Int32 col, GeUserArea* area)
	{
		return 80;
	}

	//Called to retrieve the folding status of the object.
	Bool IsOpened(void* root, void* userdata, void* obj)
	{
		return TRUE;
	}

	//Called to retrieve the name of object @formatParam{obj}.
	String GetName(void* root, void* userdata, void* obj)
	{

		if (!root || !obj)
			return String();
		String    name = String();
		MultiLightCustomTree* lights = (MultiLightCustomTree*)obj;
		if (lights == mylist[0])
			name = lights->TO_STRING();
		else
			name = String("   ") + lights->TO_STRING();
		return name;
	}


	Int	GetId(void* root, void* userdata, void* obj)
	{
		return 0;
	}

	//Called to retrieve the drag type of object @formatParam{obj}, 
	//i.e. the type that the user would get if he started a drag in that cell.
	Int32 GetDragType(void* root, void* userdata, void* obj)
	{
		return NOTOK;
	}

	//Called to select object @formatParam{obj}.
	void Select(void* root, void* userdata, void* obj, Int32 mode)
	{
		if (!root || !obj)
			return;
		MultiLightCustomTree  *lights = (MultiLightCustomTree*)obj;
		if (lights != mylist[0])
		{
			if (mode == SELECTION_NEW)
			{
				if (lights->isSelected())
					lights->Deselect();
				else if (!lights->isSelected())
					lights->Select();
			}
		}
		return;
	}

	//Called to retrieve the selection status of object @formatParam{obj}
	Bool IsSelected(void* root, void* userdata, void* obj)
	{
		if (!root || !obj)
			return FALSE;
		MultiLightCustomTree  *lights = (MultiLightCustomTree*)obj;
		return lights->isSelected();
	}

	//Called to specify the text colors of object @formatParam{obj}
	void GetColors(void *root, void *userdata, void *obj, GeData* pNormal, GeData* pSelected)
	{
		MultiLightCustomTree  *lights = (MultiLightCustomTree*)obj;
		pSelected->SetVector(Vector(1, 1, 1));
	}

	//Called to specify the background color of @formatParam{line}.
	void GetBackgroundColor(void* root, void* userdata, void* obj, Int32 line, GeData* col)
	{
		MultiLightCustomTree  *lights = (MultiLightCustomTree*)obj;
		if (lights == mylist[0])
			col->SetVector(Vector(0.27, 0.27, 0.27));

		if (lights->isSelected())
		{
			col->SetVector(Vector(0.42, 0.42, 0.42));
		}
	}
};

MultiLightTreeView all_lights;


/**
	- Description stores information on the parameters found
	on NodeData based elements like objects, tags or materials.
	description is about parameters, not GUI elements.

	- GUI system is based on GeDialogs. Such a GeDialog is more or less
	an ordinary GUI window.  This window contains groups and standard GUI
	elements. If you want to create a custom GUI element, you can do so by
	implementing a custom class based on GeUserArea.

	Conclusion:

	Descriptions and GeDialogs are two different things that have nothing to do
	with each other. Object parameters != GUI elements.  The only connection is
	that you can display the parameters of an object in a GeDialog using the
	DescriptionCustomGui element.That is what the Attribute Manager is doing.
*/


//Create a constructor
iCustomDataTypeMultiLights::iCustomDataTypeMultiLights(const BaseContainer &i_settings, CUSTOMGUIPLUGIN *i_plugin) : iCustomGui(i_settings, i_plugin)
{
};

Bool iCustomDataTypeMultiLights::CreateLayout()
{
	Bool layout_create = GeDialog::CreateLayout();
	layout_create = LoadDialogResource(MULTILIGHT_SELECTOR, nullptr, 0);

	if (layout_create)
	{
		m_light_view = (TreeViewCustomGui*)FindCustomGui(DL_MULTI_LIGHT_TREEVIEW, CUSTOMGUI_TREEVIEW);
		all_lights.Populate();
		BaseContainer layout;
		layout.SetInt32(1, LV_TREE);
		m_light_view->SetLayout(1, layout);
		m_light_view->SetRoot(m_light_view, &all_lights, this);
		m_light_view->Refresh();

		/*
			Keeping the selected items selected after the layout is changed or refreshed.
		*/
		Int32 treeSize = all_lights.mylist.size();
		if (treeSize > 0)
		{
			for (int i = 1; i < treeSize; i++)
			{
				for (int j = 0; j < m_data.m_selected_lights_itemID.GetCount(); j++)
				{
					//Checks for the selected lights that were on the m_selected_lights_GUID
					//stored before the layout was changed.
					if (all_lights.mylist[treeSize - i]->objectGUID == m_data.m_selected_lights_GUID[j])
					{
						all_lights.mylist[treeSize - i]->Select();
					}

				}

			}
		}
	}
	return SUPER::CreateLayout();
};



Bool iCustomDataTypeMultiLights::InitValues()
{
	m_light_view->Refresh();
	return SUPER::InitValues();
};


Int32 iCustomDataTypeMultiLights::Message(const BaseContainer& msg, BaseContainer& result)
{
	switch (msg.GetId())
	{
		case BFM_INTERACTEND:
		{
			ApplicationOutput("ASDD");
			/*
				Getting the state of the tree after an interaction is finished in the view.
				This includes clicking somewhere on the view where you can select or deselect
				ligts, or clicking buttons in the view etc. After we store the latest state of the
				tree in the vectors below:
				m_multi_light_selected_layers => Store the names of the selected lights
				m_selected_lights_itemID => Store the selected lights row number. This is used as their ID in the RenderOptionsHook.
				m_data.m_selected_lights_GUID => Store the selected lights Unique GUID.
				Used for reording of selections when the order of the lights is changed manually
			*/
			m_light_view->Refresh();
			m_data.m_multi_light_selected_layers.Reset();
			m_data.m_selected_lights_itemID.Reset();
			m_data.m_selected_lights_GUID.Reset();
			m_data.m_all_multi_lights.Reset();
			Int32 treeSize = all_lights.mylist.size();
			if (treeSize > 0)
			{
				for (int i = 1; i < treeSize; i++)
				{
					m_data.m_all_multi_lights.Append(all_lights.mylist[i]->TO_STRING());
					//Adding only the selected light objects in the BaseArray
					if (all_lights.mylist[i]->isSelected())
					{
						m_data.m_multi_light_selected_layers.Append(all_lights.mylist[i]->TO_STRING());
						m_data.m_selected_lights_itemID.Append(i);
						m_data.m_selected_lights_GUID.Append(all_lights.mylist[i]->objectGUID);
					}
				}
			}
			m_light_view->Refresh();

		}
	}


	BaseContainer m(BFM_ACTION);
	m.SetInt32(BFM_ACTION_ID, GetId());
	m.SetData(BFM_ACTION_VALUE, this->GetData().GetValue());
	SendParentMessage(m);
	return GeDialog::Message(msg, result);
}

/**
	This Overridden function is called when a node receives messages. e.g when a button is clicked
	or a checkbox is checked etc. In this case it checks for the changes that happens on the Custom GUI.
	It stores the information for every row and column of the ListView and any other data type that is
	used there and by using this funtion we get access on this information.
*/
Bool iCustomDataTypeMultiLights::Command(Int32 i_id, const BaseContainer &i_msg)
{

	/**
	Here we get the data from the custom UI and store it so whenever this class in instantiated
	its values will remain unchanged
	*/
	return true;
}

Bool iCustomDataTypeMultiLights::CoreMessage(Int32 id, const BaseContainer& msg)
{
	if (id == EVMSG_CHANGE)
	{
		all_lights.Populate();

		/*
			Keeping the selected items selected after the layout is changed or refreshed.
		*/
		Int32 treeSize = all_lights.mylist.size();
		if (treeSize > 0)
		{
			for (int i = 1; i < treeSize; i++)
			{
				for (int j = 0; j < m_data.m_selected_lights_itemID.GetCount(); j++)
				{
					//Checks for the selected lights that were on the m_selected_lights_GUID
					//stored before the layout was changed.
					if (all_lights.mylist[treeSize - i]->objectGUID == m_data.m_selected_lights_GUID[j])
					{
						all_lights.mylist[treeSize - i]->Select();
					}

				}

			}
		}

		m_data.m_multi_light_selected_layers.Reset();
		m_data.m_selected_lights_itemID.Reset();
		m_data.m_selected_lights_GUID.Reset();
		m_data.m_all_multi_lights.Reset();
		treeSize = all_lights.mylist.size();
		if (treeSize > 0)
		{
			for (int i = 1; i < treeSize; i++)
			{
				m_data.m_all_multi_lights.Append(all_lights.mylist[i]->TO_STRING());
				//Adding only the selected light objects in the BaseArray
				if (all_lights.mylist[i]->isSelected())
				{
					m_data.m_multi_light_selected_layers.Append(all_lights.mylist[i]->TO_STRING());
					m_data.m_selected_lights_itemID.Append(i);
					m_data.m_selected_lights_GUID.Append(all_lights.mylist[i]->objectGUID);
				}
			}
		}
		m_light_view->Refresh();
	}

	return true;
}

/**
	Called to update the custom GUI to display the value in @formatParam{tristate}.
*/
Bool iCustomDataTypeMultiLights::SetData(const TriState<GeData> &i_tristate)
{

	iCustomDataTypeLights* data = (iCustomDataTypeLights*)(i_tristate.GetValue().GetCustomDataType(ID_CUSTOMDATATYPE_MULTILIGHTS));
	if (data)
	{
		iferr(m_data.m_multi_light_selected_layers.CopyFrom(data->m_multi_light_selected_layers))
			return false;

		iferr(m_data.m_selected_lights_itemID.CopyFrom(data->m_selected_lights_itemID))
			return false;

		iferr(m_data.m_selected_lights_GUID.CopyFrom(data->m_selected_lights_GUID))
			return false;

		iferr(m_data.m_all_multi_lights.CopyFrom(data->m_all_multi_lights))
			return false;

		m_data.m_check = data->m_check;
	}

	return true;
};



// Called to retrieve the value(s) currently displayed.
TriState<GeData> iCustomDataTypeMultiLights::GetData()
{
	TriState<GeData> tri;
	tri.Add(GeData(ID_CUSTOMDATATYPE_MULTILIGHTS, m_data));
	return tri;
};

static Int32 g_stringtable[] = { ID_CUSTOMDATATYPE_MULTILIGHTS }; // This array defines the applicable datatypes.

// This CustomGuiData class registers a new custom GUI for the Layers datatype.
class DelightCustonGuiMultiLights : public CustomGuiData
{
public:
	virtual Int32 GetId();
	virtual CDialog* Alloc(const BaseContainer& i_settings);
	virtual void Free(CDialog* i_dlg, void* i_userdata);
	virtual const Char* GetResourceSym();
	virtual CustomProperty* GetProperties();
	virtual Int32 GetResourceDataType(Int32*& i_table);

};


//Called to get the plugin ID of the custom GUI.
Int32 DelightCustonGuiMultiLights::GetId()
{
	return ID_CUSTOMGUI_MULTILIGHTS;
};

/**
We override Alloc function to create a new instance of the plugin.It is used instead of a constructor
at this one provide means to return an error if something goes wrong.
*/
CDialog* DelightCustonGuiMultiLights::Alloc(const BaseContainer& i_settings)
{
	iferr(iCustomDataTypeMultiLights* dlg = NewObj(iCustomDataTypeMultiLights, i_settings, GetPlugin()))
		return nullptr;

	CDialog *cdlg = dlg->Get();

	if (!cdlg)
		return nullptr;

	return cdlg;
};


//This function is called when an instance gets freed.
void DelightCustonGuiMultiLights::Free(CDialog* i_dlg, void* i_userdata)
{
	if (!i_dlg || !i_userdata)
		return;

	iCustomDataTypeMultiLights* sub = static_cast<iCustomDataTypeMultiLights*>(i_userdata);
	DeleteObj(sub);
};

// Returns the resource symbol. This symbol can be used in resource files in combination with "CUSTOMGUI".
const Char* DelightCustonGuiMultiLights::GetResourceSym()
{
	return "DELIGHTMULTILIGHTCUSTOMGUI";
};

CustomProperty* DelightCustonGuiMultiLights::GetProperties()
{
	return nullptr;
};

// Returns the applicable datatypes defined in the stringtable array.
Int32 DelightCustonGuiMultiLights::GetResourceDataType(Int32*& i_table)
{
	i_table = g_stringtable;
	return sizeof(g_stringtable) / sizeof(Int32);
};

/**
A data class for creating custom data types.
These can be used in descriptions and container just like built-in data types.
As we do not have a buil-in data type for the Listview we create our custom data type
using the CUSTOMDATATYPECLASS.
*/
class LightsCustomDataTypeClass : public CustomDataTypeClass
{
	INSTANCEOF(LightsCustomDataTypeClass, CustomDataTypeClass)

public:
	virtual Int32 GetId()
	{
		return ID_CUSTOMDATATYPE_MULTILIGHTS;
	}

	virtual CustomDataType* AllocData()
	{
		iCustomDataTypeLights* data = NewObjClear(iCustomDataTypeLights);
		return data;
	};

	virtual void FreeData(CustomDataType* i_data)
	{
		iCustomDataTypeLights* d = static_cast<iCustomDataTypeLights*>(i_data);
		DeleteObj(d);
	}

	/**
	Called to copy an instance of the custom data type.
	Copy the data from @formatParam{src} to @formatParam{dest}.
	*/
	virtual Bool CopyData(const CustomDataType* i_src, CustomDataType* i_dst, AliasTrans* i_aliastrans)
	{
		// copy the data to the given target data
		iCustomDataTypeLights* s = (iCustomDataTypeLights*)(i_src);
		iCustomDataTypeLights* d = (iCustomDataTypeLights*)(i_dst);

		if (!s || !d)
			return false;

		d->m_multi_light_selected_layers.Flush();
		iferr(d->m_multi_light_selected_layers.CopyFrom(s->m_multi_light_selected_layers))
			return false;

		d->m_selected_lights_itemID.Flush();
		iferr(d->m_selected_lights_itemID.CopyFrom(s->m_selected_lights_itemID))
			return false;

		d->m_selected_lights_GUID.Flush();
		iferr(d->m_selected_lights_GUID.CopyFrom(s->m_selected_lights_GUID))
			return false;

		d->m_all_multi_lights.Flush();
		iferr(d->m_all_multi_lights.CopyFrom(s->m_all_multi_lights))
			return false;

		d->m_check = s->m_check;

		return true;
	}

	virtual Int32 Compare(const CustomDataType* i_dst1, const CustomDataType* i_dst2)
	{
		return 1;
	}

	/**Called to write the custom data type to a file.*/
	virtual Bool WriteData(const CustomDataType* i_t_d, HyperFile* i_hf)
	{
		const iCustomDataTypeLights* const d = static_cast<const iCustomDataTypeLights*>(i_t_d);

		const maxon::Int multi_light_selected = d->m_multi_light_selected_layers.GetCount();
		i_hf->WriteInt64((Int64)multi_light_selected);
		for (Int64 i = 0; i < multi_light_selected; ++i)
		{
			i_hf->WriteString(d->m_multi_light_selected_layers[i]);
			i_hf->WriteInt32(d->m_selected_lights_itemID[i]);
			i_hf->WriteUInt64(d->m_selected_lights_GUID[i]);
		}


		const maxon::Int all_multi_lights = d->m_all_multi_lights.GetCount();
		i_hf->WriteInt64((Int64)all_multi_lights);
		for (Int64 i = 0; i < all_multi_lights; ++i)
		{
			i_hf->WriteString(d->m_all_multi_lights[i]);
		}

		return true;
	}

	//Called to read the custom data type from a file.
	virtual Bool ReadData(CustomDataType* i_t_d, HyperFile* i_hf, Int32 i_level)
	{

		iCustomDataTypeLights* const d = static_cast<iCustomDataTypeLights*>(i_t_d);

		if (i_level > 0)
		{
			Int64 multi_light_selected = 0;
			if (i_hf->ReadInt64(&multi_light_selected))
			{
				for (Int64 i = 0; i < multi_light_selected; ++i)
				{
					String multi_light_layer;
					Int32 selected_light_GUID;

					if (i_hf->ReadString(&multi_light_layer))
					{
						iferr(d->m_multi_light_selected_layers.Append(multi_light_layer))
							return false;
					}

					if (i_hf->ReadInt32(&selected_light_GUID))
					{
						iferr(d->m_selected_lights_itemID.Append(selected_light_GUID))
							return false;
					}
				}
			}

		}
		return true;
	}

	//Called to get the symbol to use in resource files
	virtual const Char* GetResourceSym()
	{
		return "DELIGHTMULTILIGHTCUSTOMTYPE";
	}

	virtual void GetDefaultProperties(BaseContainer &i_data)
	{
		// the default values of this datatype
		// use the custom GUI as default
		i_data.SetInt32(DESC_CUSTOMGUI, ID_CUSTOMGUI_MULTILIGHTS);
		i_data.SetInt32(DESC_ANIMATE, DESC_ANIMATE_ON);
	}
};

/**
	With this function we register the custom GUI plugin that we have alread build.
*/
Bool RegisterCustomMultiLight()
{
	if (!RegisterCustomDataTypePlugin(
		"Multi-Light"_s,
		CUSTOMDATATYPE_INFO_LOADSAVE |
		CUSTOMDATATYPE_INFO_TOGGLEDISPLAY |
		CUSTOMDATATYPE_INFO_HASSUBDESCRIPTION,
		NewObjClear(LightsCustomDataTypeClass),
		1))
		return false;


	static BaseCustomGuiLib myStringGUIlib;

	ClearMem(&myStringGUIlib, sizeof(myStringGUIlib));
	FillBaseCustomGui(myStringGUIlib);

	InstallLibrary(ID_CUSTOMGUI_MULTILIGHTS, &myStringGUIlib, 1000, sizeof(myStringGUIlib));

	RegisterCustomGuiPlugin(GeLoadString(74663782), 0, NewObjClear(DelightCustonGuiMultiLights));

	return true;
}


