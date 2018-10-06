/////////////////////////////////////////////////////////////
// CINEMA 4D SDK                                           //
/////////////////////////////////////////////////////////////
// (c) 1989-2004 MAXON Computer GmbH, all rights reserved  //
/////////////////////////////////////////////////////////////

// "look at editor camera" expression example

#include "c4d.h"
#include "c4d_symbols.h"
#include "tshader.h"
#include "IDs.h"


class DL_ShaderTag: public TagData
{
	public:
		virtual Bool Init(GeListNode *node);

		static NodeData *Alloc(void) { return NewObjClear(DL_ShaderTag); }
};

Bool DL_ShaderTag::Init(GeListNode *node)
{
	BaseTag				*tag  = (BaseTag*)node;
	BaseContainer *data = tag->GetDataInstance();
	
	data->SetFloat(DISPLACEMENT_BOUND,0);

	return TRUE;
}






Bool RegisterDL_ShaderTag(void)
{
	return RegisterTagPlugin(ID_DL_SHADERTAG,"DL_ShaderNetwork",TAG_VISIBLE,DL_ShaderTag::Alloc,"tshader",AutoBitmap("Materialtag.tif"),0);
}
