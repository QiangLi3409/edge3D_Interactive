
// edge3D StudioDlg.cpp : implementation file
//

#include "stdafx.h"
#include "edge3D Studio.h"
#include "edge3D StudioDlg.h"
#include "afxdialogex.h"
#include "EdgePattern.h"
#include "include/effect/smooth_edge.h"
#include "include/effect/reduce_noise.h"
#include "LaserLinecannerDialog.h"
#include "ManualMotionCtlDlg.h"

#include "ProcessJHD.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cedge3DStudioDlg dialog

float g_JamesPoints[36]  = {7883,	3747,
1733,	3747,
13525,	3747,
7910,	3747,
7910,	7254,
7910,	498,
4065,	178,
2050,	2740,
2720,	6679,
3836,	6803,
5808,7173,
11413,	6863,
12275,	6247,
13503,	4574,
12824,	2108,
11717,	505,
7658,	631,
7883,	3747};
Cedge3DStudioDlg::Cedge3DStudioDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cedge3DStudioDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_objectImageAnalyzer = NULL;
	m_curC3DPoint = NULL;
	m_startC3DPoint = NULL;

	// 
	


}

void Cedge3DStudioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SHOW_LENSROI, m_btnShowLensROIControl);
	DDX_Control(pDX, IDC_EDIT_GAMMA, m_editGamma);
	DDX_Control(pDX, IDC_EDIT_THRES, m_editThres);
	DDX_Control(pDX, IDC_CHECK_SHOW_EDGE_1, m_btnShowEdge1);
	DDX_Control(pDX, IDC_CHECK_SHOW_EDGE_2, m_btnShowEdge2);
	DDX_Control(pDX, IDC_CHECK_SHOW_EDGE_3, m_btnShowEdge3);
	DDX_Control(pDX, IDC_CHECK_SHOW_EDGE_4, m_btnShowEdge4);
	DDX_Control(pDX, IDC_CHECK_SHOW_EDGE_5, m_btnShowEdge5);
	DDX_Control(pDX, IDC_CHECK1, m_btnDoLeftLensBox);
	DDX_Control(pDX, IDC_CHECK2, m_btnDoRightLensBox);
	DDX_Control(pDX, IDC_EDIT1, m_editAleft);
	DDX_Control(pDX, IDC_EDIT2, m_editBleft);
	DDX_Control(pDX, IDC_EDIT3, m_editEDleft);
	DDX_Control(pDX, IDC_EDIT4, m_editAright);
	DDX_Control(pDX, IDC_EDIT5, m_editBright);
	DDX_Control(pDX, IDC_EDIT6, m_editEDright);
	DDX_Control(pDX, IDC_EDIT7, m_editDBL);
	DDX_Control(pDX, IDC_EDIT8, m_editDBC);
}

BEGIN_MESSAGE_MAP(Cedge3DStudioDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK_SHOW_LENSROI, &Cedge3DStudioDlg::OnBnClickedCheckShowLensroi)
	ON_BN_CLICKED(IDC_BUTTON_SAVEXYDATA, &Cedge3DStudioDlg::OnBnClickedButtonSavexydata)
	ON_BN_CLICKED(IDC_BUTTON_LOADJIMDATA, &Cedge3DStudioDlg::OnBnClickedButtonLoadjimdata)
	ON_BN_CLICKED(IDC_BUTTON_LOADIMAGE, &Cedge3DStudioDlg::OnBnClickedButtonLoadimage)
	ON_BN_CLICKED(IDC_BUTTON2, &Cedge3DStudioDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Cedge3DStudioDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK_SHOW_EDGE_1, &Cedge3DStudioDlg::OnBnClickedCheckShowEdge1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_EDGE_2, &Cedge3DStudioDlg::OnBnClickedCheckShowEdge2)
	ON_BN_CLICKED(IDC_CHECK_SHOW_EDGE_3, &Cedge3DStudioDlg::OnBnClickedCheckShowEdge3)
	ON_BN_CLICKED(IDC_CHECK_SHOW_EDGE_4, &Cedge3DStudioDlg::OnBnClickedCheckShowEdge4)
	ON_BN_CLICKED(IDC_CHECK_SHOW_EDGE_5, &Cedge3DStudioDlg::OnBnClickedCheckShowEdge5)
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_SAVEXYDATA3, &Cedge3DStudioDlg::OnBnClickedButtonSavexydata3)
	ON_BN_CLICKED(IDC_BUTTON_LOADLASERLINE, &Cedge3DStudioDlg::OnBnClickedButtonLoadlaserline)
	ON_BN_CLICKED(IDC_CHECK1, &Cedge3DStudioDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &Cedge3DStudioDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON4, &Cedge3DStudioDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &Cedge3DStudioDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_IMAGESAVE, &Cedge3DStudioDlg::OnBnClickedImagesave)
	ON_BN_CLICKED(IDC_CHECK3, &Cedge3DStudioDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_MANUAL_DLG, &Cedge3DStudioDlg::OnBnClickedManualDlg)
	
	ON_BN_CLICKED(IDC_READMODBUS, &Cedge3DStudioDlg::OnBnClickedReadmodbus)
END_MESSAGE_MAP()


// Cedge3DStudioDlg message handlers

BOOL Cedge3DStudioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_displayScale = 2.0f;
	m_pointScanOrigin = CPoint(0, 0);
	m_pointDisplayOrigin = CPoint(-1, -1);
	m_curC3DPoint = m_startC3DPoint = new C3DPoint;
	m_startC3DPoint->Create();
	m_countLensPoints = 0;

	m_scaleJimData = 9.0f;
	m_scaleLaserData = 9.0f;
	m_rotateJimData = 0.0f;

	m_indexTraceEdge = 0;
	m_colorTraceEdge = RGB(255,0,0);

	m_isScanOriginSelected = false;
	m_btnShowLensROIControl.SetCheck(BST_CHECKED);
	m_btnShowEdge1.SetCheck(BST_CHECKED);
	m_btnDoLeftLensBox.SetCheck(BST_CHECKED);
	m_btnDoRightLensBox.SetCheck(BST_UNCHECKED);

	m_editGamma.SetWindowText("-1");
	m_editThres.SetWindowText("-1");

	m_objectImageAnalyzer = new CImageAnalyzer;
