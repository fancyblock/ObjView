#include "StdAfx.h"
#include "ObjReader.h"



ObjReader::ObjReader(void)
{
}

ObjReader::~ObjReader(void)
{
}

//
bool ObjReader::operator()( CFile& file, Model& mdl )
{
	analysisData( file );
	FillModel( mdl );

	m_uvs.clear();
	m_normals.clear();
	m_poss.clear();
	m_vtxs.clear();

	return true;
}

//原始数据进入模型
void ObjReader::FillModel( Model& mdl )
{
	int len = m_vtxs.size();
	Trangle trg;

	//索引值都是从1开始的哦
	for( int i(0); i < len ; i++ )
	{
		PointStr& pTrg = m_vtxs[i];
		for( int j(0); j < 3; j++ )
		{
			//位置
			trg.m_vtx[j].m_position = m_poss[pTrg.pt[j]._pos - 1];
			//UV
			if( pTrg.pt[j]._uv > 0 && ((pTrg.pt[j]._uv - 1) < m_uvs.size()) )
			{
				trg.m_vtx[j].m_uv = m_uvs[pTrg.pt[j]._uv - 1];
			}else
			{
				trg.m_vtx[j].m_uv.x = 0.0f;
				trg.m_vtx[j].m_uv.y = 0.0f;
			}
			//法线
			if( pTrg.pt[j]._normal > 0 && ((pTrg.pt[j]._normal - 1) < m_normals.size()) )
			{
				trg.m_vtx[j].m_normal = m_normals[pTrg.pt[j]._normal - 1];
			}else
			{
				trg.m_vtx[j].m_normal.x = 0.0f;
				trg.m_vtx[j].m_normal.y = 0.0f;
				trg.m_vtx[j].m_normal.z = 0.0f;
			}
		}

		mdl.m_Facets.push_back( trg );
		//计算包围球半径
		;

	}
}

//分析原始数据
bool ObjReader::analysisData( CFile& file )
{
	char lineBuffer[512];
	vtx2 uv;
	vtx3 pos;
	vtx3 nromal;
	PointStr vtx;

	m_uvs.clear();
	m_normals.clear();
	m_poss.clear();
	m_vtxs.clear();

	//信息搜集
	while( readLine( file, lineBuffer ) )
	{
		switch( judgeLine( lineBuffer ) )
		{
		case eVertex:
			GetPosition( lineBuffer, pos );
			m_poss.push_back( pos );
			break;
		case eNormal:
			GetNormal( lineBuffer, nromal );
			m_normals.push_back( nromal );
			break;
		case eUV:
			GetUV( lineBuffer, uv );
			m_uvs.push_back( uv );
			break;
		case eFacet:	//面的情况分很多种
			GetVertex( lineBuffer, vtx );
			m_vtxs.push_back( vtx );
			break;
		default:
			break;
		}
	}

	return true;

}

//得到位置
void ObjReader::GetPosition( char* line, vtx3& pos )
{
	sscanf( line, "v %f %f %f", &pos.x, &pos.y, &pos.z );
}

//得到UV
void ObjReader::GetUV( char* line, vtx2& uv )
{
	sscanf( line, "vt %f %f", &uv.x, &uv.y );
}

//得到法线
void ObjReader::GetNormal( char* line, vtx3& nromal )
{
	sscanf( line, "vn %f %f %f", &nromal.x, &nromal.y, &nromal.z );
}

//得到面信息
void ObjReader::GetVertex( char* line, PointStr& vtx )
{
	//预清0，之后如果索引值从0开始代表不存在该信息（OBJ面信息索引值都是从1开始的）
	for( int i(0); i< 3; i++ )
	{
		vtx.pt[i]._normal = 0;
		vtx.pt[i]._pos = 0;
		vtx.pt[i]._uv = 0;
	}

	//一律当成三角形来看待
	switch( judgeFacet( line ) )
	{
	case eOnlyPos:
		sscanf( line, "f %d %d %d", &vtx.pt[0]._pos, &vtx.pt[1]._pos, &vtx.pt[2]._pos );
		break;
	case ePosUV:
		sscanf( line, "f %d/%d %d/%d %d/%d", &vtx.pt[0]._pos, &vtx.pt[0]._uv,
			&vtx.pt[1]._pos, &vtx.pt[1]._uv, &vtx.pt[2]._pos, &vtx.pt[2]._uv );
		break;
	case ePosNormal:
		sscanf( line, "f %d//%d %d//%d %d//%d", &vtx.pt[0]._pos, &vtx.pt[0]._normal,
			&vtx.pt[1]._pos, &vtx.pt[1]._normal, &vtx.pt[2]._pos, &vtx.pt[2]._normal );
		break;
	case eAll:
		sscanf( line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vtx.pt[0]._pos, &vtx.pt[0]._uv, &vtx.pt[0]._normal,
			&vtx.pt[1]._pos, &vtx.pt[1]._uv, &vtx.pt[1]._normal, &vtx.pt[2]._pos, &vtx.pt[2]._uv, &vtx.pt[2]._normal );
		break;
	default:
		break;
	}
}

//判断面包含的信息
ObjReader::FacetType ObjReader::judgeFacet( char* buff )
{
	if( buff[0] != 'f' )
	{
		return eNonFacet;
	}

	int i = 1;
	//掠过空格
	while( buff[i] == ' ' || buff[i] == '\t' ) i++;

	int slashCount = 0;
	int slashPos[2];
	int slashPosIndex = 0;
	while( buff[i] != ' ' )
	{
		//统计斜杠数量
		if( buff[i] == '\/' )
		{
			slashCount++;
			slashPos[slashPosIndex] = i;
			slashPosIndex++;
		}
		i++;
	}

	//只有顶点
	if( slashCount == 0 )
	{
		return eOnlyPos;
	}
	//顶点和UV
	if( slashCount == 1 )
	{
		return ePosUV;
	}
	//
	if( slashCount == 2 )
	{
		//顶点和法线
		if( (slashPos[1] - slashPos[0]) == 1 )
		{
			return ePosNormal;
		}else
		//顶点UV法线都有了
		{
			return eAll;
		}
	}

}

//判断该行的数据类型
ObjReader::CommandType ObjReader::judgeLine( char* buff )
{
	//位置
	if( buff[0] == 'v' )
	{
		return eVertex;
	}
	//UV
	if( buff[0] == 'v' && buff[0] == 't' )
	{
		return eUV;
	}
	//法线
	if( buff[0] == 'v' && buff[0] == 'n' )
	{
		return eNormal;
	}
	//面
	if( buff[0] == 'f' )
	{
		return eFacet;
	}

	return eOthers;
}

//read one line
bool ObjReader::readLine( CFile& file, char* buff )
{
	int pos = 0;
	char tmp;
	bool finish = false;
	int count;

	while( !finish )
	{
		count = file.Read( &tmp, 1 );

		if( count )
		{
			if( tmp != '\n' )
			{
				buff[pos] = tmp;
				pos++;
			}else
			{
				if( buff[pos-1] != '\\' )
				{
					buff[pos] = 0;
					finish = true;
				}
			}
		}else
		{
			if( pos )
			{
				buff[pos] = 0;
				finish = true;
			}else
			{
				return false;
			}
		}
	}

	return true;
}
