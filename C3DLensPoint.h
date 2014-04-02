#pragma once

#include "stdafx.h"
#include "Vertex.h"

class C3DLensPoint;
class C3DLensPoint
{
public:
	CVertex m_3dVertex;
	C3DLensPoint* next;
	C3DLensPoint* prev;

	int m_ID;
	COLORREF m_rgbColor;

	C3DLensPoint(void)
	{
		next = NULL;

		m_ID = -1;
		m_rgbColor = RGB(0, 0, 0);
	}

	void Set(int p_ID, double p_X, double p_Y, double p_Z, COLORREF p_rgbColor)
	{
		m_ID = p_ID;
		m_rgbColor = p_rgbColor;

		m_3dVertex.Set(p_X, p_Y, p_Z);
	}

	~C3DLensPoint(void)
	{
	}
};

