#include "StdAfx.h"
#include "Model.h"

Model::Model(void)
{
}

Model::~Model(void)
{
}

void Model::Clear()
{
	m_radius = 1.0f;
	m_Facets.clear();
}