/*
	m_cloudJimData.CreatePoint(-107.000*0.0236f, 19.000, 0.470, RGB(0,0,255));
	m_cloudJimData.CreatePoint(-423.000*0.0236f, 16.000, -0.420, RGB(0,0,255));
	m_cloudJimData.CreatePoint(-775.000*0.0236f, 12.000, 0.070, RGB(0,0,255));
	m_cloudJimData.CreatePoint(-786.000*0.0236f, 8.000, -0.920, RGB(0,0,255));
	m_cloudJimData.CreatePoint(-969.000*0.0236f, 4.000, -0.570, RGB(0,0,255));
	m_cloudJimData.CreatePoint(-969.000*0.0236f, 0.000, -0.850, RGB(0,0,255));
	m_cloudJimData.CreatePoint(-1075.000*0.0236f, -4.000, -0.820, RGB(0,0,255));
	m_cloudJimData.CreatePoint(-1024.000*0.0236f, -8.000, -0.700, RGB(0,0,255));
	m_cloudJimData.CreatePoint(-874.000*0.0236f, -12.000, -0.370, RGB(0,0,255));
	m_cloudJimData.CreatePoint(184.000*0.0236f, -12.000, 1.000, RGB(0,0,255));
	m_cloudJimData.CreatePoint(672.000*0.0236f, -8.000, 0.770, RGB(0,0,255));
	m_cloudJimData.CreatePoint(870.000*0.0236f, -4.000, 1.000, RGB(0,0,255));
	m_cloudJimData.CreatePoint(870.000*0.0236f, 0.000, -0.580, RGB(0,0,255));
	m_cloudJimData.CreatePoint(826.000*0.0236f, 4.000, 1.000, RGB(0,0,255));
	m_cloudJimData.CreatePoint(826.000*0.0236f, 8.000, 1.000, RGB(0,0,255));
	m_cloudJimData.CreatePoint(652.000*0.0236f, 12.000, 1.000, RGB(0,0,255));
	m_cloudJimData.CreatePoint(588.000*0.0236f, 16.000, 1.000, RGB(0,0,255));
	m_cloudJimData.CreatePoint(85.000*0.0236f, 19.000, 1.000, RGB(0,0,255));
	m_cloudJimData.NormalizeToOrigin();
	m_cloudJimData.Save();
	m_cloudJimData.TransformMMtoPixels(0.0156f);
*/
	m_cloudJimData2.CreatePoint(-107.000*0.024280f, 19.000, 0.470, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(-423.000*0.024280f, 16.000, -0.420, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(-775.000*0.024280f, 12.000, 0.070, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(-786.000*0.024280f, 8.000, -0.920, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(-969.000*0.024280f, 4.000, -0.570, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(-969.000*0.024280f, 0.000, -0.850, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(-1075.000*0.024280f, -4.000, -0.820, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(-1024.000*0.024280f, -8.000, -0.700, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(-874.000*0.024280f, -12.000, -0.370, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(184.000*0.024280f, -12.000, 1.000, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(672.000*0.024280f, -8.000, 0.770, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(870.000*0.024280f, -4.000, 1.000, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(870.000*0.024280f, 0.000, -0.580, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(826.000*0.024280f, 4.000, 1.000, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(826.000*0.024280f, 8.000, 1.000, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(652.000*0.024280f, 12.000, 1.000, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(588.000*0.024280f, 16.000, 1.000, RGB(255,0,255));
	m_cloudJimData2.CreatePoint(85.000*0.024280f, 19.000, 1.000, RGB(255,0,255));
	m_cloudJimData2.NormalizeToOrigin();
	m_cloudJimData2.Save();
	m_cloudJimData2.TransformMMtoPixels(0.0156f);

	m_indexLensBox = 0;



	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cedge3DStudioDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
			CButton* pBtn = (CButton*) GetDlgItem(IDC_CHECK3);
			if (pBtn->GetCheck())
			{
				CImage memImageDisplay;
				float displayScale = 10.0;

				// find min x and max x 
				float minx,maxx,miny,maxy;

				minx = maxx = g_JamesPoints[0];
				miny = maxy = g_JamesPoints[1];

				for(int i=2;i<36;i+=2)
				{
					if(minx > g_JamesPoints[i]) minx = g_JamesPoints[i];
					if(maxx < g_JamesPoints[i]) maxx = g_JamesPoints[i];
					if(miny > g_JamesPoints[i+1]) miny = g_JamesPoints[i+1];
					if(maxy < g_JamesPoints[i]) maxy = g_JamesPoints[i+1];

				}
				float w = maxx - minx;
				float h = maxy - miny;

				memImageDisplay.Create((maxx - minx)/displayScale+100, (maxy-miny)/displayScale+100, 24);
				CDC memdc;
				memdc.Attach(memImageDisplay.GetDC());
				memdc.FillSolidRect(CRect(0, 0, (maxx - minx)/displayScale+100, (maxy-miny)/displayScale+100), RGB(255,255,255));

				CPen p(PS_SOLID, 1, RGB(255,0,0));
				memdc.SelectObject(p);
				for ( int i=0 ;i < 36-2; i+=2)
				{
					memdc.MoveTo((g_JamesPoints[0]-minx+50)/displayScale, (g_JamesPoints[1]-miny)/displayScale);
        			memdc.LineTo((g_JamesPoints[i]-minx+50)/displayScale, (g_JamesPoints[i+1]-miny)/displayScale);


				}
					
					memdc.Detach();
			memImageDisplay.ReleaseDC();

			memImageDisplay.Draw(dc.GetSafeHdc(), 0, 0);

			}
  	
		if(m_imageReference.IsValidImage())
		{
			CImage memImageDisplay;
			memImageDisplay.Create(m_imageReference.Width()/m_displayScale, m_imageReference.Height()/m_displayScale, 24);
			CDC memdc;
			memdc.Attach(memImageDisplay.GetDC());

			memdc.FillSolidRect(CRect(0, 0, m_imageReference.Width()/m_displayScale, m_imageReference.Height()/m_displayScale), RGB(255,255,255));

			m_imageReference.Draw(memdc.GetSafeHdc(), CRect(0, 0, m_imageReference.Width()/m_displayScale, m_imageReference.Height()/m_displayScale));


			for (int i = 0 ;i < m_referencelines.size();i++)
			{
				for( int j =0 ; j < m_referencelines[i].size()-1;j++)
				{
					CPoint temp = m_referencelines[i][j];
					CPoint temp1 = m_referencelines[i][j+1];
					
					CPen p(PS_SOLID, 1, RGB(255,0,0));
					memdc.SelectObject(p);

					
					

					memdc.MoveTo(temp.x/m_displayScale, temp.y/m_displayScale);
					memdc.LineTo(temp1.x/m_displayScale, temp1.y/m_displayScale);

				}
			}

			memdc.Detach();
			memImageDisplay.ReleaseDC();

			memImageDisplay.Draw(dc.GetSafeHdc(), 0, 0);
		}
		
		if (m_imageLensROI.IsValidImage())
		{
			CImage memImageDisplay;
			memImageDisplay.Create(m_imageLensROI.Width()/m_displayScale, m_imageLensROI.Height()/m_displayScale, 24);
			CDC memdc;
			memdc.Attach(memImageDisplay.GetDC());

			memdc.FillSolidRect(CRect(0, 0, m_imageLensROI.Width()/m_displayScale, m_imageLensROI.Height()/m_displayScale), RGB(255,255,255));

			if (m_btnShowLensROIControl.GetCheck() == BST_CHECKED)
				m_imageLensROI.Draw(memdc.GetSafeHdc(), CRect(0, 0, m_imageLensROI.Width()/m_displayScale, m_imageLensROI.Height()/m_displayScale));

			CPen p(PS_SOLID, 1, RGB(255,0,0));
			memdc.SelectObject(p);

			if (m_objectImageAnalyzer != NULL)
			{
				if (m_isScanOriginSelected && m_objectImageAnalyzer->m_isSet)
				{
					C3DPoint* indexC3DPoint = m_startC3DPoint->m_next;
					
					while (indexC3DPoint != NULL)
					{
//						memdc.MoveTo((m_pointDisplayOrigin.x/m_displayScale), (m_pointDisplayOrigin.y/m_displayScale));
//						memdc.LineTo(indexC3DPoint->x/m_displayScale, indexC3DPoint->y/m_displayScale);

						for (int i=0; i<50; i++)
						{
							if (indexC3DPoint != NULL)
								indexC3DPoint = indexC3DPoint->m_next;
						}
					}

					indexC3DPoint = m_startC3DPoint->m_next;
					C3DPoint* indexC3DPoint2 = NULL; 
					
					while (indexC3DPoint != NULL)
					{
						if (indexC3DPoint2 != NULL)
						{
							memdc.MoveTo(indexC3DPoint2->x/m_displayScale, indexC3DPoint2->y/m_displayScale);
							memdc.LineTo(indexC3DPoint->x/m_displayScale, indexC3DPoint->y/m_displayScale);
						}

						indexC3DPoint2 = indexC3DPoint;

						if (indexC3DPoint2->m_next == NULL)
						{
							memdc.MoveTo(indexC3DPoint2->x/m_displayScale, indexC3DPoint2->y/m_displayScale);
							memdc.LineTo(m_startC3DPoint->m_next->x/m_displayScale, m_startC3DPoint->m_next->y/m_displayScale);
						}

						indexC3DPoint = indexC3DPoint->m_next;
					}
				}
			}

			int offX = (m_sizeMaxExtents.cx + m_sizeMinExtents.cx)/2;
			int offY = (m_sizeMaxExtents.cy + m_sizeMinExtents.cy)/2;

//			m_cloudJimData2.DrawPointsWithSkip(&memdc, CSize(m_displayScale, m_displayScale), CPoint(offX/m_displayScale,offY/m_displayScale), 0);

			if (m_btnShowEdge1.GetCheck() == BST_CHECKED)
				m_cloudTraceEdge[0].Draw(&memdc, CSize(m_displayScale,m_displayScale), CPoint(0,0), &(m_arrayLensBox[0]));

			if (m_btnShowEdge2.GetCheck() == BST_CHECKED)
			{
			m_cloudJimData.DrawPointsWithSkip(&memdc, (float)m_scaleJimData, m_centerJimData, 0);
			m_edgeJimData.DrawPointsWithSkip(&memdc, (float)m_scaleJimData, m_centerJimData, 0, 5);
//				m_cloudTraceEdge[1].Draw(&memdc, CSize(m_displayScale,m_displayScale), CPoint(0,0));
			}

			if (m_btnShowEdge3.GetCheck() == BST_CHECKED)
				m_cloudTraceEdge[2].Draw(&memdc, CSize(m_displayScale,m_displayScale), CPoint(0,0), &(m_arrayLensBox[1]));

			CPoint pp = m_centerLaserData;
			pp.Offset(-800, -400);

			if (m_btnShowEdge4.GetCheck() == BST_CHECKED)
				m_cloudLaserLine.DrawPointsWithSkip(&memdc, (float)m_scaleLaserData*0.14f, pp, 0);

			if (m_btnShowEdge5.GetCheck() == BST_CHECKED)
				m_cloudTraceEdge[4].Draw(&memdc, CSize(m_displayScale,m_displayScale), CPoint(0,0));

			CPen p2(PS_SOLID, 1, RGB(0,0,255));
			memdc.SelectObject(p2);

			CString s;
			
			CPoint centerLensBoxLeft = m_arrayLensBox[0].GetGC();
			memdc.FillSolidRect(m_arrayLensBox[0].m_rectLensBox.left/m_displayScale, centerLensBoxLeft.y/m_displayScale, m_arrayLensBox[0].GetAdimension()/m_displayScale, 2, RGB(0,255,0));
			memdc.FillSolidRect(centerLensBoxLeft.x/m_displayScale, m_arrayLensBox[0].m_rectLensBox.top/m_displayScale, 2, m_arrayLensBox[0].GetBdimension()/m_displayScale, RGB(0,255,0));
			memdc.FillSolidRect(centerLensBoxLeft.x/m_displayScale-2, centerLensBoxLeft.y/m_displayScale-2, 5,5, RGB(0,0,255));
			memdc.MoveTo(centerLensBoxLeft.x/m_displayScale, centerLensBoxLeft.y/m_displayScale);
			memdc.LineTo(m_arrayLensBox[0].m_locationED.x/m_displayScale, m_arrayLensBox[0].m_locationED.y/m_displayScale);
			s.Format("%d", m_arrayLensBox[0].GetAdimension());
			m_editAleft.SetWindowText(s);
			s.Format("%d", m_arrayLensBox[0].GetBdimension());
			m_editBleft.SetWindowText(s);
			s.Format("%.2f", m_arrayLensBox[0].GetED());
			m_editEDleft.SetWindowText(s);


			// QL
			
			CPen p1(PS_SOLID, 1, RGB(0,255,255));
			memdc.SelectObject(p1);
			
			if (m_objectImageAnalyzer != NULL)
			{
				if (m_isScanOriginSelected && m_objectImageAnalyzer->m_isSet)
				{
					
					int count = 0;
					int step = 0;
					
					C3DPoint* indexC3DPoint =m_cloudTraceEdge[0].m_start3DPoint->m_next;
					while (indexC3DPoint != NULL)
					{
					   count++;
					   indexC3DPoint = indexC3DPoint->m_next;
					}

					// want to get 10 points 
				  step = (count)/50;

				  indexC3DPoint =m_cloudTraceEdge[0].m_start3DPoint->m_next;

					int i = 0;
					int j =0;
					CFile cfile;
				   cfile.Open(_T("50_points.dat"), CFile::modeCreate | 
				   CFile::modeReadWrite);
				 char pbufWrite[100];
					
					while (indexC3DPoint != NULL)
					{
						    if (i%step == 0 && i < count-step-1)
							{
							    memdc.MoveTo(centerLensBoxLeft.x/m_displayScale, centerLensBoxLeft.y/m_displayScale);
							    memdc.LineTo(indexC3DPoint->x/m_displayScale, indexC3DPoint->y/m_displayScale);
								memdc.Ellipse(indexC3DPoint->x/m_displayScale-5, indexC3DPoint->y/m_displayScale-5,indexC3DPoint->x/m_displayScale+5, indexC3DPoint->y/m_displayScale+5);
								j++;
								sprintf(pbufWrite,"%d %4.2f %4.2f\n",j,indexC3DPoint->x,indexC3DPoint->y);
								cfile.Write(pbufWrite, strlen(pbufWrite));

							}


							indexC3DPoint = indexC3DPoint->m_next;
							i++;
					}
				
				cfile.Flush();

			
				}
			
			
			}
			
			


			CPoint centerLensBoxRight = m_arrayLensBox[1].GetGC();
			memdc.FillSolidRect(m_arrayLensBox[1].m_rectLensBox.left/m_displayScale, centerLensBoxRight.y/m_displayScale, m_arrayLensBox[1].GetAdimension()/m_displayScale, 2, RGB(0,255,0));
			memdc.FillSolidRect(centerLensBoxRight.x/m_displayScale, m_arrayLensBox[1].m_rectLensBox.top/m_displayScale, 2, m_arrayLensBox[1].GetBdimension()/m_displayScale, RGB(0,255,0));
			memdc.FillSolidRect(centerLensBoxRight.x/m_displayScale-2, centerLensBoxRight.y/m_displayScale-2, 5,5, RGB(0,0,255));
			memdc.MoveTo(centerLensBoxRight.x/m_displayScale, centerLensBoxRight.y/m_displayScale);
			memdc.LineTo(m_arrayLensBox[1].m_locationED.x/m_displayScale, m_arrayLensBox[1].m_locationED.y/m_displayScale);
			s.Format("%d", m_arrayLensBox[1].GetAdimension());
			m_editAright.SetWindowText(s);
			s.Format("%d", m_arrayLensBox[1].GetBdimension());
			m_editBright.SetWindowText(s);
			s.Format("%.2f", m_arrayLensBox[1].GetED());
			m_editEDright.SetWindowText(s);

			if (m_arrayLensBox[0].GetAdimension() > 0 && m_arrayLensBox[1].GetAdimension() > 0)
			{
				memdc.FillSolidRect(m_arrayLensBox[0].m_rectLensBox.right/m_displayScale, (m_arrayLensBox[0].m_rectLensBox.bottom/m_displayScale)+50, m_arrayLensBox[1].m_rectLensBox.left/m_displayScale-m_arrayLensBox[0].m_rectLensBox.right/m_displayScale, 2, RGB(0,0,255));
				memdc.FillSolidRect(centerLensBoxLeft.x/m_displayScale, (m_arrayLensBox[0].m_rectLensBox.bottom/m_displayScale)+75, centerLensBoxRight.x/m_displayScale-centerLensBoxLeft.x/m_displayScale, 2, RGB(0,0,255));
				s.Format("%d", m_arrayLensBox[0].GetDBL(&(m_arrayLensBox[1])));
				m_editDBL.SetWindowText(s);
				s.Format("%d", m_arrayLensBox[0].GetDBC(&(m_arrayLensBox[1])));
				m_editDBC.SetWindowText(s);
			}

			memdc.Detach();
			memImageDisplay.ReleaseDC();

			memImageDisplay.Draw(dc.GetSafeHdc(), 0, 0);
		}

		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cedge3DStudioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CPoint Cedge3DStudioDlg::LocateEdgePos(int p_startXpos, int p_startYpos, int p_dirX, int p_dirY)
{
	CPoint pointScanPos = CPoint(p_startXpos, p_startYpos);

	while (pointScanPos.x > 0 && pointScanPos.y > 0 && pointScanPos.x < g_sizeImageAnalyzer.cx && pointScanPos.y < g_sizeImageAnalyzer.cy && !m_objectImageAnalyzer->matrixThreshold[pointScanPos.x][pointScanPos.y])
	{
		pointScanPos.x += p_dirX;
		pointScanPos.y += p_dirY;
	}

	return pointScanPos;
}


void Cedge3DStudioDlg::TestForMinMax(CPoint p_pointNewEdge)
{
	if (p_pointNewEdge.x > m_sizeMaxExtents.cx)
		m_sizeMaxExtents.cx = p_pointNewEdge.x;
	if (p_pointNewEdge.y > m_sizeMaxExtents.cy)
		m_sizeMaxExtents.cy = p_pointNewEdge.y;

	if (p_pointNewEdge.x < m_sizeMinExtents.cx)
		m_sizeMinExtents.cx = p_pointNewEdge.x;
	if (p_pointNewEdge.y < m_sizeMinExtents.cy)
		m_sizeMinExtents.cy = p_pointNewEdge.y;
}


bool Cedge3DStudioDlg::MatchPointPatternForNextMove(CPoint p_pointPatternMatch)
{
	if (m_objectImageAnalyzer->matrixThreshold[p_pointPatternMatch.x-1][p_pointPatternMatch.y-1] && m_objectImageAnalyzer->matrixThreshold[p_pointPatternMatch.x-1][p_pointPatternMatch.y] && m_objectImageAnalyzer->matrixThreshold[p_pointPatternMatch.x-1][p_pointPatternMatch.y+1])
		return true;

	return false;
}


void Cedge3DStudioDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		if (m_imageLensROI.IsValidImage())
		{
			KillTimer(1);
			m_objectImageAnalyzer->Set(&m_imageLensROI);
			m_sizeMaxExtents = CSize(0, 0);
			m_sizeMinExtents = CSize(m_objectImageAnalyzer->m_sizeWidth, m_objectImageAnalyzer->m_sizeHeight);
		}
	}
	int hT = 10;

	if(nIDEvent == 11 ) // QL reference to detect block
	{
		KillTimer(11);
		int w = m_imageReference.Width();

		int h = m_imageReference.Height();


//		int indexByteArray = p_imageToAnalyze->GetPitch() * (m_sizeHeight-1);
		int indexByteArray = 0;
		BYTE* b = (BYTE*)m_imageReference.GetMemStart();

		BYTE reference[3];
		reference[0] = b[(m_pointScanOrigin.x + m_pointScanOrigin.y*w)*3]; 
		reference[1] = b[(m_pointScanOrigin.x + m_pointScanOrigin.y*w)*3+1]; 
		reference[2] = b[(m_pointScanOrigin.x + m_pointScanOrigin.y*w)*3+2]; 
		std::vector<CPoint> hline;
		for ( int x=m_pointScanOrigin.x;x < w; x++)
		{
			BYTE p[3];
			p[0] = b[(x + m_pointScanOrigin.y*w)*3]; 
			p[1] = b[(x + m_pointScanOrigin.y*w)*3+1]; 
			p[2] = b[(x + m_pointScanOrigin.y*w)*3+2]; 

			if( abs(p[0] - reference[0]) < hT && abs(p[1] - reference[1]) < hT && abs(p[2] - reference[2]) < hT) 
			{
				CPoint temp;
				temp.x = x;
				temp.y = m_pointScanOrigin.y;
				hline.push_back(temp);
			}
			else
				break;
		}
		int max = hline[hline.size()-1].x;

		 
		for ( int x=m_pointScanOrigin.x;x > 0; x--)
		{
			BYTE p[3];
			p[0] = b[(x + m_pointScanOrigin.y*w)*3]; 
			p[1] = b[(x + m_pointScanOrigin.y*w)*3+1]; 
			p[2] = b[(x + m_pointScanOrigin.y*w)*3+2]; 

			if( abs(p[0] - reference[0]) < 10 && abs(p[1] - reference[1]) < 10 && abs(p[2] - reference[2]) < 10) 
			{
				CPoint temp;
				temp.x = x;
				temp.y = m_pointScanOrigin.y;
				hline.push_back(temp);
			}
			else
				break;
		}

		m_referencelines.push_back(hline);

		int min = hline[hline.size()-1].x;

		CString msg;
		msg.Format("horizontal l:%d r:%d is:%d",min,max,max-min);

		CListBox* edit = (CListBox*) GetDlgItem(IDC_EDIT_REF);

		edit->AddString(msg);

		UpdateData(false);



		// vertical 
		/*
		std::vector<CPoint> vline;
		for ( int y=m_pointScanOrigin.y;y < h; y++)
		{
			BYTE p[3];
			p[0] = b[(m_pointScanOrigin.x + y*w)*3]; 
			p[1] = b[(m_pointScanOrigin.x + y*w)*3+1]; 
			p[2] = b[(m_pointScanOrigin.x + y*w)*3+2]; 

			if( abs(p[0] - reference[0]) < 10 && abs(p[1] - reference[1]) < 10 && abs(p[2] - reference[2]) < 10) 
			{
				CPoint temp;
				temp.x = m_pointScanOrigin.x;
				temp.y = y- (81-27);
				vline.push_back(temp);
			}
			else
				break;
		}
		max = vline[vline.size()-1].y;


		for ( int y=m_pointScanOrigin.y;y > 0; y--)
		{
			BYTE p[3];
			p[0] = b[(m_pointScanOrigin.x + y*w)*3]; 
			p[1] = b[(m_pointScanOrigin.x + y*w)*3+1]; 
			p[2] = b[(m_pointScanOrigin.x + y*w)*3+2]; 

			if( abs(p[0] - reference[0]) < 10 && abs(p[1] - reference[1]) < 10 && abs(p[2] - reference[2]) < 10) 
			{
				CPoint temp;
				temp.x = m_pointScanOrigin.x;
				temp.y = y - (81-27);
				vline.push_back(temp);
			}
			else
				break;
		}

		m_referencelines.push_back(vline);

		min = vline[vline.size()-1].y;


		msg.Format("vertical low:%d up:%d is:%d",min,max,max-min);

		edit = (CListBox*) GetDlgItem(IDC_EDIT_REF);

		edit->AddString(msg);*/

		UpdateData(false);



		Invalidate(false);

	
	}

	if (m_objectImageAnalyzer != NULL)
	{
		if (m_isScanOriginSelected && m_objectImageAnalyzer->m_isSet)
		{
			if (nIDEvent == 10)
			{
				KillTimer(10);
				{
					int count=0;
					int mark=0;

					m_cloudTraceEdge[m_indexTraceEdge].Reset();
					CPoint pointCurPatternLocation = m_pointScanOrigin;

					while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x+1][pointCurPatternLocation.y])
					{
						while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x+1][pointCurPatternLocation.y])
							pointCurPatternLocation.x++;
					
						m_cloudTraceEdge[m_indexTraceEdge].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						if (count>=mark)
						{
							count=0;
							m_cloudTraceEdge[4].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						}
						count++;

						while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x-1][pointCurPatternLocation.y] && pointCurPatternLocation.x > 10)
							pointCurPatternLocation.x--;
					
						if (pointCurPatternLocation.x > 10)
						{
							m_cloudTraceEdge[m_indexTraceEdge].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
							if (count>=mark)
							{
								count=0;
								m_cloudTraceEdge[4].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
							}
							count++;
						}

						pointCurPatternLocation.x = m_pointScanOrigin.x;
						pointCurPatternLocation.y--;
					}

					mark = 0;
					pointCurPatternLocation.y = m_pointScanOrigin.y+1;

					while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x+1][pointCurPatternLocation.y])
					{
						while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x+1][pointCurPatternLocation.y])
							pointCurPatternLocation.x++;
					
						m_cloudTraceEdge[m_indexTraceEdge].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						if (count>=mark)
						{
							count=0;
							m_cloudTraceEdge[4].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						}
						count++;

						while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x-1][pointCurPatternLocation.y])
							pointCurPatternLocation.x--;
					
						m_cloudTraceEdge[m_indexTraceEdge].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						if (count>=mark)
						{
							count=0;
							m_cloudTraceEdge[4].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						}
						count++;

						pointCurPatternLocation.x = m_pointScanOrigin.x;
						pointCurPatternLocation.y++;
					}

					mark = 0;
					pointCurPatternLocation = m_pointScanOrigin;

					while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x][pointCurPatternLocation.y+1])
					{
						while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x][pointCurPatternLocation.y+1])
							pointCurPatternLocation.y++;
					
						m_cloudTraceEdge[m_indexTraceEdge].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						if (count>=mark)
						{
							count=0;
							m_cloudTraceEdge[4].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						}
						count++;

						while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x][pointCurPatternLocation.y-1])
							pointCurPatternLocation.y--;
					
						m_cloudTraceEdge[m_indexTraceEdge].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						if (count>=mark)
						{
							count=0;
							m_cloudTraceEdge[4].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						}
						count++;

						pointCurPatternLocation.y = m_pointScanOrigin.y;
						pointCurPatternLocation.x--;
					}

					mark = 0;
					pointCurPatternLocation.x = m_pointScanOrigin.x+1;

					while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x][pointCurPatternLocation.y+1])
					{
						while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x][pointCurPatternLocation.y+1])
							pointCurPatternLocation.y++;
					
						m_cloudTraceEdge[m_indexTraceEdge].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						if (count>=mark)
						{
							count=0;
							m_cloudTraceEdge[4].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						}
						count++;

						while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x][pointCurPatternLocation.y-1])
							pointCurPatternLocation.y--;
					
						m_cloudTraceEdge[m_indexTraceEdge].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						if (count>=mark)
						{
							count=0;
							m_cloudTraceEdge[4].CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 1.0f, m_colorTraceEdge);
						}
						count++;

						pointCurPatternLocation.y = m_pointScanOrigin.y;
						pointCurPatternLocation.x++;
					}
				}

				m_arrayLensBox[m_indexLensBox].Set(m_cloudTraceEdge[m_indexTraceEdge].m_sizeMinExtents.cx, m_cloudTraceEdge[m_indexTraceEdge].m_sizeMinExtents.cy, m_cloudTraceEdge[m_indexTraceEdge].m_sizeMaxExtents.cx, m_cloudTraceEdge[m_indexTraceEdge].m_sizeMaxExtents.cy);

				// QL arrage the first lens points to clockwise 
				CProcessJHD clockwise;
				BeginWaitCursor();
				clockwise.Arrange3DPointsClockWise(m_cloudTraceEdge[m_indexTraceEdge].m_start3DPoint,m_arrayLensBox[m_indexTraceEdge].GetGC());
				EndWaitCursor();

