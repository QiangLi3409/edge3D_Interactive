#pragma once
#include "C3DPoint.h"
#include "math.h"
#include "LensBox.h"	

class C3DCloud
{
public:
	C3DPoint* m_start3DPoint;
	C3DPoint* m_cur3DPoint;
	int m_count3DPoints;
	CSize m_sizeMaxExtents;
	CSize m_sizeMinExtents;
	CUIntArray m_arrayYindexes;
	CImage m_imageData;

	C3DCloud(void)
	{
		m_cur3DPoint = m_start3DPoint = new C3DPoint;
		m_start3DPoint->Create();
		
		Reset();

		m_imageData.Create(150000, 500, 24);
	}

	~C3DCloud(void)
	{
	}

	void Reset()
	{
		m_count3DPoints = 0;
		m_sizeMaxExtents = CSize(-1, -1);
		m_sizeMinExtents = CSize(10000, 10000);
		
		RemoveAll();
	}

	void RemoveAll()
	{
		C3DPoint* indexC3DPoint = m_start3DPoint->m_next;
		C3DPoint* removeC3DPoint;
					
		while (indexC3DPoint != NULL)
		{
			removeC3DPoint = indexC3DPoint;
			indexC3DPoint = indexC3DPoint->m_next;
			delete removeC3DPoint;
		}

		m_start3DPoint->m_next = NULL;
		m_cur3DPoint = m_start3DPoint;
	}

	C3DPoint* CreatePoint(float p_x, float p_y, float p_z, COLORREF p_rgbColor, int p_ID=-1)
	{
		C3DPoint* new3DPoint = new C3DPoint;
		new3DPoint->Create(p_x, p_y, p_z, p_rgbColor, m_cur3DPoint);
		new3DPoint->m_ID = p_ID;
		m_cur3DPoint = new3DPoint;
		m_count3DPoints++;

		if (p_x > m_sizeMaxExtents.cx)
			m_sizeMaxExtents.cx = p_x;
		if (p_y > m_sizeMaxExtents.cy)
			m_sizeMaxExtents.cy = p_y;

		if (p_x < m_sizeMinExtents.cx)
			m_sizeMinExtents.cx = p_x;
		if (p_y < m_sizeMinExtents.cy)
			m_sizeMinExtents.cy = p_y;

		return m_cur3DPoint;
	}

	void Draw(CDC* p_dc, CSize p_scaleDisplay, CPoint p_offsetDisplay, CLensBox* p_LensBox=NULL)
	{
		C3DPoint* index3DPoint = m_start3DPoint->m_next;
		C3DPoint* index3DPoint2 = NULL; 

//		CPen pen;
//		pen.CreatePen(PS_SOLID, 1, RGB(0,255,255));
//		p_dc->SelectObject(&pen);
					
		while (index3DPoint != NULL)
		{
			if (index3DPoint2 != NULL)
			{
				if (p_LensBox != NULL)
					p_LensBox->CheckAndRecordEDPoint(index3DPoint->x, index3DPoint->y);

				p_dc->FillSolidRect((p_offsetDisplay.x+(index3DPoint->x/p_scaleDisplay.cx))-1, (p_offsetDisplay.y+(index3DPoint->y/p_scaleDisplay.cy))-1, 1, 1, index3DPoint->m_rgbColor);
//				p_dc->MoveTo(index3DPoint2->x/p_scaleDisplay.cx, index3DPoint2->y/p_scaleDisplay.cy);
//				p_dc->LineTo(index3DPoint->x/p_scaleDisplay.cx, index3DPoint->y/p_scaleDisplay.cy);
			}

			index3DPoint2 = index3DPoint;

			if (index3DPoint2->m_next == NULL)
			{
				p_dc->FillSolidRect((p_offsetDisplay.x+(index3DPoint->x/p_scaleDisplay.cx))-1, (p_offsetDisplay.y+(index3DPoint->y/p_scaleDisplay.cy))-1, 1, 1, index3DPoint->m_rgbColor);
//				p_dc->MoveTo(index3DPoint2->x/p_scaleDisplay.cx, index3DPoint2->y/p_scaleDisplay.cy);
//				p_dc->LineTo(m_start3DPoint->m_next->x/p_scaleDisplay.cx, m_start3DPoint->m_next->y/p_scaleDisplay.cy);
			}

			index3DPoint = index3DPoint->m_next;
		}
	}

