#include "stdafx.h"
#include "ProcessJHD.h"
#include <regex>

CProcessJHD::CProcessJHD(void)
{
	
}


CProcessJHD::~CProcessJHD(void)
{
}


void CProcessJHD::ReadRawData(CString JHDRawDataFile)
{
	CStdioFile file(JHDRawDataFile, CFile::modeRead); 
	std::vector<double> allXYZ;
	CString strLine; 
	while(file.ReadString(strLine)) 
		  {
			    // find float
			  	std::smatch m;
	            std::regex e (" [-+]?[0-9]*\.?[0-9]+");   // matches words beginning by "sub"
				if ( strLine.Find('X') > -1 || strLine.Find('Y') > -1 || strLine.Find('Z') > -1)
				{
					
					std::string s((LPCTSTR)strLine);

					//remove_if(s.begin(), s.end(), isspace);
					//s.erase(std::remove(s.begin(),s.end(),' '),s.end());
					std::regex_search(s,m,e);

					std::string s1 = m[0].str();
					//CString a(s1.c_str());
					double temp = ::atof(s1.c_str());
					// X is step motor count at 0.0236 mm / count
					if (strLine.Find('X') > -1 )
						temp = 0.0236 * temp;

					allXYZ.push_back(temp);
				}
		}
		int len = allXYZ.size();


		for(int i=0;i<len;i=i+3)
		{

			C3DPoint newC3DPoint;
			newC3DPoint.x = (float)allXYZ[i];
			newC3DPoint.y = (float)allXYZ[i+1];
			newC3DPoint.z = (float)allXYZ[i+2];
			newC3DPoint.m_rgbColor = RGB(0, 255, 0);
			m_3DVecPoint.push_back(newC3DPoint);
		}
		// make poits clolcwise
		ClockwiseSortPoints(m_3DVecPoint);
}

void CProcessJHD::SaveJHDData(CString JHDfilename)
{
	// save a JHD file
	CStdioFile f;
	f.Open(JHDfilename, CFile::modeCreate | CFile::modeWrite);
	
	// 1572 pixel per 30mm = 0.0191 mm/pixel * 100 to match format on right lens
	// 1922 pixel per 30mm = 0.0156 mm/pixel * 100 to match format on left lens
	// 1605 pixel per 30mm = 0.0187 mm/pixel * 100 to match format on left lens
	

		int NumOfPoints = m_3DVecPoint.size();
		CString s;
		s.Format("FileType=JHD\n");
		f.WriteString(s);
		s.Format("NumOfPoints=%d\n",NumOfPoints);
		f.WriteString(s);
		
		for(int i =0 ;i< NumOfPoints;i++)
		{
			s.Format("%.3f %.3f %.3f\n", m_3DVecPoint[i].x,m_3DVecPoint[i].y,m_3DVecPoint[i].z);
			f.WriteString(s);
		}

		f.Close();

}
bool CProcessJHD::PointCmp(const float x1,const float y1,const float x2,const float y2,const float cx,const float cy)
{
    if (x1 >= 0 && x2 < 0)
        return true;
    if (x1 == 0 && x2 == 0)
        return y1 > y2;
  
    int det = (x1 - cx) * (y2 - cy) - (x2 - cx) * (y1 - cy);
    if (det < 0)
        return true;
    if (det > 0)
        return false;
 
    int d1 = (x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy);
    int d2 = (x2 - cx) * (x2 - cx) + (y2 - cy) * (y2 - cy);
    return d1 > d2;
}


void CProcessJHD::ClockwiseSortPoints(std::vector<C3DPoint> &vPoints)
{
    
    float cx,cy;

    double x = 0,y = 0;
    for (int i = 0;i < vPoints.size();i++)
    {
        x += vPoints[i].x;
        y += vPoints[i].y;
    }
    cx = x/vPoints.size();
    cy = y/vPoints.size();

    
    for(int i = 0;i < vPoints.size() - 1;i++)
    {
        for (int j = 0;j < vPoints.size() - i - 1;j++)
        {
			if (PointCmp(vPoints[j].x,vPoints[j].y,vPoints[j+1].x,vPoints[j+1].y,cx,cy))
            {
				C3DPoint tmp = vPoints[j];
                vPoints[j] = vPoints[j + 1];
                vPoints[j + 1] = tmp;
            }
        }
    }

}


void CProcessJHD::ClockwiseSortPoints(std::vector<C3DPoint> &vPoints,CPoint center)
{
	double cx,cy;
	cx = center.x;
	cy =center.y;

    
    for(int i = 0;i < vPoints.size() - 1;i++)
    {
        for (int j = 0;j < vPoints.size() - i - 1;j++)
        {
			if (PointCmp(vPoints[j].x,vPoints[j].y,vPoints[j+1].x,vPoints[j+1].y,cx,cy))
            {
				C3DPoint tmp = vPoints[j];
                vPoints[j] = vPoints[j + 1];
                vPoints[j + 1] = tmp;
            }
        }
    }

}


void CProcessJHD::Arrange3DPointsClockWise(C3DPoint* Points,CPoint center)
{
	
	
	C3DPoint* temp = Points->m_next;
	if (temp == NULL) return;

	while (temp != NULL)
	{
	
		temp->x = temp->x-center.x;
		temp->y = center.y-temp->y;

		m_3DVecPoint.push_back(*temp);
		temp = temp->m_next;
	}
	ClockwiseSortPoints(m_3DVecPoint,CPoint(0,0));

	int i=0;
	temp = Points->m_next;
	while (temp != NULL)
	{
		temp->x = m_3DVecPoint[i].x+center.x;
		temp->y = center.y- m_3DVecPoint[i].y;
		temp->z = m_3DVecPoint[i].z;
		temp = temp->m_next;
		i++;
	}
	

}
