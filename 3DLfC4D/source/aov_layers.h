#ifndef _AOV_LAYERS_H_
#define _AOV_LAYERS_H_

#include "c4d.h"




class LayersCustomDataTypeClass; // forward declaration

/**
	In this class we have stored the information that we need for the aovlayers.
	We have inherited from iCustomDataType so we can override its funtcions
	to store this data and also write them on a hyperfile so we can access them 
	even after we have closed c4d.
*/
class iCustomDataTypeLayers : public iCustomDataType<iCustomDataTypeLayers>
{
	friend class LayersCustomDataTypeClass;

public:

	maxon::BaseArray<maxon::String> m_selected_layers;
	maxon::BaseArray<maxon::Int32> m_selected_id;
	maxon::BaseArray<maxon::Bool> m_output_to_framebuffer;
	maxon::BaseArray<maxon::Bool> m_output_to_file;
	maxon::BaseArray<maxon::Bool> m_output_to_jpeg;
	bool m_check = true;
	iCustomDataTypeLayers()
	{

	}
};

/**
	AOVDialog class represents the aov selector GUI after add button
	is clicked from the AOVLAyers GUI. It inherits from GeDialog as 
	it is used to display GUI dialogs.
*/
class AOVDialog : public GeDialog
{
private:
	SimpleListView				m_shading_listview;
	SimpleListView				m_auxiliary_listview;
	AutoAlloc<BaseSelect>	    m_selection;
	void m_update_buttons();

public:
	AOVDialog();
	virtual ~AOVDialog();
	maxon::BaseArray<String> m_aov_selector_selected;
	maxon::BaseArray<Int32>  m_aov_selector_selectedID;
	iCustomDataTypeLayers*   m_data;
	virtual Bool CreateLayout();
	virtual Bool InitValues();
	virtual Bool Command(Int32 i_id, const BaseContainer& i_msg);
	virtual Int32 Message(const BaseContainer& i_msg, BaseContainer& i_result);
};



/**
iCustomDataTypeAOV is the class used to buid the AOV Layers and
it inherits from iCustomGui because the ListView for the AOV Layers
can not be build directly on a descriprion UI so we use Custom Gui
DataType to build on a parameter description. Basically we are putting
a GeDialog in a parameter description UI using the iCustomGui class
*/


class iCustomDataTypeAOV : public iCustomGui

{
	INSTANCEOF(iCustomDataTypeAOV, iCustomGui)

private:

	iCustomDataTypeLayers		m_data;
	AOVDialog					m_dialog;
	SimpleListView				m_aov_listview;
	AutoAlloc<BaseSelect>		m_selection;
	void m_update_buttons();

public:
	iCustomDataTypeAOV(const BaseContainer &i_settings, CUSTOMGUIPLUGIN *i_plugin);
	virtual Bool CreateLayout();
	virtual Bool InitValues();
	virtual Bool Command(Int32 i_id, const BaseContainer &i_msg);
	virtual Bool SetData(const TriState<GeData> &i_tristate);
	virtual TriState<GeData> GetData();
};

#endif
