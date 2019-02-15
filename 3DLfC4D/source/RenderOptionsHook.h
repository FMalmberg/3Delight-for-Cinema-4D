#ifndef RENDER_OPTIODL_HOOK_H
#define RENDER_OPTIODL_HOOK_H

#include "DL_HookPlugin.h"
#include "c4d.h"
#include <vector>
#include "aov_layers.h"



class RenderOptionsHook:public DL_HookPlugin{
public:
	virtual void CreateNSINodes(BaseDocument* i_doc, DL_SceneParser* i_parser);
	virtual void ConnectNSINodes(BaseDocument* i_doc, DL_SceneParser* i_parser);

private:
	std::string m_layer_handle;
	std::string m_layer_jpg;
	std::string m_layer_file;
	std::string m_display_driver_handle;
	std::string m_file_driver_handle;
	std::string m_bitmapdriver_handle;
	std::string m_aov;
	std::string m_layertype;
	std::string m_format;
	std::string m_variable_source;
	std::string m_output_driver_handle;
	std::string m_scalar_format;
	std::string m_extension;
	std::string m_output_format;
};

#endif