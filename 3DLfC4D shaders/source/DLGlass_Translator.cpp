#include "DL_Utilities.h"
#include "DL_TypeConversions.h"
#include "c4d.h"
#include "nsi.hpp"
#include "c4d_symbols.h"
#include "DLGlass_Translator.h"
#include "dl_glass.h"
#include <assert.h>
#include <utility>

Delight_Glass::Delight_Glass()
{
	Filename shaderpath = Filename(GeGetPluginPath() + Filename("OSL") + Filename("dlGlass.oso"));
	vector<char> c_shaderpath = StringToChars(shaderpath.GetString());
	m_ids_to_names[Shader_Path] = make_pair("", &c_shaderpath[0]);
	m_ids_to_names[REFLECTION_COLOR] = make_pair("", "reflect_color");
	m_ids_to_names[REFLECTION_COLOR_SHADER] = make_pair("outColor", "reflect_color");
	m_ids_to_names[REFLECTION_ROUGHNESS] = make_pair("", "reflect_roughness");
	m_ids_to_names[REFLECTION_ROUGHNESS_SHADER] = make_pair("outColor[0]", "reflect_roughness");
	m_ids_to_names[REFLECTION_IOR] = make_pair("", "reflect_ior");
	m_ids_to_names[REFLECTION_IOR_SHADER] = make_pair("outColor[0]", "reflect_ior");
	m_ids_to_names[REFLECTION_THIN_FILM] = make_pair("", "thin_film");
	m_ids_to_names[REFLECTION_FILM_THICKNESS] = make_pair("", "film_thickness");
	m_ids_to_names[REFLECTION_FILM_THICKNESS_SHADER] = make_pair("outColor[0]", "film_thickness");
	m_ids_to_names[REFLECTION_FILM_IOR] = make_pair("", "film_ior");
	m_ids_to_names[REFLECTION_FILM_IOR_SHADER] = make_pair("outColor[0]", "film_ior");

	m_ids_to_names[REFRACTION_COLOR] = make_pair("", "i_color");
	m_ids_to_names[REFRACTION_COLOR_SHADER] = make_pair("outColor", "i_color");
	m_ids_to_names[REFRACTION_ROUGHNESS] = make_pair("", "refract_roughness");
	m_ids_to_names[REFRACTION_ROUGHNESS_SHADER] = make_pair("outColor", "refract_roughness");
	m_ids_to_names[REFRACTION_IOR] = make_pair("", "refract_ior");
	m_ids_to_names[REFRACTION_IOR_SHADER] = make_pair("outColor[0]", "refract_ior");

	m_ids_to_names[VOLUMETRIC_ENABLE] = make_pair("", "volumetric_enable");
	m_ids_to_names[VOLUMETRIC_DENSITY] = make_pair("", "volumetric_density");
	m_ids_to_names[VOLUMETRIC_SCATTERING] = make_pair("", "volumetric_scattering_color");
	m_ids_to_names[VOLUMETRIC_TRANSPARENCY] = make_pair("", "volumetric_transparency_color");
	
	m_ids_to_names[INCADESCENCE_COLOR] = make_pair("", "incandescence");
	m_ids_to_names[INCADESCENCE_COLOR_SHADER] = make_pair("outColor", "incandescence");
	m_ids_to_names[INCADESCENCE_INTENSITY] = make_pair("", "incandescence_intensity");
	m_ids_to_names[BUMP_TYPE] = make_pair("", "disp_normal_bump_type");
	m_ids_to_names[BUMP_VALUE] = make_pair("outColor", "disp_normal_bump_value");
	m_ids_to_names[BUMP_INTENSITY] = make_pair("", "disp_normal_bump_intensity");

}
