#pragma once
#include "Model.h"
#include <cstdio>
#include <vector>

using namespace std;

struct PointStr
{
	struct  
	{
		int _pos;
		int _uv;
		int _normal;
	} pt[3];
};

//Read the obj file and fill the model struct
class ObjReader
{
	enum CommandType
	{
		eVertex,
		eUV,
		eNormal,
		eFacet,
		eOthers,
	};
	enum FacetType
	{
		eOnlyPos,
		ePosUV,
		ePosNormal,
		eAll,
		eNonFacet,
	};
	
public:
	ObjReader(void);
	~ObjReader(void);
	bool operator()( CFile& file, Model& mdl );

protected:
	bool readLine( CFile& file, char* buff );
	CommandType judgeLine( char* buff );
	FacetType judgeFacet( char* buff );
	bool analysisData( CFile& file );
	void GetPosition( char* line, vtx3& pos );
	void GetUV( char* line, vtx2& uv );
	void GetNormal( char* line, vtx3& nromal );
	void GetVertex( char* line, PointStr& vtx );
	void FillModel( Model& mdl );

protected:
	vector<vtx2> m_uvs;
	vector<vtx3> m_normals;
	vector<vtx3> m_poss;
	vector<PointStr> m_vtxs;

};
