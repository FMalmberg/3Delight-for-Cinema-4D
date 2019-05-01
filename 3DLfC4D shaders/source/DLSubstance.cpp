#include "c4d.h"
#include "c4d_symbols.h"
#include "IDs.h"
#include "dl_substance.h"
#include <algorithm>
#include "DL_Utilities.h"
#include "DL_TypeConversions.h"

void eraseSubStr(std::string & mainStr, const std::string & toErase)
{
	// Search for the substring in string
	size_t pos = mainStr.find(toErase);

	if (pos != std::string::npos)
	{
		// If found then erase it from string
		mainStr.erase(pos, toErase.length());
	}
	
}

void AssignTexture(BaseMaterial* material, BaseContainer* bc, std::string texture_path, std::string texture)
{
	Filename file = Filename(texture_path.c_str());
	BaseShader* bitmapShader = BaseShader::Alloc(Xbitmap);
	bitmapShader->SetParameter(DescID(BITMAPSHADER_FILENAME), file, DESCFLAGS_SET::NONE);
	material->InsertShader(bitmapShader);
	material->Message(MSG_UPDATE);
	material->Update(true, true);
	
	ApplicationOutput(texture_path.c_str());
	if (texture == "base_color")
	{
		bc->SetLink(SUBSTANCE_COLOR_SHADER, bitmapShader);
	}

	else if (texture == "emissive")
	{
		bc->SetLink(EMISSIVE_COLOR_SHADER, bitmapShader);
	}	

	else if (texture == "metallic")
	{
		bc->SetLink(SUBSTANCE_METALLIC_SHADER, bitmapShader);
	}	
	
	else if (texture == "opacity")
	{
		bc->SetLink(SUBSTANCE_OPACITY_SHADER, bitmapShader);
	}	
	
	else if (texture == "roughness")
	{
		bc->SetLink(SUBSTANCE_ROUGHNESS_SHADER, bitmapShader);
	}	
	
	else if (texture == "specular_level")
	{
		bc->SetLink(SUBSTANCE_SPECULAR_LEVEL_SHADER, bitmapShader);
	}	

	else if (texture == "height")
	{
		bc->SetLink(BUMP_VALUE, bitmapShader);
	}	

}

/*
	Function to check if the name in any of the suffixes
	is substring of the given name.
*/
bool exists(vector<string> suffixes, std::string &o_name)
{
	for (int i = 0; i < suffixes.size(); i++)
	{
		if (o_name.find(suffixes[i]) != std::string::npos)
		{
			eraseSubStr(o_name, suffixes[i]);
			return true;
		}
	}
	return false;
}

class DL_Substance : public MaterialData
{
	INSTANCEOF(DL_Substance, MaterialData)

private:
	Vector color;

public:
	virtual Bool Init(GeListNode* node);
	virtual	INITRENDERRESULT InitRender(BaseMaterial* mat, const InitRenderStruct& irs);
	virtual Bool GetDEnabling(GeListNode *node, const DescID &id, const GeData &t_data,
		DESCFLAGS_ENABLE flags, const BaseContainer *itemdesc);
	virtual Bool Message(GeListNode *node, Int32 type, void *data);
	virtual	void CalcSurface(BaseMaterial* mat, VolumeData* vd);
	static NodeData* Alloc(void) { return NewObjClear(DL_Substance); }

};

Bool DL_Substance::Init(GeListNode* node)
{
	BaseShader* const shader = BaseShader::Alloc(Xbrick);

	const Float step = 360;
	BaseContainer* data = ((BaseShader*)node)->GetDataInstance();

	Vector substance_color = HSVToRGB(Vector(38.182 / step, 0, 0.5));
	data->SetVector(SUBSTANCE_COLOR, substance_color);
	data->SetFloat(SUBSTANCE_ROUGHNESS, 0.3);
	data->SetFloat(SUBSTANCE_SPECULAR_LEVEL, 0.5);
	data->SetFloat(SUBSTANCE_METALLIC, 0);
	data->SetFloat(SUBSTANCE_OPACITY, 1);
	data->SetLink(SUBSTANCE_COLOR_SHADER, (C4DAtomGoal*)shader);
	
	Vector emissive_color = HSVToRGB(Vector(39.231 / step, 0, 0));
	data->SetVector(EMISSIVE_COLOR, emissive_color);
	data->SetFloat(EMISSIVE_INTENSITY, 1.0);

	data->SetInt32(BUMP_TYPE, BUMP_MAP);
	data->SetLink(BUMP_VALUE, nullptr);
	data->SetFloat(BUMP_INTENSITY, 1);

	return true;
}

