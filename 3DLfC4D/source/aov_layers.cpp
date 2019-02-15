#include "aov_layers.h"
#include "c4d_basebitmap.h"
#include "c4d_symbols.h"
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "3DelightRenderer.h"

using namespace std;

#define ID_CUSTOMGUI_LAYERS 1034668
#define ID_CUSTOMDATATYPE_LAYERS 1245668


struct LayersData
{
	Int32	id;
	Char	name[50];
};

LayersData s_shading_components[] =
{
	{ 50, "RGBA (color + alpha)" },
	{ 51, "Diffuse" },
	{ 52, "Subsurface scattering" },
	{ 53, "Reflection" },
	{ 54, "Refraction" },
	{ 55, "Incandescence" },
	{ 0, "" }
};


LayersData s_auxiliary_variables[] =
{
	{ 100, "Z (depth)" },
	{ 101, "Camera space position" },
	{ 102, "Camera space normal" },
	{ 103, "UV" },
	{ 104, "Geometry ID" },
	{ 105, "Scene Path ID" },
	{ 106, "Relighting Multiplier" },
	{ 0, "" }
};




static void ApplicationOutputF(const Char* i_format, ...)
{
	va_list arp;
	Char		buf[1024];

	va_start(arp, i_format);
	vsprintf_safe(buf, sizeof(buf), i_format, arp);
	ApplicationOutput(String(buf));
	va_end(arp);
}

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
iCustomDataTypeAOV::iCustomDataTypeAOV(const BaseContainer &i_settings, CUSTOMGUIPLUGIN *i_plugin) : iCustomGui(i_settings, i_plugin)
{
};

Bool iCustomDataTypeAOV::CreateLayout()
{
	Bool layout_create = GeDialog::CreateLayout();
	layout_create = LoadDialogResource(AOV_SELECTOR, nullptr, 0);

	if (layout_create)
	{
		m_aov_listview.AttachListView(this, DL_SELECTED_LAYERS_LISTVIEW);
	}

	m_dialog.m_data = &m_data;
	return SUPER::CreateLayout();
};

/**
	This function is used to enable or disable buttons based on user activities
*/
void iCustomDataTypeAOV::m_update_buttons()
{
	if (!m_selection) 
	{
		Enable(AOV_ADD, FALSE);
		return;
	}
	Enable(AOV_REMOVE, m_aov_listview.GetSelection(m_selection) != 0);
	Enable(AOV_DUPLICATE, m_aov_listview.GetSelection(m_selection) != 0);
	Enable(AOV_VIEW, m_aov_listview.GetSelection(m_selection) != 0);
}


Bool iCustomDataTypeAOV::InitValues()
{
	BaseContainer layout;
	BaseContainer data;
	Int32 ItemId = 1;
	m_aov_listview.SetProperty(SLV_MULTIPLESELECTION, true);

	layout = BaseContainer();
	layout.SetInt32('Dspl', LV_COLUMN_BMP);
	layout.SetInt32('Fldr', LV_COLUMN_BMP);
	layout.SetInt32('Jpg', LV_COLUMN_BMP);
	layout.SetInt32('name', LV_COLUMN_TEXT);
	m_aov_listview.SetLayout(4, layout);

	if (m_data.m_selected_layers.GetCount() == 0)
	{
		m_data.m_selected_layers.Append(maxon::String(s_shading_components[0].name));
		m_data.m_selected_id.Append(maxon::Int32(s_shading_components[0].id));
		m_data.m_output_to_framebuffer.Append(TRUE);
		m_data.m_output_to_file.Append(TRUE);
		m_data.m_output_to_jpeg.Append(FALSE);
	}

	for (auto &value : m_data.m_selected_layers)
	{
		Int32 Position = ItemId - 1;
		maxon::String name = value;
		data.SetString('name', maxon::String(name));
		data.SetInt32('Dspl', m_data.m_output_to_framebuffer[Position] ? DL_DISPLAY_ON : DL_DISPLAY_OFF);
		data.SetInt32('Fldr', m_data.m_output_to_file[Position] ? DL_FOLDER_ON : DL_FOLDER_OFF);
		data.SetInt32('Jpg',  m_data.m_output_to_jpeg[Position] ? DL_JPG_ON : DL_JPG_OFF);
		m_aov_listview.SetItem(ItemId++, data);
	}

	m_aov_listview.DataChanged();
	m_update_buttons();
	return SUPER::InitValues();
};

