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

PolygonObject* GetMeshFromNode(GeListNode* C4DNode){
	BaseObject* obj=(BaseObject*)C4DNode;

	//By default, we assume that the node is a polygon object and perform a direct cast
	PolygonObject* mesh = (PolygonObject*)C4DNode; 
	
	String type = (String::IntToString(obj->GetType()));
	ApplicationOutput(type);
	//If the object is an alembic generator, we get the polygon mesh from the cache instead
	if (obj->GetType() == Oalembicgenerator){ 
		BaseObject* cache = obj->GetCache();
		if (cache == NULL){ return NULL;}
		if (cache->GetType() != Opolygon){ return NULL; }
		mesh = (PolygonObject*)cache;
	}

	if (obj->GetType() != Opolygon && obj->GetType() != Oalembicgenerator) {
		ModelingCommandData mcd;
		mcd.op = obj;
		mcd.doc = obj->GetDocument();
		if (!SendModelingCommand(MCOMMAND_CURRENTSTATETOOBJECT, mcd)) return nullptr;
		ModelingCommandData mcd2;
		mcd2.op = static_cast<BaseObject*>(mcd.result->GetIndex(0));
		mesh = static_cast<PolygonObject*>(mcd2.op);
	}
	return mesh;
}

void PolygonObjectTranslator::CreateNSINodes(const char* ParentTransformHandle, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	NSI::Context ctx(parser->GetContext());

	skip = false;
	
	
	BaseObject* baseobject = (BaseObject*)C4DNode;
	PolygonObject* object = GetMeshFromNode(C4DNode);

	if (!object){
		skip = true;
		return;
	}

	//PolygonObject* object = (PolygonObject*)C4DNode;

	//if (baseobject->GetType() == Oalembicgenerator){ //If the object is an alembic generator, we get the polygon mesh from the cache
	//	BaseObject* cache = baseobject->GetCache();
	//	if (cache != NULL ){
	//		if (cache->GetType() == Opolygon){
	//			object = (PolygonObject*)cache;
	//		}
	//		else{
	//			skip = true;
	//			return;
	//		}
	//	}
	//	else{
	//		skip = true; 
	//		return;
	//	}
	//}

	polycount = object->GetPolygonCount();
	pointcount = object->GetPointCount();
	const CPolygon* polys = object->GetPolygonR();

	//Create a mesh and connect it to the parent transform
	handle = string(parser->GetUniqueName("mesh"));
	transform_handle = string(ParentTransformHandle);

	ctx.Create(handle, "mesh");
	ctx.Connect(handle, "", transform_handle, "objects");

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

	ctx.SetAttribute(handle, (
		arg_nvertices,
		arg_indices,
		arg_subdivision
		));

	//Export first UV set as "st"
	UVWTag* UVtag = (UVWTag*)object->GetTag(Tuvw);

	if (UVtag != NULL){
		vector<int> st_indices;
		st_indices.reserve(n_facevertices);

		map<int, vector<int> >  UV_connectivity_map;
		//For every mesh vertex index (first int), build a vector of indices (second int) into the UV coordinate vector

		vector<float> st;
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
			arg_st,
			arg_st_indices
			));

	}

	if (has_phong && !is_subd){
		NSI::Argument arg_N_indices("N.indices");
		arg_N_indices.SetType(NSITypeInteger);
		arg_N_indices.SetCount(n_facevertices);
		arg_N_indices.SetValuePointer((void*)&facevarying_indices[0]);
		ctx.SetAttribute(handle, (arg_N_indices));
	}

}

void PolygonObjectTranslator::SampleMotion(double t, long i, GeListNode* C4DNode, BaseDocument* doc, DL_SceneParser* parser){
	if (skip){ return; }

	NSI::Context ctx(parser->GetContext());

	PolygonObject* mesh = GetMeshFromNode(C4DNode);
	if (!mesh){
		skip = true;
		return;
	}

	//PolygonObject* mesh = (PolygonObject*)C4DNode;

	//BaseObject* obj = (BaseObject*)C4DNode;
	//if (obj->GetType() == Oalembicgenerator){ //If the object is an alembic generator, we get the polygon mesh from the cache
	//	BaseObject* cache = obj->GetCache();
	//	if (cache != NULL){
	//		if (cache->GetType() == Opolygon){
	//			mesh = (PolygonObject*)cache;
	//		}
	//		else{
	//			skip = true;
	//			return;
	//		}
	//	}
	//	else{
	//		skip = true;
	//		return;
	//	}
	//}


	PolygonObject* deformed = (PolygonObject*)(mesh->GetDeformCache());
	if (deformed == NULL){
		deformed = mesh;
	}

	//Can we skip motion blur sampling if deformed==NULL?
	//No, the mesh may still have PLA. 

	//Don't sample if the polygon or vertex count changed since the NSI mesh node was created
	if (!(deformed->GetPointCount() == pointcount &&  deformed->GetPolygonCount() == polycount)){
		skip = true;
		return;
	}
	
	const Vector* points = deformed->GetPointR();
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


