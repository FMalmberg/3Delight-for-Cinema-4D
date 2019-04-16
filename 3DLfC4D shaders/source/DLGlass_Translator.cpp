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
	m_ids_to_names[Shader_Path] = pair("", &c_shaderpath[0]);
	m_ids_to_names[REFLECTION_COLOR] = pair("Cout", "reflect_color");
	m_ids_to_names[REFLECTION_COLOR_SHADER] = pair("Cout", "reflect_color");
	m_ids_to_names[REFLECTION_ROUGHNESS] = pair("Cout[0]", "reflect_roughness");
	m_ids_to_names[REFLECTION_ROUGHNESS_SHADER] = pair("Cout[0]", "reflect_roughness");
	m_ids_to_names[REFLECTION_IOR] = pair("Cout[0]", "reflect_ior");
	m_ids_to_names[REFLECTION_IOR_SHADER] = pair("Cout[0]", "reflect_ior");
	m_ids_to_names[REFLECTION_THIN_FILM] = pair("", "thin_film");
	m_ids_to_names[REFLECTION_FILM_THICKNESS] = pair("Cout[0]", "film_thickness");
	m_ids_to_names[REFLECTION_FILM_THICKNESS_SHADER] = pair("Cout[0]", "film_thickness");
	m_ids_to_names[REFLECTION_FILM_IOR] = pair("Cout[0]", "film_ior");
	m_ids_to_names[REFLECTION_FILM_IOR_SHADER] = pair("Cout[0]", "film_ior");

	m_ids_to_names[REFRACTION_COLOR] = pair("Cout", "i_color");
	m_ids_to_names[REFRACTION_COLOR_SHADER] = pair("Cout", "i_color");
	m_ids_to_names[REFRACTION_ROUGHNESS] = pair("Cout[0]", "refract_roughness");
	m_ids_to_names[REFRACTION_ROUGHNESS_SHADER] = pair("Cout", "refract_roughness");
	m_ids_to_names[REFRACTION_IOR] = pair("Cout[0]", "refract_ior");
	m_ids_to_names[REFRACTION_IOR_SHADER] = pair("Cout[0]", "refract_ior");

	m_ids_to_names[VOLUMETRIC_ENABLE] = pair("", "volumetric_enable");
	m_ids_to_names[VOLUMETRIC_DENSITY] = pair("", "volumetric_density");
	m_ids_to_names[VOLUMETRIC_SCATTERING] = pair("", "volumetric_scattering_color");
	m_ids_to_names[VOLUMETRIC_TRANSPARENCY] = pair("", "volumetric_transparency_color");
	
	m_ids_to_names[INCADESCENCE_COLOR] = pair("Cout", "incandescence");
	m_ids_to_names[INCADESCENCE_COLOR_SHADER] = pair("Cout", "incandescence");
	m_ids_to_names[INCADESCENCE_INTENSITY] = pair("", "incandescence_intensity");
	m_ids_to_names[BUMP_TYPE] = pair("", "disp_normal_bump_type");
	m_ids_to_names[BUMP_INTENSITY] = pair("", "disp_normal_bump_intensity");

}