AOVDialog::AOVDialog()
{
	m_data = NULL;
}

AOVDialog::~AOVDialog()
{
}

Bool AOVDialog::CreateLayout()
{
	// first call the parent instance
	Bool aov_selector = GeDialog::CreateLayout();
	aov_selector = LoadDialogResource(AOV_SELECTOR_DIALOG, nullptr, 0);

	if (aov_selector)
	{
		m_shading_listview.AttachListView(this, DL_SHADING_COMPONENTS_LISTVIEW);
		m_auxiliary_listview.AttachListView(this, DL_AUXILIARY_COMPONENTS_LISTVIEW);
	}

	if (m_data->m_selected_layers.GetCount() == 0) 
	{
		m_data->m_selected_layers.Append(maxon::String(s_shading_components[0].name));
		m_data->m_selected_id.Append(maxon::Int32(s_shading_components[0].id));
		m_data->m_output_to_framebuffer.Append(TRUE);
		m_data->m_output_to_file.Append(TRUE);
		m_data->m_output_to_jpeg.Append(FALSE);
	}

	AutoAlloc<BaseSelect> selected;
	for (auto &value : m_data->m_selected_id) 
	{
		selected->Select(value);
		m_auxiliary_listview.SetSelection(selected);
		m_shading_listview.SetSelection(selected);
	}
	return aov_selector;
}


void AOVDialog::m_update_buttons()
{
	if (!m_selection)
		return;

	Enable(BUTTON_OK, m_shading_listview.GetSelection(m_selection) != 0);
	Enable(BUTTON_OK, m_auxiliary_listview.GetSelection(m_selection) != 0);
	Enable(BUTTON_CANCEL, TRUE);
}

Bool AOVDialog::InitValues()
{
	// first call the parent instance
	if (!GeDialog::InitValues())
		return false;

	BaseContainer layout;
	BaseContainer data;
	Int32 i = 0;

	layout = BaseContainer();
	layout.SetInt32('name', LV_COLUMN_TEXT);
	m_shading_listview.SetLayout(1, layout);
	m_shading_listview.SetProperty(SLV_MULTIPLESELECTION, TRUE);

	layout = BaseContainer();
	layout.SetInt32('name', LV_COLUMN_TEXT);
	m_auxiliary_listview.SetLayout(1, layout);
	m_auxiliary_listview.SetProperty(SLV_MULTIPLESELECTION, TRUE);


	data = BaseContainer();
	for (i = 0; s_shading_components[i].id; i++)
	{
		data.SetString('name', String(s_shading_components[i].name));
		m_shading_listview.SetItem(s_shading_components[i].id, data);
	}

	for (i = 0; s_auxiliary_variables[i].id; i++)
	{
		data.SetString('name', String(s_auxiliary_variables[i].name));
		m_auxiliary_listview.SetItem(s_auxiliary_variables[i].id, data);
	}

	m_shading_listview.DataChanged();
	m_auxiliary_listview.DataChanged();
	m_update_buttons();

	return true;
}



Int32 AOVDialog::Message(const BaseContainer& i_msg, BaseContainer& i_result)
{
	return GeDialog::Message(i_msg, i_result);
}

