
// edge3D StudioDlg.h : header file
//

#include "C3DPoint.h"
#include "C3DCloud.h"
#include "ImageAnalyzer.h"
#include "ImageStone.h"
#include "lensbox.h"

#pragma once




// Cedge3DStudioDlg dialog
class Cedge3DStudioDlg : public CDialogEx
{
// Construction
public:
	Cedge3DStudioDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EDGE3DSTUDIO_DIALOG };

	C3DPoint* m_startC3DPoint;
	C3DPoint* m_curC3DPoint;
	C3DCloud m_cloudJimData;
	C3DCloud m_edgeJimData;
	C3DCloud m_cloudJimData2;
	C3DCloud m_cloudTraceEdge[13];
	int m_indexTraceEdge;
	COLORREF m_colorTraceEdge;
	int m_countLensPoints;
	CSize m_sizeMaxExtents;
	CSize m_sizeMinExtents;

	C3DCloud m_cloudLaserLine;
	CPoint m_centerLaserData;
	float m_scaleLaserData;

	CLensBox m_arrayLensBox[2];
	int m_indexLensBox;

	CPoint m_centerJimData;
	float m_scaleJimData;
	float m_rotateJimData;

	CImageAnalyzer* m_objectImageAnalyzer;

	FCObjImage m_imageLensROI;
	float m_displayScale;

	// QL
	FCObjImage m_imageReference;
	std::vector<std::vector<CPoint>> m_referencelines;
	


		
	CPoint m_pointDisplayOrigin;
	CPoint m_pointScanOrigin;
	bool m_isScanOriginSelected;

	CString filename;

	CPoint LocateEdgePos(int p_startXpos, int p_startYpos, int p_dirX, int p_dirY);
	void TestForMinMax(CPoint p_pointNewEdge);
	bool MatchPointPatternForNextMove(CPoint p_pointPatternMatch);


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CButton m_btnShowLensROIControl;
	afx_msg void OnBnClickedCheckShowLensroi();
	afx_msg void OnBnClickedButtonSavexydata();
	afx_msg void OnBnClickedButtonLoadjimdata();
	afx_msg void OnBnClickedButtonLoadimage();
	afx_msg void OnBnClickedButton2();
	CEdit m_editGamma;
	CEdit m_editThres;
	CEdit m_editAleft;
	CEdit m_editBleft;
	CEdit m_editEDleft;
	CEdit m_editAright;
	CEdit m_editBright;
	CEdit m_editEDright;
	CEdit m_editDBL;
	CEdit m_editDBC;
	afx_msg void OnBnClickedButton3();
	CButton m_btnShowEdge1;
	CButton m_btnShowEdge2;
	CButton m_btnShowEdge3;
	CButton m_btnShowEdge4;
	CButton m_btnShowEdge5;
	CButton m_btnDoLeftLensBox;
	CButton m_btnDoRightLensBox;
	afx_msg void OnBnClickedCheckShowEdge1();
	afx_msg void OnBnClickedCheckShowEdge2();
	afx_msg void OnBnClickedCheckShowEdge3();
	afx_msg void OnBnClickedCheckShowEdge4();
	afx_msg void OnBnClickedCheckShowEdge5();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonSavexydata3();
	afx_msg void OnBnClickedButtonLoadlaserline();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedImagesave();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedManualDlg();
//	afx_msg void OnBnClickedReadmodbus();
	afx_msg void OnBnClickedReadmodbus();
};
