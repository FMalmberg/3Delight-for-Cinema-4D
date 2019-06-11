#ifndef _MULTI_LIGHT_H_
#define _MULTI_LIGHT_H_

#include "c4d.h" 
class MultiLightCustomDataTypeClass; // forward declaration

/**
	In this class we have stored the information that we need for the aovlayers.
	We have inherited from iCustomDataType so we can override its funtcions
	to store this data and also write them on a hyperfile so we can access them
	even after we have closed c4d.
*/
class iCustomDataTypeLights : public iCustomDataType<iCustomDataTypeLights>
{
	friend class MultiLightCustomDataTypeClass;

public:
	maxon::BaseArray<maxon::String> m_multi_light_selected_layers;
	maxon::BaseArray<Int32> m_selected_lights_itemID;
	maxon::BaseArray<UInt64> m_selected_lights_GUID;
	maxon::BaseArray<maxon::String> m_all_multi_lights;
	bool m_check = true;
	iCustomDataTypeLights()
	{

	}
};

/**
iCustomDataTypeAOV is the class used to buid the AOV Layers and
it inherits from iCustomGui because the ListView for the AOV Layers
can not be build directly on a descriprion UI so we use Custom Gui
DataType to build on a parameter description. Basically we are putting
a GeDialog in a parameter description UI using the iCustomGui class
*/


class iCustomDataTypeMultiLights : public iCustomGui

{
	INSTANCEOF(iCustomDataTypeAOV, iCustomGui)

private:

	iCustomDataTypeLights		m_data;
	SimpleListView				m_multi_light_listview;
	AutoAlloc<BaseSelect>		m_multi_light_selection;
	TreeViewCustomGui*			m_light_view;

public:
	iCustomDataTypeMultiLights(const BaseContainer &i_settings, CUSTOMGUIPLUGIN *i_plugin);
	virtual Bool CreateLayout();
	virtual Bool InitValues();
	virtual Bool Command(Int32 i_id, const BaseContainer &i_msg);
	virtual Int32 Message(const BaseContainer& msg, BaseContainer& result);
	virtual Bool CoreMessage(Int32 id, const BaseContainer& msg);
	virtual Bool SetData(const TriState<GeData> &i_tristate);
	virtual TriState<GeData> GetData();
};

#endif
