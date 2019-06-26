#ifndef POLYGON_OBJECT_TRANSLATOR_H
#define POLYGON_OBJECT_TRANSLATOR_H

#include "c4d.h"
#include "DL_API.h"
#include <vector>
//#include "Edge.h"
#include <string>
#include <map>

class PolygonObjectTranslator : public DL_TranslatorPlugin{

public:
	virtual void Init(BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void CreateNSINodes(const char* Handle, const char* ParentTransformHandle, BaseList2D* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void SampleAttributes(DL_SampleInfo* info, const char* Handle, BaseList2D* C4DNode, BaseDocument* doc,DL_SceneParser* parser);

private:
	bool is_subd;
	long UV_subdivision_mode;
	bool has_phong;
	/*std::string handle;
	std::string transform_handle;
	
	
	int pointcount;
	int polycount;
	int n_facevertices;*/


/*	std::vector< std::vector<float> > P_samples;
	std::vector< std::vector<float> > N_samples;
	bool has_uv;
	bool has_phong;
	std::vector<float> st; 

	edgemap edge_creases;*/
};


#endif