/**
	This Overridden function is called when a node receives messages. e.g when a button is clicked
	or a checkbox is checked etc. In this case it checks for the changes that happens on the Custom GUI.
	It stores the information for every row and column of the ListView and any other data type that is 
	used there and by using this funtion we get access on this information.
*/
Bool iCustomDataTypeAOV::Command(Int32 i_id, const BaseContainer &i_msg)
{
	switch (i_id)
	{
		case DL_SELECTED_LAYERS_LISTVIEW:
		{
			if (m_data.m_selected_layers.GetCount() == 0)
			{
				m_data.m_selected_layers.Append(maxon::String(s_shading_components[0].name));
				m_data.m_selected_id.Append(maxon::Int32(s_shading_components[0].id));
				m_data.m_output_to_framebuffer.Append(TRUE);
				m_data.m_output_to_file.Append(TRUE);
				m_data.m_output_to_jpeg.Append(FALSE);
			}

			Int32 Column = i_msg.GetInt32(LV_SIMPLE_COL_ID);
			if (Column == 'Dspl') 
			{
				BaseContainer data;
				Int32 SelectedItemId = i_msg.GetInt32(LV_SIMPLE_ITEM_ID);
				m_aov_listview.GetItem(SelectedItemId, &data);
				Int32 CurrIconId = data.GetInt32('Dspl');
				String SelectedItemName = data.GetString('name');
				Int32 Pos = SelectedItemId - 1;
				switch (CurrIconId)
				{
					case DL_DISPLAY_ON:
						m_data.m_output_to_framebuffer[Pos] = FALSE;
						data.SetInt32('Dspl',m_data.m_output_to_framebuffer[Pos] ? DL_DISPLAY_ON : DL_DISPLAY_OFF);
						break;
					case DL_DISPLAY_OFF:
						m_data.m_output_to_framebuffer[Pos] = TRUE;
						data.SetInt32('Dspl', m_data.m_output_to_framebuffer[Pos] ? DL_DISPLAY_ON : DL_DISPLAY_OFF);
						break;
					default:
						m_data.m_output_to_framebuffer[Pos] = TRUE;
						break;
				}
				m_aov_listview.SetItem(SelectedItemId, data);
				m_aov_listview.DataChanged();
			}

			else if (Column == 'Fldr') 
			{
				BaseContainer data;
				Int32 SelectedItemId = i_msg.GetInt32(LV_SIMPLE_ITEM_ID);
				m_aov_listview.GetItem(SelectedItemId, &data);
				Int32 CurrIconId = data.GetInt32('Fldr');
				String SelectedItemName = data.GetString('name');
				Int32 Pos = SelectedItemId - 1;

				switch (CurrIconId)
				{
					case DL_FOLDER_ON:
						m_data.m_output_to_file[Pos] = FALSE;
						data.SetInt32('Fldr', m_data.m_output_to_file[Pos] ? DL_FOLDER_ON : DL_FOLDER_OFF);
						break;
					case DL_FOLDER_OFF:
						m_data.m_output_to_file[Pos] = TRUE;
						data.SetInt32('Fldr', m_data.m_output_to_file[Pos] ? DL_FOLDER_ON : DL_FOLDER_OFF);
						break;
					default:
						m_data.m_output_to_file[Pos] = TRUE;
						break;
				}
				m_aov_listview.SetItem(SelectedItemId, data);
				m_aov_listview.DataChanged();
			}

			else if (Column == 'Jpg') 
			{
				BaseContainer data;
				Int32 SelectedItemId = i_msg.GetInt32(LV_SIMPLE_ITEM_ID);
				m_aov_listview.GetItem(SelectedItemId, &data);
				Int32 CurrIconId = data.GetInt32('Jpg');
				String SelectedItemName = data.GetString('Jpg');
				Int32 Pos = SelectedItemId - 1;

				switch (CurrIconId)
				{
					case DL_JPG_ON:
						m_data.m_output_to_jpeg[Pos] = FALSE;
						data.SetInt32('Jpg', m_data.m_output_to_jpeg[Pos] ? DL_JPG_ON : DL_JPG_OFF);
						break;
					case DL_JPG_OFF:
						m_data.m_output_to_jpeg[Pos] = TRUE;
						data.SetInt32('Jpg', m_data.m_output_to_jpeg[Pos] ? DL_JPG_ON : DL_JPG_OFF);
						break;
					default:
						m_data.m_output_to_jpeg[Pos] = TRUE;
						break;
				}
				m_aov_listview.SetItem(SelectedItemId, data);
				m_aov_listview.DataChanged();
			}
			m_update_buttons();
			break;
	
		}

		case AOV_ADD:
		{
			m_data.m_check = true;
			Int32 Size = m_aov_listview.GetItemCount();
			m_dialog.Open(DLG_TYPE::MODAL, 10, -1, -1, 500, 200);

			if (!m_data.m_check)
			{
				for (int i = 1; i <= Size; i++) 
					m_aov_listview.RemoveItem(i);
		
				maxon::BaseArray<Int32> removedData;
				Int32 ItemId = 1;
				BaseContainer data;
				Int32 pos = 0; //Position in the array

				for (auto &value : m_data.m_selected_id) 
				{
					//if selected layer not found in aov_selector remove it
					if (!(m_dialog.m_aov_selector_selectedID.Contains(value)))
					{
						removedData.Append(pos);
					}
					pos++;
				}

				Int32 decrement = 0;
				for (auto &value : removedData) 
				{
					Int32 ID = value-decrement; //Arary size is decreased by 1 each time we remove
					m_data.m_selected_layers.Erase(ID);
					m_data.m_selected_id.Erase(ID);
					m_data.m_output_to_framebuffer.Erase(ID);
					m_data.m_output_to_file.Erase(ID);
					m_data.m_output_to_jpeg.Erase(ID);
					decrement++;
				}

				for (auto &value : m_dialog.m_aov_selector_selectedID)
				{
					Int32 Id = value;
					if (!(m_data.m_selected_id.Contains(Id)))
					{
						m_data.m_selected_id.Append(Id);
					}
				}

				for (auto &value : m_dialog.m_aov_selector_selected)
				{
					String name = value;
					if (!(m_data.m_selected_layers.Contains(name)))
					{
						m_data.m_selected_layers.Append(name);
						m_data.m_output_to_framebuffer.Append(TRUE);
						m_data.m_output_to_file.Append(TRUE);
						m_data.m_output_to_jpeg.Append(FALSE);
					}
				}


				if (m_data.m_selected_layers.GetCount() == 0)
				{
					m_data.m_selected_layers.Append(maxon::String(s_shading_components[0].name));
					m_data.m_selected_id.Append(maxon::Int32(s_shading_components[0].id));
					m_data.m_output_to_framebuffer.Append(TRUE);
					m_data.m_output_to_file.Append(TRUE);
					m_data.m_output_to_jpeg.Append(FALSE);
				}


				for (auto &value : m_data.m_selected_layers)
				{
					Int32 Position = ItemId - 1;
					maxon::String name = value;
					data.SetString('name', maxon::String(name));
					data.SetInt32('Dspl', m_data.m_output_to_framebuffer[Position] ? DL_DISPLAY_ON : DL_DISPLAY_OFF);
					data.SetInt32('Fldr', m_data.m_output_to_file[Position] ? DL_FOLDER_ON : DL_FOLDER_OFF);
					data.SetInt32('Jpg',  m_data.m_output_to_jpeg[Position] ? DL_JPG_ON : DL_JPG_OFF);
					m_aov_listview.SetItem(ItemId++, data);

				}
				m_aov_listview.DataChanged();
			}
			break;
		}

		case AOV_REMOVE:
		{
			Int Size = m_aov_listview.GetItemCount();
			Int32 i, a, b, dec = 1;
			Int32 RemPos = 0;
			Int32 pos = -1;
			for (i = 0; m_selection->GetRange(i, LIMIT<Int32>::MAX, &a, &b); i++)
			{
				for (; a <= b; a++)
				{
					Int32 SelectedItemId = a - dec;

					m_data.m_selected_layers.Erase(SelectedItemId);
					m_data.m_selected_id.Erase(SelectedItemId);
					m_data.m_output_to_framebuffer.Erase(SelectedItemId);
					m_data.m_output_to_file.Erase(SelectedItemId);
					m_data.m_output_to_jpeg.Erase(SelectedItemId);
					dec++;
				}
			}

			for (Int i = 1; i <= Size; i++)
			{
				m_aov_listview.RemoveItem(i);
			}

			Int ItemId = 1;
			BaseContainer data;

			if (m_data.m_selected_layers.GetCount() == 0)
			{
				m_data.m_selected_layers.Insert(0, maxon::String(s_shading_components[0].name));
				m_data.m_selected_id.Insert(0, maxon::Int32(s_shading_components[0].id));
				m_data.m_output_to_framebuffer.Append(TRUE);
				m_data.m_output_to_file.Append(TRUE);
				m_data.m_output_to_jpeg.Append(FALSE);
			}

			for (auto &value : m_data.m_selected_layers)
			{
				Int32 Position = ItemId - 1;
				maxon::String name = value;
				data.SetString('name', maxon::String(name));
				data.SetInt32('Dspl', m_data.m_output_to_framebuffer[Position] ? DL_DISPLAY_ON : DL_DISPLAY_OFF);
				data.SetInt32('Fldr', m_data.m_output_to_file[Position] ? DL_FOLDER_ON : DL_FOLDER_OFF);
				data.SetInt32('Jpg',  m_data.m_output_to_jpeg[Position] ? DL_JPG_ON : DL_JPG_OFF);
				m_aov_listview.SetItem(ItemId++, data);
			}
			m_aov_listview.DataChanged();
			m_update_buttons();
			break;
		}

		case AOV_DUPLICATE:
		{
			AutoAlloc<BaseSelect> s2;
			if (m_selection && s2)
			{
				Int32 i, id2, count = m_aov_listview.GetItemCount();
				Int32 cnt = 0;
				BaseContainer test;

				for (i = 0; i < count; i++)
				{
					m_aov_listview.GetItemLine(i, &id2, &test);
				}

				if (m_aov_listview.GetSelection(m_selection))
				{
					Int32	 a, b;
					BaseContainer data;
					AutoAlloc<BaseSelect> SelectedDuplicate;

					for (int i = 1; i <= count; i++)
					{
						if (m_selection->IsSelected(i))
						{
							String name		  =  m_data.m_selected_layers[i - 1];
							Bool framebuffer  =  m_data.m_output_to_framebuffer[i - 1];
							Bool file		  =  m_data.m_output_to_file[i - 1];
							Bool jpeg		  =  m_data.m_output_to_jpeg[i - 1];
							Int32 SelectedID  =  m_data.m_selected_id[i-1];
	
							Int32 Position = count;
						
							m_data.m_selected_layers.Append(name);
							m_data.m_selected_id.Append(SelectedID);
							m_data.m_output_to_framebuffer.Append(framebuffer);
							m_data.m_output_to_file.Append(file);
							m_data.m_output_to_jpeg.Append(jpeg);
					
							data.SetString('name', name);
							data.SetInt32('Dspl', m_data.m_output_to_framebuffer[Position] ? DL_DISPLAY_ON : DL_DISPLAY_OFF);
							data.SetInt32('Fldr', m_data.m_output_to_file[Position] ? DL_FOLDER_ON : DL_FOLDER_OFF);
							data.SetInt32('Jpg', m_data.m_output_to_jpeg[Position] ? DL_JPG_ON : DL_JPG_OFF);
							m_aov_listview.SetItem(++count, data);
							SelectedDuplicate->Select(count);
						}
					}
					m_aov_listview.SetSelection(SelectedDuplicate);
					m_aov_listview.DataChanged();
				}
			}
			m_update_buttons();
			break;
		}

		case AOV_VIEW:
		{
			system("C:\\Users\\Ogers\\Documents\\filename.exr");
		}

	}

	/**
	Here we get the data from the custom UI and store it so whenever this class in instantiated 
	its values will remain unchanged
	*/
	BaseContainer m(i_msg);
	m.SetInt32(BFM_ACTION_ID, GetId());
	m.SetData(BFM_ACTION_VALUE, this->GetData().GetValue());
	SendParentMessage(m);
	m_aov_listview.DataChanged();
	return true;
}