	void DrawPointsWithSkip(CDC* p_dc, CSize p_scaleDisplay, CPoint p_offsetDisplay, int p_countToSkip)
	{
		C3DPoint* index3DPoint = m_start3DPoint->m_next;
		C3DPoint* index3DPoint2 = NULL; 
					
		while (index3DPoint != NULL)
		{
			if (index3DPoint != NULL)
				p_dc->FillSolidRect((p_offsetDisplay.x+(index3DPoint->x/p_scaleDisplay.cx))-1, (p_offsetDisplay.y+(index3DPoint->y/p_scaleDisplay.cy))-1, 3, 3, index3DPoint->m_rgbColor);

			index3DPoint2 = index3DPoint;

			if (index3DPoint2->m_next == NULL)
				p_dc->FillSolidRect((p_offsetDisplay.x+(m_start3DPoint->m_next->x/p_scaleDisplay.cx))-1, (p_offsetDisplay.y+(m_start3DPoint->m_next->y/p_scaleDisplay.cy))-1, 3, 3, m_start3DPoint->m_next->m_rgbColor);

			index3DPoint = index3DPoint->m_next;
		}
	}

	void DrawPointsWithSkip(CDC* p_dc, float p_scaleDisplay, CPoint p_offsetDisplay, int p_countToSkip, int p_Size=1)
	{
		C3DPoint* index3DPoint = m_start3DPoint->m_next;
		C3DPoint* index3DPoint2 = NULL; 

		if (index3DPoint == NULL)
			return;
					
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, index3DPoint->m_rgbColor);
		p_dc->SelectObject(&pen);

		p_dc->MoveTo((p_offsetDisplay.x+(index3DPoint->x/p_scaleDisplay)), (p_offsetDisplay.y+(index3DPoint->y/p_scaleDisplay)));

		while (index3DPoint != NULL)
		{
			if (index3DPoint != NULL)
				if (p_Size != 1)
					p_dc->LineTo((p_offsetDisplay.x+(index3DPoint->x/p_scaleDisplay)), (p_offsetDisplay.y+(index3DPoint->y/p_scaleDisplay)));
				else
					p_dc->FillSolidRect((p_offsetDisplay.x+(index3DPoint->x/p_scaleDisplay))-p_Size, (p_offsetDisplay.y+(index3DPoint->y/p_scaleDisplay))-p_Size, (p_Size*2)+1, (p_Size*2)+1, index3DPoint->m_rgbColor);

			index3DPoint2 = index3DPoint;

			if (index3DPoint2->m_next == NULL && p_Size == 1)
				p_dc->FillSolidRect((p_offsetDisplay.x+(m_start3DPoint->m_next->x/p_scaleDisplay))-p_Size, (p_offsetDisplay.y+(m_start3DPoint->m_next->y/p_scaleDisplay))-p_Size, (p_Size*2)+1, (p_Size*2)+1, m_start3DPoint->m_next->m_rgbColor);

			index3DPoint = index3DPoint->m_next;
		}

