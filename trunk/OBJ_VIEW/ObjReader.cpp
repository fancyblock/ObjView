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

//ԭʼ���ݽ���ģ��
void ObjReader::FillModel( Model& mdl )
{
	int len = m_vtxs.size();
	Trangle trg;

	//����ֵ���Ǵ�1��ʼ��Ŷ
	for( int i(0); i < len ; i++ )
	{
		PointStr& pTrg = m_vtxs[i];
		for( int j(0); j < 3; j++ )
		{
			//λ��
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
			//����
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
		//�����Χ��뾶
		;

	}
}

//����ԭʼ����
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

	//��Ϣ�Ѽ�
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
		case eFacet:	//�������ֺܶ���
			GetVertex( lineBuffer, vtx );
			m_vtxs.push_back( vtx );
			break;
		default:
			break;
		}
	}

	return true;

}

//�õ�λ��
void ObjReader::GetPosition( char* line, vtx3& pos )
{
	sscanf( line, "v %f %f %f", &pos.x, &pos.y, &pos.z );
}

//�õ�UV
void ObjReader::GetUV( char* line, vtx2& uv )
{
	sscanf( line, "vt %f %f", &uv.x, &uv.y );
}

//�õ�����
void ObjReader::GetNormal( char* line, vtx3& nromal )
{
	sscanf( line, "vn %f %f %f", &nromal.x, &nromal.y, &nromal.z );
}

//�õ�����Ϣ
void ObjReader::GetVertex( char* line, PointStr& vtx )
{
	//Ԥ��0��֮���������ֵ��0��ʼ�������ڸ���Ϣ��OBJ����Ϣ����ֵ���Ǵ�1��ʼ�ģ�
	for( int i(0); i< 3; i++ )
	{
		vtx.pt[i]._normal = 0;
		vtx.pt[i]._pos = 0;
		vtx.pt[i]._uv = 0;
	}

	//һ�ɵ���������������
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

//�ж����������Ϣ
ObjReader::FacetType ObjReader::judgeFacet( char* buff )
{
	if( buff[0] != 'f' )
	{
		return eNonFacet;
	}

	int i = 1;
	//�ӹ��ո�
	while( buff[i] == ' ' || buff[i] == '\t' ) i++;

	int slashCount = 0;
	int slashPos[2];
	int slashPosIndex = 0;
	while( buff[i] != ' ' )
	{
		//ͳ��б������
		if( buff[i] == '\/' )
		{
			slashCount++;
			slashPos[slashPosIndex] = i;
			slashPosIndex++;
		}
		i++;
	}

	//ֻ�ж���
	if( slashCount == 0 )
	{
		return eOnlyPos;
	}
	//�����UV
	if( slashCount == 1 )
	{
		return ePosUV;
	}
	//
	if( slashCount == 2 )
	{
		//����ͷ���
		if( (slashPos[1] - slashPos[0]) == 1 )
		{
			return ePosNormal;
		}else
		//����UV���߶�����
		{
			return eAll;
		}
	}

}

//�жϸ��е���������
ObjReader::CommandType ObjReader::judgeLine( char* buff )
{
	//λ��
	if( buff[0] == 'v' )
	{
		return eVertex;
	}
	//UV
	if( buff[0] == 'v' && buff[0] == 't' )
	{
		return eUV;
	}
	//����
	if( buff[0] == 'v' && buff[0] == 'n' )
	{
		return eNormal;
	}
	//��
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