Bool AOVDialog::Command(Int32 i_id, const BaseContainer& i_msg)
{
	switch (i_id)
	{
		case BUTTON_OK:
		{
			m_aov_selector_selected.Reset();
			m_aov_selector_selectedID.Reset();
			m_data->m_check = false;
			AutoAlloc<BaseSelect> s2;
			if (m_selection && s2)
			{
				Int32 i, id2, count = m_shading_listview.GetItemCount();
				BaseContainer test;

				for (i = 0; i < count; i++)
				{
					m_shading_listview.GetItemLine(i, &id2, &test);
				}

				if (m_shading_listview.GetSelection(m_selection))
				{
					Int32 a, b;
					BaseContainer data;

					for (i = 0; s_shading_components[i].id; i++)
					{
						if (m_selection->IsSelected(s_shading_components[i].id))
						{
							maxon::String name = maxon::String(s_shading_components[i].name);
							maxon::Int32 Id = maxon::Int32(s_shading_components[i].id);
							m_aov_selector_selected.Append(name);
							m_aov_selector_selectedID.Append(Id);
						}
					}
				}
			}

			AutoAlloc<BaseSelect> s3;
			if (m_selection && s3)
			{
				Int32					i, id2, count = m_auxiliary_listview.GetItemCount();
				BaseContainer test;

				for (i = 0; i < count; i++)
				{
					m_auxiliary_listview.GetItemLine(i, &id2, &test);
				}
				
				if (m_auxiliary_listview.GetSelection(m_selection))
				{
					Int32	 a, b;
					BaseContainer data;

					for (i = 0; s_auxiliary_variables[i].id; i++)
					{
						if (m_selection->IsSelected(s_auxiliary_variables[i].id))
						{
							maxon::String name = maxon::String(s_auxiliary_variables[i].name);
							maxon::Int32 Id = maxon::Int32(s_auxiliary_variables[i].id);
							m_aov_selector_selected.Append(name);
							m_aov_selector_selectedID.Append(Id);

						}
					}
				}
			}
			m_update_buttons();
			this->Close();
			break;
		}

		case BUTTON_CANCEL:
		{
			this->Close();
			break;
		}
	}
	return true;
}


