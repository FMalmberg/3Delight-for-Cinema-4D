#include "PolygonObjectTranslator.h"
#include "nsi.hpp"
#include <map>
#include <vector>
using namespace std;

int GetUVIndex(int poly_index, float u, float v, map<int, vector<int> > & conmap, vector<float>& st, int& maxindex){
	vector<int>& con = conmap[poly_index];
	bool found = false;

	int result = 0;
	int index;

	for (int i = 0; i < con.size() &&!found; i++ ){
		index = con[i];
		if (!(abs(st[2 * index] - u)>0)  && !(abs(st[2 * index + 1] - v)>0)){
			result = index;
			
			found = true;
		}
	}
	
	if (!found){
		result = maxindex;
		con.push_back(result);
		maxindex++;
		st.push_back(u);
		st.push_back(v);
	}

	return result;
}

void PolygonObjectTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	//Create a mesh and connect it to the parent transform
	handle = string(parser->GetUniqueName("mesh"));
	transform_handle = string(ParentTransformHandle);

	ctx.Create(handle, "mesh");
	ctx.Connect(handle, "", transform_handle, "objects");

	PolygonObject* object = (PolygonObject*)C4DNode;
	int polycount = object->GetPolygonCount();
	int pointcount = object->GetPointCount();
	const CPolygon* polys = object->GetPolygonR();

	//---------------------------//
	//Check for SDS
	is_subd = false;
	string subdivision_scheme = "";
	long UV_subdivision_mode = SDSOBJECT_SUBDIVIDE_UV_STANDARD;

	BaseObject* tmp = (BaseObject*)C4DNode;
	BaseObject* tmp2 = tmp;
	tmp = tmp->GetUp();

	while (tmp != NULL && !(tmp->GetType() == Osds && tmp->GetDeformMode())){
		tmp2 = tmp;
		tmp = tmp->GetUp();
	}

	if (tmp != NULL && tmp->GetType() == Osds){
		if (tmp2 == tmp->GetDown()){
			is_subd = true;
			subdivision_scheme = "catmull-clark";
			BaseContainer* sds_settings = tmp->GetDataInstance();
			UV_subdivision_mode = sds_settings->GetInt32(SDSOBJECT_SUBDIVIDE_UV, SDSOBJECT_SUBDIVIDE_UV_STANDARD);
		}
	}
	//---------------------------//


	//Check for Phong tag
	BaseTag* phongtag = object->GetTag(Tphong);
	has_phong = false;
	if (phongtag != NULL){
		has_phong = true;
	}

	vector<int> nvertices(polycount);
	vector<int> indices;
	indices.reserve(polycount * 4);

	for (int j = 0; j < polycount; j++){
		indices.push_back(polys[j].a);
		indices.push_back(polys[j].b);
		indices.push_back(polys[j].c);
		nvertices[j] = 3;
		if (polys[j].c != polys[j].d){
			nvertices[j] = 4;
			indices.push_back(polys[j].d);
		}
	}

	NSI::Argument arg_nvertices("nvertices");
	arg_nvertices.SetType(NSITypeInteger);
	arg_nvertices.SetCount(polycount);
	arg_nvertices.SetValuePointer((void*)&nvertices[0]);

	NSI::Argument arg_indices("P.indices");
	arg_indices.SetType(NSITypeInteger);
	arg_indices.SetCount(indices.size());
	arg_indices.SetValuePointer((void*)&indices[0]);

	NSI::StringArg arg_subdivision("subdivision.scheme", subdivision_scheme.c_str());

	vector<int> facevarying_indices;
	facevarying_indices.reserve(polycount * 4);
	int nverts;
	int index = 0;
	for (int j = 0; j < polycount; j++){
		nverts = 3;
		if (polys[j].c != polys[j].d){
			nverts = 4;
		}

		for (int i = 0; i < nverts; i++){
			facevarying_indices.push_back(index); index++;
		}
	}

	n_facevertices = facevarying_indices.size();

	//Export first UV set as "st"

	UVWTag* UVtag = (UVWTag*)object->GetTag(Tuvw);

	if (UVtag != NULL){
		vector<int> st_indices;
		st_indices.reserve(n_facevertices);

		map<int, vector<int> >  UV_connectivity_map;
		//For every mesh vertex index (first int), build a vector of indices (second int) into the UV coordinate vector

		vector<float> st;
		//vector<int> polygon_vertices;
		st.reserve(n_facevertices * 2);
		int maxindex = 0;
		int index = 0;

		for (int j = 0; j < polycount; j++){
			UVWStruct uv = UVtag->GetSlow(j);

			index = GetUVIndex(polys[j].a, uv.a.x, uv.a.y, UV_connectivity_map, st, maxindex);
			st_indices.push_back(index);

			index = GetUVIndex(polys[j].b, uv.b.x, uv.b.y, UV_connectivity_map, st, maxindex);
			st_indices.push_back(index);

			index = GetUVIndex(polys[j].c, uv.c.x, uv.c.y, UV_connectivity_map, st, maxindex);
			st_indices.push_back(index);

			if (polys[j].c != polys[j].d){
				index = GetUVIndex(polys[j].d, uv.d.x, uv.d.y, UV_connectivity_map, st, maxindex);
				st_indices.push_back(index);
			}
		}


		int st_flags = (NSIParamPerVertex);
		if (UV_subdivision_mode == SDSOBJECT_SUBDIVIDE_UV_STANDARD){
			st_flags = st_flags | NSIParamInterpolateLinear;
		}
		NSI::Argument arg_st("st");
		arg_st.SetCount(st.size()/2);
		arg_st.SetArrayType(NSITypeFloat, 2);
		arg_st.SetValuePointer((void*)&st[0]);
		arg_st.SetFlags(st_flags);

		NSI::Argument arg_st_indices("st.indices");
		arg_st_indices.SetType(NSITypeInteger);
		arg_st_indices.SetCount(st_indices.size());
		arg_st_indices.SetValuePointer((void*)&st_indices[0]);

		ctx.SetAttribute(handle, (
			arg_nvertices,
			arg_indices,
			arg_subdivision,
			arg_st,
			arg_st_indices
			));
		/*polygon_vertices.clear();
		polygon_vertices.push_back(polys[j].a);
		polygon_vertices.push_back(polys[j].b);
		polygon_vertices.push_back(polys[j].c);
		if (polys[j].c != polys[j].d){
		polygon_vertices.push_back(polys[j].d);
		}

		for (int i = 0; i < polygon_vertices.size(); i++){

		}*/



		/*int index=0;
		for(int j=0; j<polycount; j++){
		UVWStruct uv=UVtag->GetSlow(j);
		st.push_back(uv.a.x); st.push_back(1-uv.a.y); //a
		st.push_back(uv.b.x); st.push_back(1-uv.b.y); //b
		st.push_back(uv.c.x); st.push_back(1-uv.c.y); //c
		if(polys[j].c !=polys[j].d){
		st.push_back(uv.d.x); st.push_back(1-uv.d.y); //d
		}
		}*/

	}
	/*else { //Default st
		int index=0;
		for(int j=0; j<polycount; j++){

		st.push_back(0); st.push_back(1); //a
		st.push_back(0); st.push_back(0); //b
		st.push_back(1); st.push_back(0); //c
		if(polys[j].c !=polys[j].d){
		st.push_back(1); st.push_back(1); //d
		}
		}

		}*/

	//NSI API for defining indirect parameters updated.
	//Old version:
	//int st_flags=(NSIParamIndirect|NSIParamPerVertex); 

	//New version:


	if (has_phong && !is_subd){
		NSI::Argument arg_N_indices("N.indices");
		arg_N_indices.SetType(NSITypeInteger);
		arg_N_indices.SetCount(n_facevertices);
		arg_N_indices.SetValuePointer((void*)&facevarying_indices[0]);
		ctx.SetAttribute(handle, (arg_N_indices));
	}

}