/*				{
					CPoint pointCurPatternLocation = m_pointScanOrigin;

					while (!m_objectImageAnalyzer->matrixThreshold[pointCurPatternLocation.x][pointCurPatternLocation.y])
						pointCurPatternLocation.x++;

					CEdgePattern* curEdgePattern;
					m_cloudTraceEdge.Reset();
					bool isPaternFound = true;
					bool t[3][3];

					for (int ttt=0; ttt<6000; ttt++)
					{
						if (isPaternFound)
						{
							m_cloudTraceEdge.CreatePoint((float)pointCurPatternLocation.x, (float)pointCurPatternLocation.y, 0.0f, RGB(0, 255, 255));
						}
						else
						{
							int s = 13;
						}

						isPaternFound = false;

						int curPatternIndex = 0;
						CPoint nextMoveDirection = CPoint(0,0);

						while (!isPaternFound && curPatternIndex < m_objectImageAnalyzer->m_arrayEdgePatterns.GetCount())
						{
							curEdgePattern = (CEdgePattern*)m_objectImageAnalyzer->m_arrayEdgePatterns.GetAt(curPatternIndex);
							bool isaPatternmatch = true;

							for (int y=0; y<3; y++)
							{
								for (int x=0; x<3; x++)
								{
									if (x != 1 && y != 1)
									{
										t[x][y] = m_objectImageAnalyzer->matrixThreshold[(pointCurPatternLocation.x-1)+x][(pointCurPatternLocation.y-1)+y];

										if (m_objectImageAnalyzer->matrixThreshold[(pointCurPatternLocation.x-1)+x][(pointCurPatternLocation.y-1)+y] != curEdgePattern->m_matrixPattern[x][y])
											isaPatternmatch = false;
									}
								}
							}
							
							if (isaPatternmatch)
							{
								isPaternFound = true;
								nextMoveDirection = curEdgePattern->m_directionToMove;
							}

							curPatternIndex++;
						}

						pointCurPatternLocation.Offset(nextMoveDirection.x, nextMoveDirection.y);
					}
				}*/

				Invalidate(false);
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void Cedge3DStudioDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	
	m_pointScanOrigin.x = point.x*m_displayScale;
	m_pointScanOrigin.y = point.y*m_displayScale;

	if (m_pointDisplayOrigin.x == -1)
		m_pointDisplayOrigin = m_pointScanOrigin;

	if(m_imageReference.IsValidImage())
	{
			SetTimer(11, 100, NULL);
			Invalidate(false);
			CDialogEx::OnLButtonUp(nFlags, point);
			return; 
	}
	


	C3DPoint* indexC3DPoint = m_startC3DPoint->m_next;
	C3DPoint* removeC3DPoint;
