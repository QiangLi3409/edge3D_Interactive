#pragma once

#include "ImageStone.h"
#include "C3DCloud.h"

// CLaserLinecannerDialog dialog

class CLaserLinecannerDialog : public CDialog
{
	DECLARE_DYNAMIC(CLaserLinecannerDialog)

public:
	CLaserLinecannerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLaserLinecannerDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_LASERLINESCANNER };

	CImage m_imageLaserLineCaptureFrame;
	int m_indexCaptureFile;
	int timeslice;

	float slope;
	float intercept;
	int x0;
	int y0;
	int x1;
	int y1;
	C3DCloud *m_cloudLaserLine;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLaserlinescanScan();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonLaserlinescanSave();
};
