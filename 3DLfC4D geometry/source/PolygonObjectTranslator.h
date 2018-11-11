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
	virtual void CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser);
	virtual void SampleMotion(double t, long i, GeListNode* C4DNode, BaseDocument* doc,DL_SceneParser* parser);

private:
	bool skip;
	std::string handle;
	std::string transform_handle;
	bool has_phong;
	bool is_subd;
	int pointcount;
	int polycount;
	int n_facevertices;
/*	std::vector< std::vector<float> > P_samples;
	std::vector< std::vector<float> > N_samples;
	bool has_uv;
	bool has_phong;
	std::vector<float> st; 

	edgemap edge_creases;*/
};


#endif