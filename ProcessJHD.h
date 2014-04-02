#pragma once

#include <iostream>
#include <vector>
#include "C3DPoint.h"

class CProcessJHD
{
public:
	std::vector<C3DPoint> m_3DVecPoint;
	
	void ClockwiseSortPoints(std::vector<C3DPoint> &vPoints);
	bool PointCmp(const float x1,const float y1,const float x2,const float y2,const float cx,const float cy);
	void DrawJHDPoints();
	void ReadRawData(CString JHDRawDataFile);
	void SaveJHDData(CString JHDfilename);
	void Arrange3DPointsClockWise(C3DPoint* Points,CPoint center);
    void ClockwiseSortPoints(std::vector<C3DPoint> &vPoints,CPoint center);
	CProcessJHD(void);
	~CProcessJHD(void);
};

