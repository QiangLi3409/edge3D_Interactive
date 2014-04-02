#include "stdafx.h"

#pragma once
class C3DPoint;

class C3DPoint
{
public:
	int m_ID;

	float x;
	float y;
	float z;

	COLORREF m_rgbColor;
	BYTE m_typeClassifier;

	C3DPoint* m_next;

	C3DPoint(void)
	{
		Create();
	}

	C3DPoint(float xx, float yy, float zz)
	{
		x = xx;
		y = yy;
		z = zz;

	}



	~C3DPoint(void)
	{
	}

	void Create()
	{
		m_ID = -1;

		x = y = z = 0.0f;
		m_rgbColor = RGB(0, 0, 0);
		m_typeClassifier = 0;

		m_next = NULL;
	}

	void Create(float p_x, float p_y, float p_z, COLORREF p_rgbColor, C3DPoint* p_prev)
	{
		x = p_x;
		y = p_y;
		z = p_z;
		m_rgbColor = p_rgbColor;

		m_next = NULL;

		p_prev->m_next = this;
	}

};