void PolygonObjectTranslator::SampleMotion(double t, long i, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	PolygonObject* mesh = (PolygonObject*)C4DNode;
	PolygonObject* deformed = (PolygonObject*)(mesh->GetDeformCache());

	//Can we skip motion blur sampling if deformed==NULL?
	//No, the mesh may still have PLA. 

	if (deformed == NULL){
		deformed = mesh;
	}
	const Vector* points = deformed->GetPointR();

	int pointcount = deformed->GetPointCount();
	int polycount = deformed->GetPolygonCount();
	const CPolygon* polys = deformed->GetPolygonR();

	//Vertex positions
	vector<float> P(pointcount * 3);

	for (int i = 0; i < pointcount; i++){
		P[i * 3] = points[i].x;
		P[i * 3 + 1] = points[i].y;
		P[i * 3 + 2] = points[i].z;
	}

	NSI::Argument arg_P("P");
	arg_P.SetType(NSITypePoint);
	arg_P.SetCount(pointcount);
	arg_P.SetValuePointer((void*)&P[0]);

	ctx.SetAttributeAtTime(handle, t, (
		arg_P
		));

	//Phong normals
	if (has_phong && !is_subd){
		vector<float> N;
		N.reserve(n_facevertices * 3);
		Vector32* normals = deformed->CreatePhongNormals();
		Vector32 tmp;
		long nverts;
		for (int j = 0; j < polycount; j++){
			nverts = 3;
			if (polys[j].c != polys[j].d){ nverts = 4; }

			for (int n = 0; n < nverts; n++){
				tmp = normals[4 * j + n];
				N.push_back(tmp.x);
				N.push_back(tmp.y);
				N.push_back(tmp.z);
			}
		}
		DeleteMem(normals);

		NSI::Argument arg_N("N");
		arg_N.SetType(NSITypeNormal);
		arg_N.SetCount(n_facevertices);
		arg_N.SetValuePointer((void*)&N[0]);
		arg_N.SetFlags(NSIParamPerVertex);


		ctx.SetAttributeAtTime(handle, t, (
			arg_N
			));
	}

}