/*	m_countLensPoints = 0;
	m_sizeMaxExtents = CSize(0, 0);
	m_sizeMinExtents = CSize(m_objectImageAnalyzer->m_sizeWidth, m_objectImageAnalyzer->m_sizeHeight);

					
	while (indexC3DPoint != NULL)
	{
		removeC3DPoint = indexC3DPoint;
		indexC3DPoint = indexC3DPoint->m_next;
		delete removeC3DPoint;
	}

	m_startC3DPoint->m_next = NULL;
	m_curC3DPoint = m_startC3DPoint;
*/
	m_isScanOriginSelected = true;

	SetTimer(10, 100, NULL);

	Invalidate(false);

	CDialogEx::OnLButtonUp(nFlags, point);
}


void Cedge3DStudioDlg::OnBnClickedCheckShowLensroi()
{
	Invalidate(false);
}


void Cedge3DStudioDlg::OnBnClickedButtonSavexydata()
{
	m_edgeJimData.SaveJimData();

	return;		
	
	int offX = (m_sizeMaxExtents.cx + m_sizeMinExtents.cx)/2;
	int offY = (m_sizeMaxExtents.cy + m_sizeMinExtents.cy)/2;
					
	CString s;
	CStdioFile fx;
	fx.Open("c:\\data\\test2_left_x.txt", CFile::modeCreate | CFile::modeWrite);
	CStdioFile fy;
	fy.Open("c:\\data\\test2_left_y.txt", CFile::modeCreate | CFile::modeWrite);

	C3DPoint* indexC3DPoint = m_startC3DPoint->m_next;

	// 1572 pixel per 30mm = 0.0191 mm/pixel * 100 to match format on right lens
	// 1922 pixel per 30mm = 0.0156 mm/pixel * 100 to match format on left lens
	// 1605 pixel per 30mm = 0.0187 mm/pixel * 100 to match format on left lens
	// 2248 pixel per 30mm = 0.0133 mm/pixel * 100 to match format on left lens
					
	while (indexC3DPoint != NULL)
	{
		s.Format("%.3f", -1.0f * (float)(indexC3DPoint->x - offX) * 1.33f);
		s += '\n';
		fx.WriteString(s);

		s.Format("%.3f", -1.0f * (float)(indexC3DPoint->y - offY) * 1.33f);
		s += '\n';
		fy.WriteString(s);

		indexC3DPoint = indexC3DPoint->m_next;
	}

	fx.Close();
	fy.Close();
}


