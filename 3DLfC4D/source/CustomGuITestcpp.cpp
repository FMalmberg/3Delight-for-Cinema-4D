//#include "c4d.h"
//#include "c4d_symbols.h"
//#include "c4d_basebitmap.h"
//#include "3DlghtRenderer.h"
//#include <list>
//#include <vector>
//#include <string>
//#include <algorithm>
//#include <map>
//
//using namespace std;
////----------------------------------------------------------------------------------------
///// This simple example shows how to use the CustomGuiData and iCustomGui classes to create custom GUI elements.
///// Such custom GUI elements can be added to a GeDialog using AddCustomGui() or can be used in a resource file. 
///// This example GUI can be applied to string parameters. It will display the string in a default text field but additionally it will show the number of characters in an extra text field.
/////
///// A CustomGuiData based plugin is used to register the new custom GUI in Cinema 4D. It defines various properties, the resource symbol and the applicable datatypes. \n
///// The iCustomGui based class is the actual GUI. This subdialog is used to display custom elements and is created by the CustomGuiData plugin. The actual user interaction happens in this iCustomGui based class.
/////
////----------------------------------------------------------------------------------------
//
//
//#define ID_SDK_EXAMPLE_CUSTOMGUI_STRING 1034666
//
//#define TEXT_ID 10001 ///< The ID of the string text field GUI element.
//#define COUNT_ID 10002 ///< The ID of the number text field GUI element.
//# define ID_SDK_EXAMPLE_CUSTOMDATATYPE_TEST 1245665
////----------------------------------------------------------------------------------------
///// A custom GUI to display a string with an additional text field showing the number of characters.
////----------------------------------------------------------------------------------------ID_SDK_EXAMPLE_CUSTOMDATATYPE_TEST
//
//Int counterrr=1;
//bool check = false;
//
//struct TestData
//{
//	Int32 id;
//	Char	name[50];
//};
//
//TestData g_testdata[] =
//{
//	{ 50, "RGBA (color + alpha)" },
//	{ 51, "Diffuse" },
//	{ 52, "Subsurface scattering" },
//	{ 53, "Reflection" },
//	{ 54, "Refraction" },
//	{ 55, "Incandescence" },
//	{ 0, "" }
//};
//
//
//TestData AuxiliaryVariables[] =
//{
//	{ 100, "Z (depth)" },
//	{ 101, "Camera space position" },
//	{ 102, "Camera space normal" },
//	{ 103, "UV" },
//	{ 104, "Geometry ID" },
//	{ 105, "Scene Path ID" },
//	{ 106, "Relighting Multiplier" },
//	{ 0, "" }
//};
//
//
//std::vector<String> Duplicated;
//std::vector<Int32> DuplicatedId;
//std::vector<String> added;
//std::vector<Int32> addedId;
//std::map<String, Int32> DisplayIc;
//std::vector<Int32> DisplayIcon;
//
//
//class AovCustomDataTypeClass; // forward declaration
//
////---------------------------
///// the actual data
////---------------------------
//class iCustomDataTypeAOV : public iCustomDataType<iCustomDataTypeAOV>
//{
//	friend class AovCustomDataTypeClass;
//	
//public:
//
//	// an array of Vectors
//	maxon::BaseArray<Vector> _points;
//
//	iCustomDataTypeAOV()
//	{
//
//	}
//};
//
//
//
//static void ApplicationOutputF(const Char* format, ...)
//{
//	va_list arp;
//	Char		buf[1024];
//
//	va_start(arp, format);
//	vsprintf_safe(buf, sizeof(buf), format, arp);
//	ApplicationOutput(String(buf));
//	va_end(arp);
//	
//}
//
//
//
//class IAovLayersCustomGui : public iCustomGui
//
//{
//	INSTANCEOF(IAovLayersCustomGui, iCustomGui)
//
//private:
//
//	iCustomDataTypeAOV _data;
//	Bool _tristate; ///< The current tristate.
//	SimpleListView				Shading_Listview;
//	SimpleListView				Auxiliary_Listview;
//	SimpleListView				Aov_Listview;
//	AutoAlloc<BaseSelect>	selection;
//	Int32	counter2;
//	BaseContainer data;
//
//
//
//	void UpdateButtons();
//
//public:
//
//	Int32 DisplayId = DL_DISPLAY_ON;
//	Int32 FolderId = DL_FOLDER_ON;
//	IAovLayersCustomGui(const BaseContainer &settings, CUSTOMGUIPLUGIN *plugin);
//	virtual Bool CreateLayout();
//	virtual Bool InitValues();
//	//virtual Int32 Message(const BaseContainer& msg, BaseContainer& result);
//	virtual Bool Command(Int32 id, const BaseContainer &msg);
//
//	//virtual Bool SetData(const TriState<GeData> &tristate);
//	//virtual TriState<GeData> GetData();
//};
//
//
//IAovLayersCustomGui::IAovLayersCustomGui(const BaseContainer &settings, CUSTOMGUIPLUGIN *plugin) : iCustomGui(settings, plugin)
//{
//	// Defining default values
//	counter2 = 0;
//	_tristate = false;
//};
//
//Bool IAovLayersCustomGui::CreateLayout()
//{
//	Bool res = GeDialog::CreateLayout();
//		res = LoadDialogResource(AOV_SELECTOR, nullptr, 0);
//
//		if (res)
//		{
//			Aov_Listview.AttachListView(this, GADGET_LISTVIEW3);
//		}
//	
//	return res;
//};
//
//void IAovLayersCustomGui::UpdateButtons()
//{
//	
//	if (!selection) {
//		Enable(AOV_ADD, FALSE);
//		return;
//	}
//	Enable(AOV_REMOVE, Aov_Listview.GetSelection(selection) != 0);
//	Enable(AOV_DUPLICATE, Aov_Listview.GetSelection(selection) != 0);
//	Enable(AOV_VIEW, Aov_Listview.GetSelection(selection) != 0);
//}
//
//
//Bool IAovLayersCustomGui::InitValues()
//{
//	if (!GeDialog::InitValues())
//		return false;
//	
//
//	BaseContainer layout;
//	Int32					i = 0;
//
//	layout = BaseContainer();
//	//layout.SetInt32('chck', LV_COLUMN_CHECKBOX);
//	layout.SetInt32('Dspl', LV_COLUMN_BMP);
//	layout.SetInt32('Fldr', LV_COLUMN_BMP);
//	layout.SetInt32('Jpg', LV_COLUMN_BMP);
//	layout.SetInt32('name', LV_COLUMN_TEXT);
//	//layout.SetInt32('bttn', LV_COLUMN_BUTTON);
//	Aov_Listview.SetLayout(4, layout);
//
//	Aov_Listview.SetProperty(SLV_MULTIPLESELECTION, true);
//
//	//data = BaseContainer();
//
//	if (added.size()==0)
//	{
//		added.insert(added.begin(),g_testdata[0].name);
//		addedId.insert(addedId.begin(),g_testdata[0].id);
//		DisplayIcon.insert(DisplayIcon.begin(), DL_DISPLAY_ON);
//		
//		for (i = 0; g_testdata[i].id; i++)
//		{
//			String val = g_testdata[i].name;
//			DisplayIc[val] = DL_DISPLAY_ON;
//		}
//
//
//		for (i = 0; AuxiliaryVariables[i].id; i++)
//		{
//			String val = AuxiliaryVariables[i].name;
//			DisplayIc[val] = DL_DISPLAY_ON;
//		}
//
//	}
//	
//	counterrr = 1;
//	for (String &value : added) {
//
//		
//		data.SetString('name', String(value));
//		data.SetInt32('Dspl', DisplayIc[value]);
//		data.SetInt32('Fldr', FolderId);
//		data.SetInt32('Jpg', DL_JPG);
//
//		Aov_Listview.SetItem(counterrr, data);
//		counterrr++;
//	}
//
//	/*data = BaseContainer();
//	if (!Duplicated.empty()) {
//		for (String &value : Duplicated) {
//			data.SetString('name', String(value));
//			Aov_Listview.SetItem(counterrr++, data);
//		}
//	 }*/
//
//
//
//	Aov_Listview.DataChanged();
//
//	UpdateButtons();
//
//	return true;
//};
//
//
//class AOVDialog : public GeDialog
//{
//
//	friend class IAovLayersCustomGui;
//
//private:
//	SimpleListView				Shading_Listview;
//	SimpleListView				Auxiliary_Listview;
//	SimpleListView				listview4;
//	AutoAlloc<BaseSelect>	selection;
//	Int32									counter2;
//	Int32 Start = 50;
//
//	void UpdateButtons();
//
//public:
//	AOVDialog();
//	virtual ~AOVDialog();
//	std::list<String> m_selected;
//	std::list<Int32> m_selectedID;
//	virtual Bool CreateLayout();
//	virtual Bool InitValues();
//	virtual Bool Command(Int32 id, const BaseContainer& msg);
//	virtual Int32 Message(const BaseContainer& msg, BaseContainer& result);
//
//};
//
//
//
//AOVDialog::AOVDialog()
//{
//	counter2 = 0;
//}
//
//AOVDialog::~AOVDialog()
//{
//}
//
//
//Bool AOVDialog::CreateLayout()
//{
//	// first call the parent instance
//	Bool res = GeDialog::CreateLayout();
//
//	res = LoadDialogResource(AOV_SELECTOR_DIALOG, nullptr, 0);
//
//	if (res)
//	{
//		Shading_Listview.AttachListView(this, GADGET_LISTVIEW1);
//		Auxiliary_Listview.AttachListView(this, GADGET_LISTVIEW2);
//	}
//
//	return res;
//}
//
//
//void AOVDialog::UpdateButtons()
//{
//	if (!selection)
//		return;
//
//	Enable(BUTTON_OK, Shading_Listview.GetSelection(selection) != 0);
//	Enable(BUTTON_CANCEL, Auxiliary_Listview.GetSelection(selection) != 0);
//}
//
//
//
//Bool AOVDialog::InitValues()
//{
//	// first call the parent instance
//	if (!GeDialog::InitValues())
//		return false;
//
//	BaseContainer layout;
//	BaseContainer data;
//	Int32					i = 0;
//
//	layout.SetInt32('name', LV_COLUMN_TEXT);
//	//	layout.SetInt32('used',LV_COLUMN_CHECKBOX);
//	Shading_Listview.SetLayout(1, layout);
//	Shading_Listview.SetProperty(SLV_MULTIPLESELECTION,TRUE);
//	
//
//	layout = BaseContainer();
//	layout.SetInt32('name', LV_COLUMN_TEXT);
//
//	//	layout.SetInt32('used',LV_COLUMN_CHECKBOX);
//	Auxiliary_Listview.SetLayout(1, layout);
//	Auxiliary_Listview.SetProperty(SLV_MULTIPLESELECTION, TRUE);
//
//
//	for (auto &value : addedId) {
//		selection->Select(value);
//		Shading_Listview.SetSelection(selection);
//		Auxiliary_Listview.SetSelection(selection);
//	}
//
//
//	data = BaseContainer();
//	for (i = 0; g_testdata[i].id; i++)
//	{
//		data.SetString('name', String(g_testdata[i].name));
//		//data.SetInt32('used',false);
//		Shading_Listview.SetItem(g_testdata[i].id, data);
//	}
//
//
//	for (i = 0; AuxiliaryVariables[i].id; i++)
//	{
//		data.SetString('name', String(AuxiliaryVariables[i].name));
//		//data.SetInt32('used',false);
//		Auxiliary_Listview.SetItem(AuxiliaryVariables[i].id, data);
//	}
//
//	Shading_Listview.DataChanged();
//	Auxiliary_Listview.DataChanged();
//	UpdateButtons();
//
//	return true;
//}
//
//
//
//Int32 AOVDialog::Message(const BaseContainer& msg, BaseContainer& result)
//{
//	//	switch (msg.GetId())
//	{
//	}
//	return GeDialog::Message(msg, result);
//}
//
//
//
//Bool IAovLayersCustomGui::Command(Int32 id, const BaseContainer &msg)
//{
//	switch (id)
//	{
//	case GADGET_LISTVIEW1:
//	case GADGET_LISTVIEW2:
//	case GADGET_LISTVIEW3:
//	{
//		switch (msg.GetInt32(BFM_ACTION_VALUE))
//		{
//		case LV_SIMPLE_SELECTIONCHANGED:
//			ApplicationOutputF("Selection changed, id: %d, val: %p ", msg.GetInt32(LV_SIMPLE_ITEM_ID), msg.GetVoid(LV_SIMPLE_DATA));
//			break;
//
//		case LV_SIMPLE_CHECKBOXCHANGED:
//			ApplicationOutputF("CheckBox changed, id: %d, col: %d, val: %p", msg.GetInt32(LV_SIMPLE_ITEM_ID), msg.GetInt32(LV_SIMPLE_COL_ID), msg.GetVoid(LV_SIMPLE_DATA));
//			break;
//
//		case LV_SIMPLE_FOCUSITEM:
//			ApplicationOutputF("Focus set id: %d, col: %d", msg.GetInt32(LV_SIMPLE_ITEM_ID), msg.GetInt32(LV_SIMPLE_COL_ID));
//			break;
//
//		case LV_SIMPLE_BUTTONCLICK:
//			ApplicationOutputF("Button clicked id: %d, col: %d", msg.GetInt32(LV_SIMPLE_ITEM_ID), msg.GetInt32(LV_SIMPLE_COL_ID));
//			break;
//
//		case LV_RES_BITMAP:
//			ApplicationOutputF("Bitmap clicked id: %d, col: %d", msg.GetInt32(LV_SIMPLE_ITEM_ID), msg.GetInt32(LV_SIMPLE_COL_ID));
//		}
//		
//
//
//
//		Int32 ColId = msg.GetInt32(LV_SIMPLE_COL_ID);
//
//
//		if (ColId == 'Dspl')
//		{
//		// Retrieves all data of this lines
//		Int32 itemId = msg.GetInt32(LV_SIMPLE_ITEM_ID);
//		Aov_Listview.GetItem(itemId, &data);
//
//		// Get the current data of our int(bmp) column / BaseContainer
//		Int32 currIconId = data.GetInt32('Dspl');
//		String name = data.GetString('name');
//		switch (currIconId)
//		{
//		case DL_DISPLAY_ON:
//			DisplayId = DL_DISPLAY;
//			DisplayIc[name] = DisplayId;
//			data.SetInt32('Dspl', DisplayId);
//			break;
//		case DL_DISPLAY:
//			DisplayId = DL_DISPLAY_ON;
//			DisplayIc[name] = DisplayId;
//			data.SetInt32('Dspl', DisplayId);
//			break;
//		default:
//			data.SetInt32('Dspl', DisplayId);
//			break;
//		}
//
//		// Push back our change to the item
//		Aov_Listview.SetItem(itemId, data);
//
//		// Update the ListView
//		Aov_Listview.DataChanged();
//		}
//
//
//
//		else if (ColId == 'Fldr')
//		{
//			// Retrieves all data of this lines
//			Int32 itemId = msg.GetInt32(LV_SIMPLE_ITEM_ID);
//			Aov_Listview.GetItem(itemId, &data);
//
//			// Get the current data of our int(bmp) column / BaseContainer
//			Int32 currIconId = data.GetInt32('Fldr');
//			switch (currIconId)
//			{
//			case DL_FOLDER_ON:
//				FolderId = DL_FOLDER;
//				data.SetInt32('Fldr', FolderId);
//				break;
//			case DL_FOLDER:
//				FolderId = DL_FOLDER_ON;
//				data.SetInt32('Fldr', FolderId);
//				break;
//			default:
//				data.SetInt32('Fldr', FolderId);
//				break;
//			}
//
//			// Push back our change to the item
//			Aov_Listview.SetItem(itemId, data);
//
//			// Update the ListView
//			Aov_Listview.DataChanged();
//		}
//
//
//		else if (ColId == 'Jpg')
//		{
//			// Retrieves all data of this lines
//			Int32 itemId = msg.GetInt32(LV_SIMPLE_ITEM_ID);
//			Aov_Listview.GetItem(itemId, &data);
//
//			// Get the current data of our int(bmp) column / BaseContainer
//			Int32 currIconId = data.GetInt32('Jpg');
//			switch (currIconId)
//			{
//			case DL_JPG_ON:
//				data.SetInt32('Jpg', DL_JPG);
//				break;
//			case DL_JPG:
//				data.SetInt32('Jpg', DL_JPG_ON);
//				break;
//			default:
//				data.SetInt32('Jpg', DL_JPG_ON);
//				break;
//			}
//
//			// Push back our change to the item
//			Aov_Listview.SetItem(itemId, data);
//
//			// Update the ListView
//			Aov_Listview.DataChanged();
//		}
//
//
//
//		UpdateButtons();
//		break;
//	}
//
//
//	case AOV_ADD:
//	{
//
//		check = true;
//		
//		
//		Int32 Size = Aov_Listview.GetItemCount(); //Get size before ListView is updated otherwise it will cause malfunction 
//		AOVDialog dlg; 
//		dlg.Open(DLG_TYPE::MODAL, 10, -1, -1, 500, 200);	
//		
//		if (!check)
//		{
//			Int32 decrement = 0;
//			Int32 Poss = 0;
//			for (Int i = 1; i <= Size + 2; i++)
//			{
//				Aov_Listview.RemoveItem(i);
//			}
//
//			counterrr = 1;
//
//			for (auto &value : dlg.m_selectedID)
//			{
//				Int32 Id = value;
//				if (!(std::find(addedId.begin(), addedId.end(), Id) != addedId.end()))
//				{
//					addedId.push_back(Id);
//				}
//			}
//
//			//BaseContainer data;
//			for (auto &value : dlg.m_selected)
//			{
//				String name = value;
//				if (!(std::find(added.begin(), added.end(), name) != added.end()))
//				{
//					added.push_back(name);
//				}
//			
//			}
//			for (auto &value : added) {				
//				String name = value;
//				data.SetString('name', String(name));
//				data.SetInt32('Dspl', DisplayIc[value]);
//				Aov_Listview.SetItem(counterrr++, data);
//			}
//
//			//Check if duplicated is on selected list or not.
//			std::list<Int> removeDuplicates;
//			for (auto &value : Duplicated) {
//				String name = value;
//
//				if (!(std::find(dlg.m_selected.begin(), dlg.m_selected.end(), name) != dlg.m_selected.end()))
//				{
//					Int32 pos = find(Duplicated.begin()+Poss, Duplicated.end(), name) - Duplicated.begin();
//					removeDuplicates.push_back(pos);
//					ApplicationOutput(name);
//					ApplicationOutput(name +" NOT FOUNDDD at POSITION" + String::IntToString(pos)+" Array Size "+String::IntToString((Int32)Duplicated.size()));
//					Poss++;
//				}
//			}
//
//			for (auto &value : removeDuplicates) {
//				Int32 ID = value-decrement;
//				Duplicated.erase(Duplicated.begin() + ID);
//				DuplicatedId.erase(DuplicatedId.begin() + ID);
//				decrement++;
//			}
//
//
//
//
//			if (!Duplicated.empty()) {
//				for (auto &value : Duplicated) {
//					added.push_back(value);
//					data.SetString('name', String(value));
//					Aov_Listview.SetItem(counterrr++, data);
//				}
//			}
//
//			if (!DuplicatedId.empty()) {
//				for (auto &value : DuplicatedId) {
//					addedId.push_back(value);
//				}
//			}
//
//			if (added.size() == 0)
//			{
//				data = BaseContainer();
//				added.insert(added.begin(), g_testdata[0].name);
//				addedId.insert(addedId.begin(), g_testdata[0].id);
//				data.SetString('name', (String)g_testdata[0].name);
//				Aov_Listview.SetItem(1, data);
//			}
//			Aov_Listview.DataChanged();
//			Int32 size = added.size();
//		}
//		break;
//	}
//
//
//
//	case AOV_REMOVE: {
//
//			Int Size =Aov_Listview.GetItemCount();
//			Int32 i, a, b,dec=1,decc=1;
//			Int32 RemPos = 0;
//			Int32 pos = -1;
//			for (i = 0; selection->GetRange(i, LIMIT<Int32>::MAX, &a, &b); i++)
//			{
//				for (; a <= b; a++)
//				{
//					//if (a != 1) 
//					{
//						Int32 sss = added.size();
//						Int32 aa = a-dec;
//						String name = added[aa];
//						bool ToRemove = false;
//
//						RemPos = 0;
//						for (auto &value : Duplicated) {
//							String remove = value;
//							if (name == remove) {
//								pos = RemPos;
//								ToRemove = true;
//								break;
//							}
//							RemPos++;
//						}
//						ApplicationOutput("Array Size " + String::IntToString((Int32)Duplicated.size()) + " Position " + String::IntToString(pos));
//
//						if (ToRemove) {
//							Duplicated.erase(Duplicated.begin() + pos);
//							DuplicatedId.erase(DuplicatedId.begin() + pos);
//						}
//
//
//						ApplicationOutput("Array Size " + String::IntToString(sss)+ " Position "+ String::IntToString(aa));
//						DisplayIc[added[aa]] = DL_DISPLAY_ON;
//						added.erase(added.begin() + aa);
//						addedId.erase(addedId.begin() + aa);
//						dec++;
//					}
//	
//				}
//			}
//
//			for (Int i = 1; i <= Size; i++)
//			{
//				ApplicationOutput(String::IntToString(i));
//				Aov_Listview.RemoveItem(i);
//			}
//
//			ApplicationOutput(String::IntToString(Aov_Listview.GetItemCount()));
//
//			Aov_Listview.DataChanged();
//
//			Int cnt = 1;
//			//BaseContainer data;
//
//			for (auto &value : added)
//			{
//					ApplicationOutput(String::IntToString(cnt)+"  "+String(value));
//					data.SetString('name', String(value));
//					data.SetInt32('Dspl', DisplayIc[value]);
//					Aov_Listview.SetItem(cnt++, data);
//			}
//
//		
//			if (added.size() == 0)
//			{
//				data = BaseContainer();
//				added.insert(added.begin(), g_testdata[0].name);
//				addedId.insert(addedId.begin(), g_testdata[0].id);
//				data.SetString('name', String(g_testdata[0].name));
//				Aov_Listview.SetItem(1, data);
//			}
//
//			Aov_Listview.DataChanged();
//		
//		UpdateButtons();
//		break;
//	}
//
//	case AOV_DUPLICATE:
//	{
//		AutoAlloc<BaseSelect> s2;
//		if (selection && s2)
//		{
//			// TEST
//			Int32					i, id2, count = Aov_Listview.GetItemCount();
//			Int32 cnt = 0;
//			BaseContainer test;
//
//			for (i = 0; i < count; i++)
//			{
//				Aov_Listview.GetItemLine(i, &id2, &test);
//			}
//			// TEST
//
//			if (!Aov_Listview.GetSelection(selection))
//			{
//				ApplicationOutput("No Selection"_s);
//			}
//			else
//			{
//				Int32	 a, b;
//				String str;
//				for (i = 0; selection->GetRange(i, LIMIT<Int32>::MAX, &a, &b); i++)
//				{
//					if (a == b)
//						str += String::IntToString(a) + " ";
//					else
//						str += String::IntToString(a) + "-" + String::IntToString(b) + " ";
//				}
//				//				str.Delete(str.GetLength()-1,1);
//				ApplicationOutput("Selection: " + str);
//
//				//BaseContainer data;
//				for (int i=1; i<=count; i++)
//				{
//					ApplicationOutput(String::IntToString(count) + "  " + String::IntToString(i));
//
//					if (selection->IsSelected(i))
//					{
//						String name = added[i - 1];
//
//						Int32 SelectedID;
//						for (int j = 0; g_testdata[j].id; j++)
//						{
//							if (name == g_testdata[j].name)
//								SelectedID = g_testdata[j].id;
//						}
//
//						for (int k = 0; AuxiliaryVariables[k].id; k++)
//						{
//							if (name == AuxiliaryVariables[k].name)
//								SelectedID = AuxiliaryVariables[k].id;
//						}
//
//						ApplicationOutput(String::IntToString(count));
//						data.SetString('name', name);
//						added.insert(added.begin()+count,name);
//						//DisplayIc[name + String::IntToString()] = 1;
//						addedId.insert(addedId.begin() + count,SelectedID);
//						Duplicated.push_back(name);
//						DuplicatedId.push_back(SelectedID);
//						Aov_Listview.SetItem(++count, data);
//						ApplicationOutput(String::IntToString(count));
//
//					}
//				}
//				Aov_Listview.SetSelection(selection);
//				Aov_Listview.DataChanged();
//			}
//		}
//		UpdateButtons();
//		break;
//	}
//
//	}
//	return true;
//}
//
//
//
//
//
//
//Bool AOVDialog::Command(Int32 id, const BaseContainer& msg)
//{
//	switch (id)
//	{
//	case GADGET_LISTVIEW1:
//	case GADGET_LISTVIEW2:
//	{
//		switch (msg.GetInt32(BFM_ACTION_VALUE))
//		{
//		case LV_SIMPLE_SELECTIONCHANGED:
//			ApplicationOutputF("Selection changed, id: %d, val: %p ", msg.GetInt32(LV_SIMPLE_ITEM_ID), msg.GetVoid(LV_SIMPLE_DATA));
//			break;
//
//		case LV_SIMPLE_CHECKBOXCHANGED:
//			ApplicationOutputF("CheckBox changed, id: %d, col: %d, val: %p", msg.GetInt32(LV_SIMPLE_ITEM_ID), msg.GetInt32(LV_SIMPLE_COL_ID), msg.GetVoid(LV_SIMPLE_DATA));
//			break;
//
//		case LV_SIMPLE_FOCUSITEM:
//			ApplicationOutputF("Focus set id: %d, col: %d", msg.GetInt32(LV_SIMPLE_ITEM_ID), msg.GetInt32(LV_SIMPLE_COL_ID));
//			break;
//
//		case LV_SIMPLE_BUTTONCLICK:
//			ApplicationOutputF("Button clicked id: %d, col: %d", msg.GetInt32(LV_SIMPLE_ITEM_ID), msg.GetInt32(LV_SIMPLE_COL_ID));
//			break;
//		}
//		UpdateButtons();
//		break;
//	}
//
//	//		case GADGET_LISTVIEW2:
//	//			break;
//
//	case BUTTON_OK:
//	{
//		check = false;
//		added.resize(0);
//		addedId.resize(0);
//		AutoAlloc<BaseSelect> s2;
//		if (selection && s2)
//		{
//			// TEST
//			Int32					i, id2, count = Shading_Listview.GetItemCount();
//			BaseContainer test;
//
//			for (i = 0; i < count; i++)
//			{
//				Shading_Listview.GetItemLine(i, &id2, &test);
//			}
//			// TEST
//
//			if (!Shading_Listview.GetSelection(selection))
//			{
//				ApplicationOutput("No Selection"_s);
//			}
//			else
//			{
//				Int32	 a, b;
//				String str;
//				for (i = 0; selection->GetRange(i, LIMIT<Int32>::MAX, &a, &b); i++)
//				{
//					if (a == b)
//						str += String::IntToString(a) + " ";
//					else
//						str += String::IntToString(a) + "-" + String::IntToString(b) + " ";
//				}
//				//				str.Delete(str.GetLength()-1,1);
//				ApplicationOutput("Selection: " + str);
//
//				BaseContainer data;
//				for (i = 0; g_testdata[i].id; i++)
//				{
//					if (selection->IsSelected(g_testdata[i].id))
//					{
//						data.SetInt32('chck', true);
//						data.SetString('name', String(g_testdata[i].name));
//
//						m_selected.push_back(g_testdata[i].name);
//						m_selectedID.push_back(g_testdata[i].id);
//						//data.SetString('bttn', "..."_s);
//						selection->Select(counter2);
//						listview4.SetItem(counter2++, data);
//					}
//				}
//				listview4.SetSelection(selection);
//				listview4.DataChanged();
//			}
//		}
//
//
//		AutoAlloc<BaseSelect> s3;
//		if (selection && s3)
//		{
//			// TEST
//			Int32					i, id2, count = Auxiliary_Listview.GetItemCount();
//			BaseContainer test;
//
//			for (i = 0; i < count; i++)
//			{
//				Auxiliary_Listview.GetItemLine(i, &id2, &test);
//			}
//			// TEST
//
//			if (!Auxiliary_Listview.GetSelection(selection))
//			{
//				ApplicationOutput("No Selection"_s);
//			}
//			else
//			{
//				Int32	 a, b;
//				String str;
//				for (i = 0; selection->GetRange(i, LIMIT<Int32>::MAX, &a, &b); i++)
//				{
//					if (a == b)
//						str += String::IntToString(a) + " ";
//					else
//						str += String::IntToString(a) + "-" + String::IntToString(b) + " ";
//				}
//				//				str.Delete(str.GetLength()-1,1);
//				ApplicationOutput("Selection: " + str);
//
//				BaseContainer data;
//				for (i = 0; AuxiliaryVariables[i].id; i++)
//				{
//					if (selection->IsSelected(AuxiliaryVariables[i].id))
//					{
//						data.SetInt32('chck', true);
//						data.SetString('name', String(AuxiliaryVariables[i].name));
//
//						m_selected.push_back(AuxiliaryVariables[i].name);
//						m_selectedID.push_back(AuxiliaryVariables[i].id);
//						//edata.SetString('bttn', "..."_s);
//						selection->Select(counter2);
//						listview4.SetItem(counter2++, data);
//					}
//				}
//				listview4.SetSelection(selection);
//				listview4.DataChanged();
//			}
//		}
//		UpdateButtons();
//
//		this->Close();
//		break;
//	}
//
//
//
//
//	case BUTTON_CANCEL:
//	{
//		this->Close();
//	}
//
//	
//
//
//	}
//	return true;
//}
//
//
//
//static Int32 g_stringtable[] = { ID_SDK_EXAMPLE_CUSTOMDATATYPE_TEST }; ///< This array defines the applicable datatypes.
//
//
////---------------------
///// This CustomGuiData class registers a new custom GUI for the String datatype.
////---------------------
//class SDKExampleCustomGUIString : public CustomGuiData
//{
//public:
//	virtual Int32 GetId();
//	virtual CDialog* Alloc(const BaseContainer& settings);
//	virtual void Free(CDialog* dlg, void* userdata);
//	virtual const Char* GetResourceSym();
//	virtual CustomProperty* GetProperties();
//	virtual Int32 GetResourceDataType(Int32*& table);
//
//};
//
//
//
//Int32 SDKExampleCustomGUIString::GetId()
//{
//	return ID_SDK_EXAMPLE_CUSTOMGUI_STRING;
//};
//
//
//CDialog* SDKExampleCustomGUIString::Alloc(const BaseContainer& settings)
//{
//	// Creates and returns a new sub-dialog.
//
//	iferr(IAovLayersCustomGui* dlg = NewObj(IAovLayersCustomGui, settings, GetPlugin()))
//		return nullptr;
//
//	CDialog *cdlg = dlg->Get();
//
//	if (!cdlg)
//		return nullptr;
//
//	return cdlg;
//};
//
//
//
//void SDKExampleCustomGUIString::Free(CDialog* dlg, void* userdata)
//{
//	// Destroys the given subdialog.
//	if (!dlg || !userdata)
//		return;
//
//	IAovLayersCustomGui* sub = static_cast<IAovLayersCustomGui*>(userdata);
//	DeleteObj(sub);
//};
//
//const Char* SDKExampleCustomGUIString::GetResourceSym()
//{
//	// Returns the resource symbol. This symbol can be used in resource files in combination with "CUSTOMGUI".
//	return "CUSTOMGUISTRINGTEST";
//};
//
//CustomProperty* SDKExampleCustomGUIString::GetProperties()
//{
//	// This method can return a pointer to a data structure holding various additional properties. 
//	return nullptr;
//};
//
//Int32 SDKExampleCustomGUIString::GetResourceDataType(Int32*& table)
//{
//	// Returns the applicable datatypes defined in the stringtable array.
//	table = g_stringtable;
//	return sizeof(g_stringtable) / sizeof(Int32);
//};
//
//class AovCustomDataTypeClass : public CustomDataTypeClass
//{
//	INSTANCEOF(AovCustomDataTypeClass, CustomDataTypeClass)
//
//public:
//	virtual Int32 GetId()
//	{
//		return ID_SDK_EXAMPLE_CUSTOMDATATYPE_TEST;
//		
//	}
//
//	virtual CustomDataType* AllocData()
//	{
//		iCustomDataTypeAOV* data = NewObjClear(iCustomDataTypeAOV);
//		return data;
//	};
//
//	virtual void FreeData(CustomDataType* data)
//	{
//		iCustomDataTypeAOV* d = static_cast<iCustomDataTypeAOV*>(data);
//		DeleteObj(d);
//	}
//
//	virtual Bool CopyData(const CustomDataType* src, CustomDataType* dst, AliasTrans* aliastrans)
//	{
//		return true;
//	}
//
//	virtual Int32 Compare(const CustomDataType* d1, const CustomDataType* d2)
//	{
//		
//		return 0;
//	}
//
//	virtual Bool WriteData(const CustomDataType* t_d, HyperFile* hf)
//	{
//		
//		return true;
//	}
//
//	virtual Bool ReadData(CustomDataType* t_d, HyperFile* hf, Int32 level)
//	{
//		
//		return true;
//	}
//
//
//	virtual const Char* GetResourceSym()
//	{
//		// this symbol can be used in resource files
//		return "MYTEST";
//	}
//
//	virtual void GetDefaultProperties(BaseContainer &data)
//	{
//		// the default values of this datatype
//		// use the custom GUI as default
//		data.SetInt32(DESC_CUSTOMGUI, ID_SDK_EXAMPLE_CUSTOMGUI_STRING);
//		data.SetInt32(DESC_ANIMATE, DESC_ANIMATE_ON);
//	}
//};
//
//
//Bool RegisterCustomGUITest()
//{
//	if (!RegisterCustomDataTypePlugin(
//		GeLoadString(IDS_CUSTOMDATATYPE_DOTS),
//		CUSTOMDATATYPE_INFO_LOADSAVE |
//		CUSTOMDATATYPE_INFO_TOGGLEDISPLAY |
//		CUSTOMDATATYPE_INFO_HASSUBDESCRIPTION,
//		NewObjClear(AovCustomDataTypeClass),
//		1))
//		return false;
//
//
//	static BaseCustomGuiLib myStringGUIlib;
//
//	ClearMem(&myStringGUIlib, sizeof(myStringGUIlib));
//	FillBaseCustomGui(myStringGUIlib);
//
//	if (!InstallLibrary(ID_SDK_EXAMPLE_CUSTOMGUI_STRING, &myStringGUIlib, 1000, sizeof(myStringGUIlib)))
//		return false;
//
//	if (!RegisterCustomGuiPlugin(GeLoadString(IDS_CUSTOMGUISTRING), 0, NewObjClear(SDKExampleCustomGUIString)))
//		return false;
//
//	return true;
//}
//
//