Bool DL_Substance::GetDEnabling(
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
	case SUBSTANCE_COLOR:
		return dldata->GetLink(SUBSTANCE_COLOR_SHADER, doc) == false;
		break;

	case SUBSTANCE_METALLIC:
		return dldata->GetLink(SUBSTANCE_METALLIC_SHADER, GetActiveDocument()) == false;
		break;

	case SUBSTANCE_OPACITY:
		return dldata->GetLink(SUBSTANCE_OPACITY_SHADER, GetActiveDocument()) == false;
		break;

	case SUBSTANCE_ROUGHNESS:
		return dldata->GetLink(SUBSTANCE_ROUGHNESS_SHADER, GetActiveDocument()) == false;
		break;

	case SUBSTANCE_SPECULAR_LEVEL:
		return dldata->GetLink(SUBSTANCE_SPECULAR_LEVEL_SHADER, GetActiveDocument()) == false;
		break;

	case EMISSIVE_COLOR:
		return dldata->GetLink(EMISSIVE_COLOR_SHADER, GetActiveDocument()) == false;
		break;

	default:
		break;

	}

	return true;
}

Bool DL_Substance::Message(GeListNode *node, Int32 type, void *data)
{
	BaseContainer* bc = ((BaseMaterial*)node)->GetDataInstance();
	BaseMaterial* material = (BaseMaterial*)node;

	vector<string> suffixes
	{
	  "base_color",
	  "emissive",
	  "metallic",
	  "normal",
	  "opacity",
	  "height",
	  "roughness",
	  "specular_level"
	};

	switch (type)
	{
		case MSG_DESCRIPTION_COMMAND:
		{
			DescriptionCommand* dc = (DescriptionCommand*)data;
			if (dc->_descId[0].id == ASSIGN_TEXTURES)
			{
				Filename loadFile;
				// open a file selector dialog to open a file
				if (loadFile.FileSelect(FILESELECTTYPE::IMAGES, FILESELECT::LOAD, "Open"_s))
				{
					//Get only the suffix of the file (png,jpg etc)
					std::string suffix = loadFile.GetSuffix().GetCStringCopy();
					suffix = "." + suffix;

					//Gets the name of the file without suffix 
					loadFile.ClearSuffix();
					std::string file_name = loadFile.GetFileString().GetCStringCopy();

					//Gets the directory of the file including the file in it.
					std::string texturepath = loadFile.GetString().GetCStringCopy();

					//converts the name of the file to lower characters. Not case sensitive
					std::transform(file_name.begin(), file_name.end(), file_name.begin(), ::tolower);

					if (exists(suffixes, file_name))
					{
						bc->SetLink(SUBSTANCE_COLOR_SHADER, nullptr);
						bc->SetLink(SUBSTANCE_METALLIC_SHADER, nullptr);
						bc->SetLink(SUBSTANCE_OPACITY_SHADER, nullptr);
						bc->SetLink(SUBSTANCE_ROUGHNESS_SHADER, nullptr);
						bc->SetLink(SUBSTANCE_SPECULAR_LEVEL, nullptr);
						bc->SetLink(EMISSIVE_COLOR_SHADER, nullptr);
						bc->SetLink(BUMP_VALUE, nullptr);
						//Get Only the name without the substance type(color,metal). Already done in the exists function. 
						//file_name was passed by reference. if it was brick_lumpy_Base_Color now it will be brick_lumpy_.
						eraseSubStr(texturepath, loadFile.GetFileString().GetCStringCopy());
						for (int i = 0; i < suffixes.size(); i++)
						{
							//This will have the filenames of all possible subbstance that can be found on the current directory
							//Ex. for brick_lumpy_ it will check  brick_lumpy_metallic, brick_lumpy_normal and all the others.
							std::string temp_file = texturepath + file_name + suffixes[i] + suffix;
							Filename file = Filename(temp_file.c_str());
							if (GeFExist(file))
								AssignTexture(material, bc, temp_file, suffixes[i]);

						}

					}
				}
			}
			break;
		}
	
	}
	return true;
}

//Initializes resources for rendering.
INITRENDERRESULT DL_Substance::InitRender(BaseMaterial* mat, const InitRenderStruct& irs)
{
	BaseContainer* data = mat->GetDataInstance();
	return INITRENDERRESULT::OK;
}

void DL_Substance::CalcSurface(BaseMaterial* mat, VolumeData* vd)
{

	Vector diff, spec;
	vd->IlluminanceSimple(&diff, &spec, 0, 0, 0);
	vd->col = 0.8*diff;
}

Bool RegisterDLSubstance(void)
{
	return RegisterMaterialPlugin(DL_SUBSTANCE, "Substance Material"_s, PLUGINFLAG_HIDE, DL_Substance::Alloc, "DL_Substance"_s, 0);
}
