// LaserLinecannerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "edge3D Studio.h"
#include "LaserLinecannerDialog.h"
#include "afxdialogex.h"


using namespace Gdiplus;


C3DCloud g_cloudLaserLine;
// CLaserLinecannerDialog dialog

IMPLEMENT_DYNAMIC(CLaserLinecannerDialog, CDialog)

CLaserLinecannerDialog::CLaserLinecannerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLaserLinecannerDialog::IDD, pParent)
{

}

CLaserLinecannerDialog::~CLaserLinecannerDialog()
{
}

void CLaserLinecannerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLaserLinecannerDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LASERLINESCAN_SCAN, &CLaserLinecannerDialog::OnBnClickedButtonLaserlinescanScan)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_LASERLINESCAN_SAVE, &CLaserLinecannerDialog::OnBnClickedButtonLaserlinescanSave)
	
END_MESSAGE_MAP()


// CLaserLinecannerDialog message handlers


BOOL CLaserLinecannerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CLaserLinecannerDialog::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 13)
	{
		int timespan = 6;
		int threshold = 40;

		CString filename;

		// QL 
		CString zFilename;
		CImage zImage;


		if(m_indexCaptureFile <10)
		{
			filename.Format("C:\\Data\\video-frame\\scene0000%d.jpg", m_indexCaptureFile);
			zFilename.Format("C:\\Data\\video-frame-z\\scene0000%d.jpg", m_indexCaptureFile);

		}
		else if(m_indexCaptureFile <100)
		{
			filename.Format("C:\\Data\\video-frame\\scene000%d.jpg", m_indexCaptureFile);
			zFilename.Format("C:\\Data\\video-frame-z\\scene0000%d.jpg", m_indexCaptureFile);
		}
		else if(m_indexCaptureFile <1000)
		{
			filename.Format("C:\\Data\\video-frame\\scene00%d.jpg", m_indexCaptureFile);
			zFilename.Format("C:\\Data\\video-frame-z\\scene0000%d.jpg", m_indexCaptureFile);
		}

		m_indexCaptureFile++;

//		CFileDialog dialogFileOpen(true);
//		if (dialogFileOpen.DoModal() != IDOK)
//			return;

		m_imageLaserLineCaptureFrame.Destroy();

//		wchar_t wszString[512];
//		MultiByteToWideChar(CP_ACP, 0, filename, -1,  wszString,  256);
	
		m_imageLaserLineCaptureFrame.Load(filename);

		// rotation the image 
		/*Bitmap* gdiPlusBitmap=CImage2Image(&m_imageLaserLineCaptureFrame);
		gdiPlusBitmap->RotateFlip(Rotate90FlipNone);
		HBITMAP hbmp;
		gdiPlusBitmap->GetHBITMAP(Color::White, &hbmp);
		m_imageLaserLineCaptureFrame.Detach();
		m_imageLaserLineCaptureFrame.Attach(hbmp);*/

		zImage.Load(filename);

		float z= 0.0f;
		float k = -1.0/slope;
		
		//for (int i=0; i<timespan; i++)
		//if (0)
		{
			for (int yCheck=0; yCheck<m_imageLaserLineCaptureFrame.GetHeight(); yCheck++)
			
			{
				float xResult = (float(yCheck)-intercept)/slope;

				COLORREF color = m_imageLaserLineCaptureFrame.GetPixel(xResult, yCheck);
				DWORD r = GetRValue(color);
				DWORD g = GetRValue(color);
				DWORD b = GetRValue(color);

				if (r < threshold && g < threshold && b < threshold)
					{
						
						zImage.SetPixel(xResult,yCheck,RGB(255,0,0));

						//m_cloudLaserLine.CreatePoint(timeslice, yCheck, 1.0f, RGB(0,0,255));
						// QL to check z value 
						float d = yCheck - xResult*k;
						int max_indx = 0;
						float yResult; 
						float max_indy =0.0;
						for( int indx = int(xResult)+1; indx < m_imageLaserLineCaptureFrame.GetWidth() ;indx++)
					
								
						{

								yResult =  (float)indx*k + d;

								if(yResult >= m_imageLaserLineCaptureFrame.GetHeight() || yResult <0) 
								{ 
									continue;
								}

								//zImage.SetPixel(indx,yResult,RGB(255,0,0));
								color = m_imageLaserLineCaptureFrame.GetPixel(indx, yResult);
								r = GetRValue(color);
								g = GetRValue(color);
								b = GetRValue(color);
								if (r > threshold && g > threshold && b > threshold)
								{
									if ( indx > max_indx) { max_indx = indx; max_indy = yResult;}
								}

							}
							
							if(max_indx > xResult) 
							{
								z = max_indx -xResult;
								zImage.SetPixel(max_indx,max_indy,RGB(255,255,0));
								//g_cloudLaserLine.CreatePoint(timeslice, yCheck, z, RGB(0,255,0));

							}
							else
							{
								zImage.SetPixel((int)xResult,(int) yCheck,RGB(0,255,0));
							}
							g_cloudLaserLine.CreatePoint(timeslice, yCheck, z, RGB(0,0,255));

						
				}
			}


			//zImage.Save(zFilename);
			//timeslice++;
		timeslice+=6;

		}
		
		if (m_indexCaptureFile >= 468)//400)//111400)
		{
			KillTimer(13);
			return;
		}

		Invalidate(false);

		return;
	}



	if (nIDEvent == 12)
	{
		int timespan = 6;
		int threshold = 40;

		CString filename;

		// QL 
		CString zFilename;
		CImage zImage;


		if(m_indexCaptureFile <10)
		{
			filename.Format("C:\\Data\\video\\scene0000%d.jpg", m_indexCaptureFile);
			zFilename.Format("C:\\Data\\video-z\\scene0000%d.jpg", m_indexCaptureFile);

		}
		else if(m_indexCaptureFile <100)
		{
			filename.Format("C:\\Data\\video\\scene000%d.jpg", m_indexCaptureFile);
			zFilename.Format("C:\\Data\\video-z\\scene0000%d.jpg", m_indexCaptureFile);
		}
		else if(m_indexCaptureFile <1000)
		{
			filename.Format("C:\\Data\\video\\scene00%d.jpg", m_indexCaptureFile);
			zFilename.Format("C:\\Data\\video-z\\scene0000%d.jpg", m_indexCaptureFile);
		}

		m_indexCaptureFile++;

//		CFileDialog dialogFileOpen(true);
//		if (dialogFileOpen.DoModal() != IDOK)
//			return;

		m_imageLaserLineCaptureFrame.Destroy();

//		wchar_t wszString[512];
//		MultiByteToWideChar(CP_ACP, 0, filename, -1,  wszString,  256);
	
		m_imageLaserLineCaptureFrame.Load(filename);

		// rotation the image 
		/*Bitmap* gdiPlusBitmap=CImage2Image(&m_imageLaserLineCaptureFrame);
		gdiPlusBitmap->RotateFlip(Rotate90FlipNone);
		HBITMAP hbmp;
		gdiPlusBitmap->GetHBITMAP(Color::White, &hbmp);
		m_imageLaserLineCaptureFrame.Detach();
		m_imageLaserLineCaptureFrame.Attach(hbmp);*/

		zImage.Load(filename);

		float z= 0.0f;
		float k = -1.0/slope;
		
		//for (int i=0; i<timespan; i++)
		//if (0)
		{
			//for (int yCheck=0; yCheck<m_imageLaserLineCaptureFrame.GetHeight(); yCheck++)
			for (int xCheck=x0; xCheck<x1; xCheck++)
			
			{
				float yResult = (float(xCheck)*slope + intercept);

				COLORREF color = m_imageLaserLineCaptureFrame.GetPixel(xCheck, yResult);
				DWORD r = GetRValue(color);
				DWORD g = GetRValue(color);
				DWORD b = GetRValue(color);

				if (r < threshold/2 && g < threshold/2 && b < threshold/2)
					{
						
						//zImage.SetPixel(xCheck,yResult,RGB(255,0,0));

						//m_cloudLaserLine.CreatePoint(timeslice, yCheck, 1.0f, RGB(0,0,255));
						// QL to check z value 
						float d = yResult - xCheck*k;
						int max_indy = 0;
						float xResult; 
						float max_indx =0.0;
						for( int indy = int(yResult)+1; indy < m_imageLaserLineCaptureFrame.GetHeight() ;indy++)
					
								
						{

							xResult = float(indy-d)/k;	
							if (xResult >= m_imageLaserLineCaptureFrame.GetWidth() || xResult <0) 
								{ 
									continue;
								}

								//zImage.SetPixel(indx,yResult,RGB(255,0,0));
								color = m_imageLaserLineCaptureFrame.GetPixel(xResult,indy);
								r = GetRValue(color);
								g = GetRValue(color);
								b = GetRValue(color);
								if (r > threshold/2 && g > threshold/2 && b > threshold/2)
								{
									if ( indy > max_indy) { max_indy = indy; max_indx = xResult;}
								}

							}
							
							if(max_indy > yResult) 
							{
								z = max_indy -yResult;
								//zImage.SetPixel(max_indx,max_indy,RGB(255,255,0));
							}
							else
							{
								//zImage.SetPixel((int)xCheck,(int) yResult,RGB(0,255,0));
							}
							g_cloudLaserLine.CreatePoint(xCheck,timeslice, z, RGB(0,0,255));

				}
			}


			//zImage.Save(zFilename);
			timeslice++;
		

		}
		
		if (m_indexCaptureFile >= 468)//400)//111400)
		{
			KillTimer(12);
			return;
		}

		Invalidate(false);

		return;
	}




	CDialog::OnTimer(nIDEvent);
}


