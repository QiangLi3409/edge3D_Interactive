#pragma once

#include "stdafx.h"
#include "C3DLensPoint.h"
#include "LensBox.h"

class C3DLens
{
public:
	C3DLensPoint* m_firstLensPoint;
	C3DLensPoint* m_lastLensPoint;

	CLensBox m_lensboxLensDim;

	C3DLens(void)
	{
		m_firstLensPoint = NULL;
		m_lastLensPoint = NULL;
	}

	void AddPoint(int p_ID, double p_X, double p_Y, double p_Z, COLORREF p_rgbColor)
	{
		C3DLensPoint* curLensPoint = new C3DLensPoint;
		curLensPoint->Set(p_ID, p_X, p_Y, p_Z, p_rgbColor);

		if (m_firstLensPoint = NULL)
			m_firstLensPoint = curLensPoint;
		else
			m_lastLensPoint->prev = curLensPoint;

		m_lastLensPoint = curLensPoint;
	}

	void RemoveAll()
	{
		C3DLensPoint* curLensPoint = m_firstLensPoint;

		while (m_firstLensPoint != NULL)
		{
			if (curLensPoint != NULL)
			{
				m_firstLensPoint = m_firstLensPoint->next;
				delete curLensPoint;
			}
		}
	}

	~C3DLens(void)
	{
	}
};

