#pragma once

#include "stdafx.h"

class CEdgePattern
{
public:
	bool m_matrixPattern[3][3];
	CPoint m_directionToMove;

	CEdgePattern(void)
	{
		for (int y=0; y<3; y++)
			for (int x=0; x<3; x++)
				m_matrixPattern[x][y] = false;

		m_matrixPattern[1][1] = true;

		m_directionToMove = CPoint(0, 0);
	}

	~CEdgePattern(void)
	{
	}

	void Set(bool p_00, bool p_10, bool p_20, bool p_01, bool p_21, bool p_02, bool p_12, bool p_22, CPoint p_directionToMove)
	{
		m_matrixPattern[0][0] = p_00;
		m_matrixPattern[1][0] = p_10;
		m_matrixPattern[2][0] = p_20;

		m_matrixPattern[0][1] = p_01;
		m_matrixPattern[2][1] = p_21;

		m_matrixPattern[0][2] = p_02;
		m_matrixPattern[1][2] = p_12;
		m_matrixPattern[2][2] = p_22;

		m_directionToMove = p_directionToMove;
	}
};