void Cedge3DStudioDlg::OnBnClickedButtonLoadjimdata()
{
	CFileDialog loadFileDlg(true);

	if (loadFileDlg.DoModal() == IDOK)
	{
		float x;
		float y;
		float z;

		float aspect = 1.3f;

		float lastY = 0.0f;
		int index = -1;
		float ind[500];
		for (int i=0; i<500; i++)
			ind[i] = -1.0f;
		
		int e;
		int ID = 0;
		CString s;
		CStdioFile fx;
		fx.Open(loadFileDlg.GetPathName(), CFile::modeRead);

		fx.ReadString(s);	// burn data count

		while (!s.IsEmpty())
		{
			fx.ReadString(s);
			sscanf_s(s, "%f %f %f %d", &x, &y, &z, &e);
			
			if (e == 0)
				m_cloudJimData.CreatePoint(x*0.3f*0.4f, y*aspect, z, RGB(255,0,255), ID);
			if (e == 1)
				m_cloudJimData.CreatePoint(x*0.3f*0.4f, y*aspect, z, RGB(0,255,255), ID);
			if (e == 2)
				m_cloudJimData.CreatePoint(x*0.3f*0.4f, y*aspect, z, RGB(0,0,255));

			if (lastY != y)
			{
				index++;
				lastY = y;
				ind[index] = y;
			}

			if (e == 0 || e == 1)
			{
				m_cloudJimData.m_imageData.SetPixel((int)x, index, RGB(255,0,255));
				m_edgeJimData.m_imageData.SetPixel((int)x, index, ID);
				ID++;

				{

//					m_edgeJimData.CreatePoint(x*0.3f*0.4f, y*0.3f, z, RGB(255,255,0));
				}
			}
		}

		int l = (m_cloudJimData.m_sizeMinExtents.cx/0.3f/0.4f);
		int h = (m_cloudJimData.m_sizeMaxExtents.cx/0.3f/0.4f);
		l = (h+l)/2;
		int ss = h-((h-l)/2);
		int xx = ss;

		for (int yy=9; yy<60; yy++)
		{
			xx = ss;

			while (m_cloudJimData.m_imageData.GetPixel(xx, yy) != RGB(255,0,255) && xx > l)
				xx--;

			if (xx > l)
			{
				int ID = m_edgeJimData.m_imageData.GetPixel(xx, yy);
				m_edgeJimData.CreatePoint(xx*0.3f*0.4f, ind[yy]*aspect, m_cloudJimData.GetID_Z(ID), RGB(255,255,0));
				
//				m_edgeJimData.CreatePoint(ss*0.3f*0.4f, ind[yy]*0.3f, 1.0f, RGB(255,0,0));
//				m_edgeJimData.CreatePoint(l*0.3f*0.4f, ind[yy]*0.3f, 1.0f, RGB(0,255,0));
//				m_edgeJimData.CreatePoint(h*0.3f*0.4f, ind[yy]*0.3f, 1.0f, RGB(0,0,255));
			}
		}

		for (int yy=59; yy>8; yy--)
		{
			xx = ss;

			while (m_cloudJimData.m_imageData.GetPixel(xx, yy) != RGB(255,0,255) && xx < h)
				xx++;

			if (xx < h)
			{
				int ID = m_edgeJimData.m_imageData.GetPixel(xx, yy);
				m_edgeJimData.CreatePoint(xx*0.3f*0.4f, ind[yy]*aspect, m_cloudJimData.GetID_Z(ID), RGB(255,255,0));
				
//				m_edgeJimData.CreatePoint(ss*0.3f*0.4f, ind[yy]*0.3f, 1.0f, RGB(255,0,0));
//				m_edgeJimData.CreatePoint(l*0.3f*0.4f, ind[yy]*0.3f, 1.0f, RGB(0,255,0));
//				m_edgeJimData.CreatePoint(h*0.3f*0.4f, ind[yy]*0.3f, 1.0f, RGB(0,0,255));
			}
		}


		m_cloudJimData.NormalizeToOrigin();
		m_cloudJimData.FlipAroundY();

		m_edgeJimData.m_sizeMaxExtents = m_cloudJimData.m_sizeMaxExtents;
		m_edgeJimData.m_sizeMinExtents = m_cloudJimData.m_sizeMinExtents;

		m_edgeJimData.NormalizeToOrigin();
		m_edgeJimData.FlipAroundY();
//		m_cloutoPixels(0.05f);

		fx.Close();
	}
}


