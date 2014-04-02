#include "stdafx.h"
#include "EdgePattern.h"
#include "ImageStone.h"

#pragma once

#define MATRIX_MAXWIDTH 5168
#define MATRIX_MAXHEIGHT 2907

class CImageAnalyzer
{
public:
	bool matrixThreshold[MATRIX_MAXWIDTH][MATRIX_MAXHEIGHT];
	int m_sizeWidth;
	int m_sizeHeight;

	bool m_isSet;

	CPtrArray m_arrayEdgePatterns;


	CImageAnalyzer(void)
	{
		m_isSet = false;
	}

	~CImageAnalyzer(void)
	{
	}

	void Set(FCObjImage* p_imageToAnalyze)
	{
		m_isSet = false;

		g_sizeImageAnalyzer.cx = m_sizeWidth = p_imageToAnalyze->Width();
		g_sizeImageAnalyzer.cy = m_sizeHeight = p_imageToAnalyze->Height();

//		int indexByteArray = p_imageToAnalyze->GetPitch() * (m_sizeHeight-1);
		int indexByteArray = 0;
		BYTE* b = (BYTE*)p_imageToAnalyze->GetMemStart();

		for (int y=m_sizeHeight-1; y>=0; y--)
		{
			for (int x=0; x<m_sizeWidth; x++)
			{
				matrixThreshold[x][y] = (b[indexByteArray] < 230 && b[indexByteArray+1] < 230 && b[indexByteArray+2] < 230);
				
				indexByteArray += 3;
			}
		}

		m_isSet = true;
	}
};