/**
	Called to update the custom GUI to display the value in @formatParam{tristate}.
*/
Bool iCustomDataTypeAOV::SetData(const TriState<GeData> &i_tristate)
{
	
	iCustomDataTypeLayers* data = (iCustomDataTypeLayers*)(i_tristate.GetValue().GetCustomDataType(ID_CUSTOMDATATYPE_LAYERS));
	if (data)
	{
		iferr(m_data.m_selected_layers.CopyFrom(data->m_selected_layers))
			return false;

		iferr(m_data.m_selected_id.CopyFrom(data->m_selected_id))
			return false;

		iferr(m_data.m_output_to_framebuffer.CopyFrom(data->m_output_to_framebuffer))
			return false;

		iferr(m_data.m_output_to_file.CopyFrom(data->m_output_to_file))
			return false;

		iferr(m_data.m_output_to_jpeg.CopyFrom(data->m_output_to_jpeg))
			return false;

		m_data.m_check = data->m_check;
		m_aov_listview.DataChanged();
	}

	return true;
};



// Called to retrieve the value(s) currently displayed.
TriState<GeData> iCustomDataTypeAOV::GetData()
{
	TriState<GeData> tri;
	tri.Add(GeData(ID_CUSTOMDATATYPE_LAYERS, m_data));
	return tri;
};