void CLaserLinecannerDialog::OnBnClickedButtonLaserlinescanScan()
{
	//x0 = 1500;
	/*x0 = 1508;
	y0 = 0;
	//x1 = 1428;
	x1 = 1441;
	y1 = 1079;*/
	
	x0= 850;
	y0= 533;
	x1= 1800;
	y1= 532;


	slope = float(y0-y1)/float(x0-x1);
	intercept = y0-(slope*float(x0));

	m_indexCaptureFile = 105;//82;//111;//82;
	timeslice = 13;
	//SetTimer(13, 50, NULL);
	SetTimer(12, 50, NULL);
}


void CLaserLinecannerDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	if (!m_imageLaserLineCaptureFrame.IsNull())
	{
		CImage memImageDisplay;
		memImageDisplay.Create(m_imageLaserLineCaptureFrame.GetWidth(), m_imageLaserLineCaptureFrame.GetHeight(), 24);
		CDC memdc;
		memdc.Attach(memImageDisplay.GetDC());

		memdc.FillSolidRect(CRect(0, 0, m_imageLaserLineCaptureFrame.GetWidth(), m_imageLaserLineCaptureFrame.GetHeight()), RGB(255,255,255));

		m_imageLaserLineCaptureFrame.Draw(memdc.GetSafeHdc(), CRect(0, 0, m_imageLaserLineCaptureFrame.GetWidth(), m_imageLaserLineCaptureFrame.GetHeight()));

		CPen penBaseline;
		penBaseline.CreatePen(PS_SOLID, 8, RGB(255,0,0));

		memdc.SelectObject(&penBaseline);

		memdc.MoveTo(x0, y0);
		memdc.LineTo(x1, y1);

		int yCheck = 500;
		float xResult = (float(yCheck)-intercept)/slope;

		memdc.FillSolidRect(xResult-10, yCheck -10, 21, 21, RGB(0,255,0));

		memdc.Detach();
		memImageDisplay.ReleaseDC();

		memImageDisplay.Draw(dc.GetSafeHdc(), 800, 0, 1980/3, 1080/3);

		//m_cloudLaserLine.Draw(&dc, CSize(3,3), CPoint(0,0));
		g_cloudLaserLine.Draw(&dc, CSize(3,3), CPoint(0,0));
	}
}


void CLaserLinecannerDialog::OnBnClickedButtonLaserlinescanSave()
{
	//m_cloudLaserLine.SaveLaserLineData();
	g_cloudLaserLine.SaveLaserLineData();
	AfxMessageBox("save to test4.ply");
}