void Cedge3DStudioDlg::OnBnClickedButtonLoadimage()
{
	CFileDialog dialogFileOpen(true);
	if (dialogFileOpen.DoModal() != IDOK)
		return;

	m_imageLensROI.Destroy();

	wchar_t wszString[512];
	MultiByteToWideChar(CP_ACP, 0, dialogFileOpen.GetPathName(), -1,  wszString,  256);
	
	if (!m_imageLensROI.Load(wszString))
	{
		AfxMessageBox("Could not load Lens ROI Image!");
	}

	filename = dialogFileOpen.GetPathName();

	Invalidate(true);
}


void Cedge3DStudioDlg::OnBnClickedButton2()
{
	m_imageLensROI.Destroy();

	wchar_t wszString[512];
	MultiByteToWideChar(CP_ACP, 0, filename, -1,  wszString,  256);
	
	if (!m_imageLensROI.Load(wszString))
	{
		AfxMessageBox("Could not load Lens ROI Image!");
		return;
	}

	CString s;

	m_editGamma.GetWindowText(s);
	
	if (s != "-1")
		m_imageLensROI.ApplyEffect(FCEffectGamma(atoi(s)));

//	m_imageLensROI.ApplyEffect(FCEffectReduceNoise(5, 0.4));

	m_editThres.GetWindowText(s);
	
	if (s != "-1")
		m_imageLensROI.ApplyEffect(FCEffectThreshold(atoi(s)));
	
//	if (s != "-1")
//		m_imageLensROI.ApplyEffect(FCEffectThreshold(atoi(s)));
	
	if (m_imageLensROI.IsValidImage())
	{
		m_objectImageAnalyzer->Set(&m_imageLensROI);
		m_sizeMaxExtents = CSize(0, 0);
		m_sizeMinExtents = CSize(m_objectImageAnalyzer->m_sizeWidth, m_objectImageAnalyzer->m_sizeHeight);
	}

	Invalidate(true);
}