static Int32 g_stringtable[] = { ID_CUSTOMDATATYPE_LAYERS }; // This array defines the applicable datatypes.

// This CustomGuiData class registers a new custom GUI for the Layers datatype.
class DelightCustonGuiLayers : public CustomGuiData
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
Int32 DelightCustonGuiLayers::GetId()
{
	return ID_CUSTOMGUI_LAYERS;
};

/**
We override Alloc function to create a new instance of the plugin.It is used instead of a constructor
at this one provide means to return an error if something goes wrong.
*/
CDialog* DelightCustonGuiLayers::Alloc(const BaseContainer& i_settings)
{
	iferr(iCustomDataTypeAOV* dlg = NewObj(iCustomDataTypeAOV, i_settings, GetPlugin()))
		return nullptr;

	CDialog *cdlg = dlg->Get();

	if (!cdlg)
		return nullptr;

	return cdlg;
};


//This function is called when an instance gets freed.
void DelightCustonGuiLayers::Free(CDialog* i_dlg, void* i_userdata)
{
	if (!i_dlg || !i_userdata)
		return;

	iCustomDataTypeAOV* sub = static_cast<iCustomDataTypeAOV*>(i_userdata);
	DeleteObj(sub);
};

// Returns the resource symbol. This symbol can be used in resource files in combination with "CUSTOMGUI".
const Char* DelightCustonGuiLayers::GetResourceSym()
{
	return "DELIGHTLAYERCUSTOMGUI";
};

CustomProperty* DelightCustonGuiLayers::GetProperties()
{
	return nullptr;
};

// Returns the applicable datatypes defined in the stringtable array.
Int32 DelightCustonGuiLayers::GetResourceDataType(Int32*& i_table)
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
class LayersCustomDataTypeClass : public CustomDataTypeClass
{
	INSTANCEOF(LayersCustomDataTypeClass, CustomDataTypeClass)

public:
	virtual Int32 GetId()
	{
		return ID_CUSTOMDATATYPE_LAYERS;
	}

	virtual CustomDataType* AllocData()
	{
		iCustomDataTypeLayers* data = NewObjClear(iCustomDataTypeLayers);
		return data;
	};

	virtual void FreeData(CustomDataType* i_data)
	{
		iCustomDataTypeLayers* d = static_cast<iCustomDataTypeLayers*>(i_data);
		DeleteObj(d);
	}

