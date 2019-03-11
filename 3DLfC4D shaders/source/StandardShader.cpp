// example for a complex channel shader with custom areas
// and animated preview
#include "c4d.h"
#include "c4d_symbols.h"
#include "xstandardshader.h"
#include "IDs.h"


class StandardShader : public ShaderData
{
public:
	virtual Bool Init(GeListNode* node);
	virtual Vector Output(BaseShader* sh, ChannelData* sd);
	static NodeData* Alloc(void) { return NewObjClear(StandardShader); }
};

Bool StandardShader::Init(GeListNode* node)
{
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();

	//Coating default values
	data->SetBool(COATING_ON, false);
	data->SetVector(COATING_COLOR, Vector(1, 1, 1));
	data->SetVector(COATING_TRANSMITTANCE, Vector(1, 1, 1));
	data->SetFloat(COATING_THICKNESS, 0.01);
	data->SetFloat(COATING_ROUGHNESS, 0.05);
	data->SetFloat(COATING_FR, 0.05);

	//Diffuse default values
	data->SetFloat(DIFFUSE_WEIGHT, 0.7);
	data->SetVector(DIFFUSE_COLOR, Vector(1, 1, 1));
	data->SetFloat(DIFFUSE_ROUGHNESS, 0.0);

	//Specular default values
	data->SetFloat(SPECULAR_WEIGHT, 0.3);
	data->SetVector(SPECULAR_COLOR, Vector(1, 1, 1));
	data->SetFloat(SPECULAR_ROUGHNESS, 0.05);
	data->SetFloat(SPECULAR_FR, 0.05);
	data->SetVector(SPECULAR_ANISOTROPY_DIRECTION, Vector(1, 0, 0));


	//Emission default values
	data->SetFloat(EMISSION_WEIGHT, 1.0);
	data->SetVector(EMISSION_COLOR, Vector(1, 1, 1));

	//Refraction default values
	//data->SetBool(REFRACTION_ON, false);
	//data->SetFloat(REFRACTION_WEIGHT, 1.0);
	//data->SetVector(REFRACTION_COLOR, Vector(1, 1, 1));
	//data->SetFloat(REFRACTION_ROUGHNESS, 0.05);
	//data->SetFloat(REFRACTION_IOR, 1.5);

	return true;
}


Vector StandardShader::Output(BaseShader* sh, ChannelData* sd)
{
	return Vector(0, 0, 0);
}

Bool RegisterStandardShader(void)
{
	//GePrint("Register standard shader");
	// be sure to use a unique ID obtained from www.plugincafe.com
	return RegisterShaderPlugin(ID_STANDARDSHADER, "DL_StandardSurface"_s, 0, StandardShader::Alloc, "Xstandardshader"_s, 0);
}