void Cedge3DStudioDlg::OnBnClickedButton3()
{
	if (m_indexTraceEdge >= 13)
		return;

	m_indexTraceEdge++;

	if (m_indexTraceEdge == 1)
	{
		m_colorTraceEdge = RGB(0,255,0);
		m_btnShowEdge2.SetCheck(BST_CHECKED);
	}
	else if (m_indexTraceEdge == 2)
	{
		m_colorTraceEdge = RGB(0,0,255);
		m_btnShowEdge3.SetCheck(BST_CHECKED);
	}
	else if (m_indexTraceEdge == 3)
	{
		m_colorTraceEdge = RGB(255,255,0);
		m_btnShowEdge4.SetCheck(BST_CHECKED);
	}
	else if (m_indexTraceEdge == 4)
	{
		m_colorTraceEdge = RGB(0,255,255);
		m_btnShowEdge5.SetCheck(BST_CHECKED);
	}
	else if (m_indexTraceEdge == 5)
	{
		m_colorTraceEdge = RGB(255,0,255);
	}
}


void Cedge3DStudioDlg::OnBnClickedCheckShowEdge1()
{
	Invalidate(false);
}


void Cedge3DStudioDlg::OnBnClickedCheckShowEdge2()
{
	Invalidate(false);
}


void Cedge3DStudioDlg::OnBnClickedCheckShowEdge3()
{
	Invalidate(false);
}