		index3DPoint = m_start3DPoint->m_next;
		if (p_Size != 1)
			p_dc->LineTo((p_offsetDisplay.x+(index3DPoint->x/p_scaleDisplay)), (p_offsetDisplay.y+(index3DPoint->y/p_scaleDisplay)));
	}

	void TransformMMtoPixels(float p_factorForConversion)
	{
		C3DPoint* indexC3DPoint = m_start3DPoint->m_next;
					
		while (indexC3DPoint != NULL)
		{
			indexC3DPoint->x = indexC3DPoint->x/p_factorForConversion;
			indexC3DPoint->y = indexC3DPoint->y/p_factorForConversion;
			indexC3DPoint->z = indexC3DPoint->z/p_factorForConversion;

			indexC3DPoint = indexC3DPoint->m_next;
		}
	}

	void NormalizeToOrigin()
	{
		int offX = (m_sizeMaxExtents.cx + m_sizeMinExtents.cx)/2;
		int offY = (m_sizeMaxExtents.cy + m_sizeMinExtents.cy)/2;
					
		C3DPoint* indexC3DPoint = m_start3DPoint->m_next;
					
		while (indexC3DPoint != NULL)
		{
			indexC3DPoint->x = indexC3DPoint->x-offX;
			indexC3DPoint->y = indexC3DPoint->y-offY;

			indexC3DPoint = indexC3DPoint->m_next;
		}
	}

	void FlipAroundY()
	{
		C3DPoint* indexC3DPoint = m_start3DPoint->m_next;
					
		while (indexC3DPoint != NULL)
		{
			indexC3DPoint->x = indexC3DPoint->x * -1.0f;

			indexC3DPoint = indexC3DPoint->m_next;
		}
	}

	void FlipAroundX()
	{
		C3DPoint* indexC3DPoint = m_start3DPoint->m_next;
					
		while (indexC3DPoint != NULL)
		{
			indexC3DPoint->y = indexC3DPoint->y * -1.0f;

			indexC3DPoint = indexC3DPoint->m_next;
		}
	}

	void Rotate(float p_degreeOfRotation)
	{
		C3DPoint* indexC3DPoint = m_start3DPoint->m_next;

		float rads = p_degreeOfRotation * (3.14159f/180.0);
					
		while (indexC3DPoint != NULL)
		{
			indexC3DPoint->x = (indexC3DPoint->x * cos(rads)) - (indexC3DPoint->y * sin(rads));
			indexC3DPoint->y = (indexC3DPoint->x * sin(rads)) + (indexC3DPoint->y * cos(rads));

			indexC3DPoint = indexC3DPoint->m_next;
		}
	}

	void Save()
	{
		CString s;
		CStdioFile fx;
		fx.Open("c:\\data\\test3_left_x.txt", CFile::modeCreate | CFile::modeWrite);
		CStdioFile fy;
		fy.Open("c:\\data\\test3_left_y.txt", CFile::modeCreate | CFile::modeWrite);
					
		C3DPoint* indexC3DPoint = m_start3DPoint->m_next;
					
		while (indexC3DPoint != NULL)
		{
			s.Format("%.3f", indexC3DPoint->x*100.0);
			s += '\n';
			fx.WriteString(s);

			s.Format("%.3f", -1.0 * indexC3DPoint->y*100.0);
			s += '\n';
			fy.WriteString(s);

			indexC3DPoint = indexC3DPoint->m_next;
		}

		fx.Close();
		fy.Close();
	}

	void SaveJimData()
	{
		CString s;
		CStdioFile fx;
//		fx.Open("c:\\data\\studio-x-841-h-1000-08-53-Thursday-February-17.txt ", CFile::modeCreate | CFile::modeWrite);
		fx.Open("c:\\data\\test2.ply ", CFile::modeCreate | CFile::modeWrite);
					
		C3DPoint* indexC3DPoint = m_start3DPoint->m_next;

		
//		s.Format("%d", m_count3DPoints);
//		s += '\n';
//		fx.WriteString(s);
							
		{ // header for .ply file
			CString h;
			h = "ply";
			h += '\n';
			h += "format ascii 1.0";
			h += '\n';
			h += "comment created by tjr";
			h += '\n';
			s.Format("element vertex %d", m_count3DPoints+1);
			h += s;
			h += '\n';
			h += "property float32 x";
			h += '\n';
			h += "property float32 y";
			h += '\n';
			h += "property float32 z";
			h += '\n';
			s.Format("element face %d", m_count3DPoints);
			h += s;
			h += '\n';
			h += "property list uint8 int32 vertex_indices";
			h += '\n';
			h += "end_header";
			h += '\n';
			fx.WriteString(h);
		}

		float as = 0.125f;

		CRect r(indexC3DPoint->x/0.4f/0.3f, indexC3DPoint->y/as, indexC3DPoint->x/0.4f/0.3f, indexC3DPoint->y/as);
							
		while (indexC3DPoint != NULL)
		{
			if (indexC3DPoint->y/as < r.top)
				r.top = indexC3DPoint->y/as;

			if (indexC3DPoint->y/as > r.bottom)
				r.bottom = indexC3DPoint->y/as;

			if (indexC3DPoint->x/0.4f/0.3f < r.left)
				r.left = indexC3DPoint->x/0.4f/0.3f;

			if (indexC3DPoint->x/0.4f/0.3f > r.right)
				r.right = indexC3DPoint->x/0.4f/0.3f;

			indexC3DPoint = indexC3DPoint->m_next;
		}

		CPoint p = r.CenterPoint();
		s.Format("%.3f %.3f %.3f", -1.0*(float)p.x, (float)p.y, 7.0f*1000.0f);
		s += '\n';
		fx.WriteString(s);
							
		indexC3DPoint = m_start3DPoint->m_next;
		while (indexC3DPoint != NULL)
		{
//			s.Format("%.3f %.3f %.3f %d", indexC3DPoint->x/0.4f/0.3f, indexC3DPoint->y/1.3f, indexC3DPoint->z, 0);
			s.Format("%.3f %.3f %.3f", -1.0*(indexC3DPoint->x/0.4f/0.3f), indexC3DPoint->y/as, indexC3DPoint->z*1000.0f);
			s += '\n';
			fx.WriteString(s);

			indexC3DPoint = indexC3DPoint->m_next;
		}

		
		for (int i=1; i<m_count3DPoints; i++)
		{
			s.Format("3 %d %d %d", 0, i, i+1);
			s += '\n';
			fx.WriteString(s);
		}

		s.Format("3 %d %d %d", 0, m_count3DPoints, 1);
		s += '\n';
		fx.WriteString(s);

		fx.Close();
	}


	void SaveLaserLineData()
	{
		CString s;
		CStdioFile fx;
		fx.Open("c:\\data\\test4.ply ", CFile::modeCreate | CFile::modeWrite);
					
		C3DPoint* indexC3DPoint = m_start3DPoint->m_next;

		{ // header for .ply file
			CString h;
			h = "ply";
			h += '\n';
			h += "format ascii 1.0";
			h += '\n';
			h += "comment created by tjr";
			h += '\n';
			s.Format("element vertex %d", m_count3DPoints);
			h += s;
			h += '\n';
			h += "property float32 x";
			h += '\n';
			h += "property float32 y";
			h += '\n';
			h += "property float32 z";
			h += '\n';
			s.Format("element face %d", 0);
			h += s;
			h += '\n';
			h += "property list uint8 int32 vertex_indices";
			h += '\n';
			h += "end_header";
			h += '\n';
			fx.WriteString(h);
		}

		indexC3DPoint = m_start3DPoint->m_next;
		while (indexC3DPoint != NULL)
		{
			s.Format("%.3f %.3f %.3f", indexC3DPoint->x, indexC3DPoint->y, indexC3DPoint->z*10.0f);
			s += '\n';
			fx.WriteString(s);

			indexC3DPoint = indexC3DPoint->m_next;
		}

		fx.Close();
	}

	void SaveTedData()
	{
		CString s;
		CStdioFile fx;
		fx.Open("c:\\data\\test3.ply ", CFile::modeCreate | CFile::modeWrite);
					
		C3DPoint* indexC3DPoint = m_start3DPoint->m_next;
							
		{ // header for .ply file
			CString h;
			h = "ply";
			h += '\n';
			h += "format ascii 1.0";
			h += '\n';
			h += "comment created by tjr";
			h += '\n';
			s.Format("element vertex %d", (2*m_count3DPoints)+1);
			h += s;
			h += '\n';
			h += "property float32 x";
			h += '\n';
			h += "property float32 y";
			h += '\n';
			h += "property float32 z";
			h += '\n';
			s.Format("element face %d", 2*m_count3DPoints);
			h += s;
			h += '\n';
			h += "property list uint8 int32 vertex_indices";
			h += '\n';
			h += "end_header";
			h += '\n';
			fx.WriteString(h);
		}

		float as = 0.125f;

		CRect r(indexC3DPoint->x, indexC3DPoint->y, indexC3DPoint->x, indexC3DPoint->y);
							
		while (indexC3DPoint != NULL)
		{
			if (indexC3DPoint->y < r.top)
				r.top = indexC3DPoint->y;

			if (indexC3DPoint->y > r.bottom)
				r.bottom = indexC3DPoint->y;

			if (indexC3DPoint->x < r.left)
				r.left = indexC3DPoint->x;

			if (indexC3DPoint->x > r.right)
				r.right = indexC3DPoint->x;

			indexC3DPoint = indexC3DPoint->m_next;
		}

		CPoint p = r.CenterPoint();
		s.Format("%.3f %.3f %.3f", -1.0*(float)p.x, (float)p.y, 200.0f);
		s += '\n';
		fx.WriteString(s);
							
		indexC3DPoint = m_start3DPoint->m_next;
		while (indexC3DPoint != NULL)
		{
			s.Format("%.3f %.3f %.3f", -1.0*(indexC3DPoint->x), indexC3DPoint->y, indexC3DPoint->z);
			s += '\n';
			fx.WriteString(s);

			indexC3DPoint = indexC3DPoint->m_next;
		}

		indexC3DPoint = m_start3DPoint->m_next;
		while (indexC3DPoint != NULL)
		{
			s.Format("%.3f %.3f %.3f", -1.0*(indexC3DPoint->x), indexC3DPoint->y, indexC3DPoint->z-120.0f);
			s += '\n';
			fx.WriteString(s);

			indexC3DPoint = indexC3DPoint->m_next;
		}

		for (int i=1; i<m_count3DPoints; i++)
		{
			s.Format("3 %d %d %d", 0, i, i+1);
			s += '\n';
			fx.WriteString(s);
		}

		s.Format("3 %d %d %d", 0, m_count3DPoints, 1);
		s += '\n';
		fx.WriteString(s);

//		s.Format("4 %d %d %d %d", 1, 2, m_count3DPoints+2, m_count3DPoints+1);
//		s += '\n';
//		fx.WriteString(s);

		for (int i=1; i<m_count3DPoints; i++)
		{
			s.Format("4 %d %d %d %d", i, i+1, m_count3DPoints+i+1, m_count3DPoints+i);
			s += '\n';
			fx.WriteString(s);
		}

		s.Format("4 %d %d %d %d", m_count3DPoints, 1, m_count3DPoints+2, m_count3DPoints+1);
		s += '\n';
		fx.WriteString(s);

		fx.Close();
	}

	float GetID_Z(int p_ID)
	{
		C3DPoint* indexC3DPoint = m_start3DPoint->m_next;
					
		while (indexC3DPoint != NULL)
		{
			if (indexC3DPoint->m_ID == p_ID)
				return indexC3DPoint->z;

			indexC3DPoint = indexC3DPoint->m_next;
		}

		return 0.0f;
	}

};

