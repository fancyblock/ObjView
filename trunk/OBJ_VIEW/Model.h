#pragma once
#include <vector>

using namespace std;


union vtx2
{
	struct 
	{
		float x;
		float y;
	};
	float mm[2];
};

union vtx3
{
	struct 
	{
		float x;
		float y;
		float z;
	};
	float mm[3];
};

//顶点
struct Vertex
{
	vtx3 m_position;
	vtx2 m_uv;
	vtx3 m_normal;
};

//三角形
struct Trangle
{
	Vertex m_vtx[3];
};


//用于表示模型的中间结构
class Model
{
public:
	Model(void);
	~Model(void);
	void Clear();

public:
	vector<Trangle> m_Facets;
	float m_radius;

};