void Cedge3DStudioDlg::OnBnClickedCheckShowEdge4()
{
	Invalidate(false);
}


void Cedge3DStudioDlg::OnBnClickedCheckShowEdge5()
{
	Invalidate(false);
}


void Cedge3DStudioDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
if (m_btnShowEdge4.GetCheck() != BST_CHECKED)
	m_centerJimData = point;
else
	m_centerLaserData = point;

	Invalidate(false);

	CDialogEx::OnRButtonUp(nFlags, point);
}


BOOL Cedge3DStudioDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
if (m_btnShowEdge4.GetCheck() != BST_CHECKED)
{
	if (nFlags == 0)
	{
		if (zDelta > 0)
		{
			m_scaleJimData += 0.01f;
		}
		else
		{
			m_scaleJimData -= 0.01f;
		}
	}
	else if (nFlags == 4)
	{
		if (zDelta > 0)
		{
			m_cloudJimData.Rotate(0.1f);
			m_edgeJimData.Rotate(0.1f);
		}
		else
		{
			m_cloudJimData.Rotate(-0.1f);
			m_edgeJimData.Rotate(-0.1f);
		}
	}
}
else
{
	if (nFlags == 0)
	{
		if (zDelta > 0)
		{
			m_scaleLaserData += 0.05f;
		}
		else
		{
			m_scaleLaserData -= 0.05f;
		}
	}
	else if (nFlags == 4)
	{
		if (zDelta > 0)
		{
			m_cloudLaserLine.Rotate(0.3f);
		}
		else
		{
			m_cloudLaserLine.Rotate(-0.3f);
		}
	}
}

	Invalidate(false);


	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void Cedge3DStudioDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags == 2)
	{
if (m_btnShowEdge4.GetCheck() != BST_CHECKED)
	m_centerJimData = point;
else
	m_centerLaserData = point;
		Invalidate(false);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void Cedge3DStudioDlg::OnBnClickedButtonSavexydata3()
{
	m_cloudTraceEdge[4].SaveTedData();
}


void Cedge3DStudioDlg::OnBnClickedButtonLoadlaserline()
{
	CLaserLinecannerDialog LaserLinecannerDialog;
	LaserLinecannerDialog.m_cloudLaserLine = &m_cloudLaserLine;
	LaserLinecannerDialog.DoModal();
}


void Cedge3DStudioDlg::OnBnClickedCheck1()
{
	m_indexLensBox = 0;
	m_btnDoLeftLensBox.SetCheck(BST_CHECKED);
	m_btnDoRightLensBox.SetCheck(BST_UNCHECKED);

}


void Cedge3DStudioDlg::OnBnClickedCheck2()
{
	m_indexLensBox = 1;
	m_btnDoLeftLensBox.SetCheck(BST_UNCHECKED);
	m_btnDoRightLensBox.SetCheck(BST_CHECKED);
}


void Cedge3DStudioDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	CFileDialog dialogFileOpen(true);
	if (dialogFileOpen.DoModal() != IDOK)
		return;

	m_imageReference.Destroy();

	wchar_t wszString[512];
	MultiByteToWideChar(CP_ACP, 0, dialogFileOpen.GetPathName(), -1,  wszString,  256);
	
	if (!m_imageReference.Load(wszString))
	{
		AfxMessageBox("Could not load Lens ROI Image!");
	}

	filename = dialogFileOpen.GetPathName();

	Invalidate(true);


}


void Cedge3DStudioDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	if(m_imageReference.IsValidImage())
	{
		m_imageReference.CopyToClipboard();
	}
}


void Cedge3DStudioDlg::OnBnClickedImagesave()
{
	// TODO: Add your control notification handler code here

	// get screen rectangle
 RECT windowRect;
 GetWindowRect(&windowRect);

 // bitmap dimensions
 int bitmap_dx = windowRect.right - windowRect.left;
 int bitmap_dy = windowRect.bottom - windowRect.top;

 // create file
 //std::ofstream file("screen.bmp", std::ios::binary);
 //if(!file) return;


 CFile file;
	if( !file.Open( "screen.bmp", CFile::modeWrite|CFile::modeCreate) )
		return ;

 // save bitmap file headers
 BITMAPFILEHEADER fileHeader;
 BITMAPINFOHEADER infoHeader;

 fileHeader.bfType      = 0x4d42;
 fileHeader.bfSize      = 0;
 fileHeader.bfReserved1 = 0;
 fileHeader.bfReserved2 = 0;
 fileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

 infoHeader.biSize          = sizeof(infoHeader);
 infoHeader.biWidth         = bitmap_dx;
 infoHeader.biHeight        = bitmap_dy;
 infoHeader.biPlanes        = 1;
 infoHeader.biBitCount      = 24;
 infoHeader.biCompression   = BI_RGB;
 infoHeader.biSizeImage     = 0;
 infoHeader.biXPelsPerMeter = 0;
 infoHeader.biYPelsPerMeter = 0;
 infoHeader.biClrUsed       = 0;
 infoHeader.biClrImportant  = 0;

 file.Write((char*)&fileHeader, sizeof(fileHeader));
 file.Write((char*)&infoHeader, sizeof(infoHeader));

 // dibsection information
 BITMAPINFO info;
 info.bmiHeader = infoHeader; 

 // ------------------
 // THE IMPORTANT CODE
 // ------------------
 // create a dibsection and blit the window contents to the bitmap
 //HDC winDC = GetWindowDC();
// HDC memDC = CreateCompatibleDC(winDC);
 
 HDC winDC = GetWindowDC()->GetSafeHdc();
 HDC memDC = CreateCompatibleDC(winDC);

 BYTE* memory = 0;
 HBITMAP bitmap = CreateDIBSection(winDC, &info, DIB_RGB_COLORS, (void**)&memory, 0, 0);
 SelectObject(memDC, bitmap);
 BitBlt(memDC, 0, 0, bitmap_dx, bitmap_dy, winDC, 0, 0, SRCCOPY);
 DeleteDC(memDC);


 // save dibsection data
 int bytes = (((24*bitmap_dx + 31) & (~31))/8)*bitmap_dy;
 file.Write(memory, bytes);

 // HA HA, forgot paste in the DeleteObject lol, happy now ;)?
 DeleteObject(bitmap);
 file.Close();
}


void Cedge3DStudioDlg::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	Invalidate(true);


}


void Cedge3DStudioDlg::OnBnClickedManualDlg()
{
	// TODO: Add your control notification handler code here

	CManualMotionCtlDlg ManualMotionCtlDlg;
	ManualMotionCtlDlg.DoModal();


}


//void Cedge3DStudioDlg::OnBnClickedReadmodbus()
//{
//	// TODO: Add your control notification handler code here
//}


void Cedge3DStudioDlg::OnBnClickedReadmodbus()
{
	// TODO: Add your control notification handler code here
}