	/**
	Called to copy an instance of the custom data type.
	Copy the data from @formatParam{src} to @formatParam{dest}.
	*/
	virtual Bool CopyData(const CustomDataType* i_src, CustomDataType* i_dst, AliasTrans* i_aliastrans)
	{
		// copy the data to the given target data
		iCustomDataTypeLayers* s = (iCustomDataTypeLayers*)(i_src);
		iCustomDataTypeLayers* d = (iCustomDataTypeLayers*)(i_dst);

		if (!s || !d)
			return false;

		d->m_selected_layers.Flush();
		iferr(d->m_selected_layers.CopyFrom(s->m_selected_layers))
			return false;

		d->m_selected_id.Flush();
		iferr(d->m_selected_id.CopyFrom(s->m_selected_id))
			return false;

		d->m_output_to_framebuffer.Flush();
		iferr(d->m_output_to_framebuffer.CopyFrom(s->m_output_to_framebuffer))
			return false;

		d->m_output_to_file.Flush();
		iferr(d->m_output_to_file.CopyFrom(s->m_output_to_file))
			return false;

		d->m_output_to_jpeg.Flush();
		iferr(d->m_output_to_jpeg.CopyFrom(s->m_output_to_jpeg))
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
		const iCustomDataTypeLayers* const d = static_cast<const iCustomDataTypeLayers*>(i_t_d);

		const maxon::Int length = d->m_selected_layers.GetCount();
		i_hf->WriteInt64((Int64)length);

		for (Int64 i = 0; i < length; ++i)
		{
			i_hf->WriteString(d->m_selected_layers[i]);
			i_hf->WriteInt32(d->m_selected_id[i]);
			i_hf->WriteBool(d->m_output_to_framebuffer[i]);
			i_hf->WriteBool(d->m_output_to_file[i]);
			i_hf->WriteBool(d->m_output_to_jpeg[i]);
			
		}

		return true;
	}

	//Called to read the custom data type from a file.
	virtual Bool ReadData(CustomDataType* i_t_d, HyperFile* i_hf, Int32 i_level)
	{

		iCustomDataTypeLayers* const d = static_cast<iCustomDataTypeLayers*>(i_t_d);

		if (i_level > 0)
		{
			Int64 length = 0;
			if (i_hf->ReadInt64(&length))
			{
				for (Int64 i = 0; i < length; ++i)
				{
					String layer;
					Int32 layerId;
					Bool output_to_framebuffer;
					Bool output_to_file;
					Bool output_to_jpeg;
					if (i_hf->ReadString(&layer))
					{
						iferr(d->m_selected_layers.Append(layer))
							return false;
					}

					if (i_hf->ReadInt32(&layerId)) 
					{
						iferr(d->m_selected_id.Append(layerId))
							return false;
					}

					if (i_hf->ReadBool(&output_to_framebuffer))
					{
						iferr(d->m_output_to_framebuffer.Append(output_to_framebuffer))
							return false;
					}

					if (i_hf->ReadBool(&output_to_file))
					{
						iferr(d->m_output_to_file.Append(output_to_file))
							return false;
					}

					if (i_hf->ReadBool(&output_to_jpeg))
					{
						iferr(d->m_output_to_jpeg.Append(output_to_jpeg))
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
		return "DELIGHTLAYERCUSTOMTYPE";
	}

	virtual void GetDefaultProperties(BaseContainer &i_data)
	{
		// the default values of this datatype
		// use the custom GUI as default
		i_data.SetInt32(DESC_CUSTOMGUI, ID_CUSTOMGUI_LAYERS);
		i_data.SetInt32(DESC_ANIMATE, DESC_ANIMATE_ON);
	}
};

/**
	With this function we register the custom GUI plugin that we have alread build.
*/
Bool RegisterCustomListView()
{
	if (!RegisterCustomDataTypePlugin(
		GeLoadString(IDS_CUSTOMDATATYPE_DOTS),
		CUSTOMDATATYPE_INFO_LOADSAVE |
		CUSTOMDATATYPE_INFO_TOGGLEDISPLAY |
		CUSTOMDATATYPE_INFO_HASSUBDESCRIPTION,
		NewObjClear(LayersCustomDataTypeClass),
		1))
		return false;


	static BaseCustomGuiLib myStringGUIlib;

	ClearMem(&myStringGUIlib, sizeof(myStringGUIlib));
	FillBaseCustomGui(myStringGUIlib);

	InstallLibrary(ID_CUSTOMGUI_LAYERS, &myStringGUIlib, 1000, sizeof(myStringGUIlib));
	
	RegisterCustomGuiPlugin(GeLoadString(IDS_CUSTOMGUISTRING), 0, NewObjClear(DelightCustonGuiLayers));

	return true;
}


