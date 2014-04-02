#pragma once

#include "stdafx.h"
#include "math.h"

class CLensBox;
class CLensBox
{
public:
	CRect m_rectLensBox;
	CPoint m_locationED;
	double m_sizeED;

	CLensBox(void)
	{
		m_rectLensBox.SetRectEmpty();
		m_sizeED = -1;
		m_locationED.SetPoint(0, 0);
	}

	~CLensBox(void)
	{
	}

	void Set(int l, int t, int r, int b)
	{
		m_rectLensBox = CRect(l, t, r, b);
	}

	int GetAdimension()
	{
		if (m_rectLensBox.IsRectEmpty())
			return -1;

		return m_rectLensBox.Width();
	}

	int GetBdimension()
	{
		if (m_rectLensBox.IsRectEmpty())
			return -1;

		return m_rectLensBox.Height();
	}

	CPoint GetGC()
	{
		if (m_rectLensBox.IsRectEmpty())
			return CPoint(-1, -1);

		return m_rectLensBox.CenterPoint();
	}

	int GetDBL(CLensBox* p_altLensBox)
	{
		if (p_altLensBox == NULL)
			return -1;

		if (m_rectLensBox.IsRectEmpty() || p_altLensBox->m_rectLensBox.IsRectEmpty())
			return-1;

		if (p_altLensBox->m_rectLensBox.left > m_rectLensBox.right)
			return p_altLensBox->m_rectLensBox.left - m_rectLensBox.right;

		return m_rectLensBox.right - p_altLensBox->m_rectLensBox.left;
	}

	int GetDBC(CLensBox* p_altLensBox)
	{
		if (p_altLensBox == NULL)
			return -1;

		int DBL = GetDBL(p_altLensBox);
		if (DBL == -1)
			return-1;

		return GetAdimension() + DBL;
	}

	double GetED()
	{
		return m_sizeED;
	}

	void CheckAndRecordEDPoint(int p_X, int p_Y)
	{
		CPoint pointCenter = GetGC();

		double dist = sqrt((double)((pointCenter.x-p_X)*(pointCenter.x-p_X)) + ((pointCenter.y-p_Y)*(pointCenter.y-p_Y))) * 2.0f;

		if (dist > m_sizeED)
		{
			m_sizeED = dist;
			m_locationED.SetPoint(p_X, p_Y);
		}
	}

};

