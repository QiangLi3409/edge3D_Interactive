#pragma once
#include "afxwin.h"
#include "MotionControl.h"

// CManualMotionCtlDlg dialog

class CManualMotionCtlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualMotionCtlDlg)

public:
	CManualMotionCtlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManualMotionCtlDlg();
	bool GetDoubleFromEditBox(const CString str, float &output);
	bool CManualMotionCtlDlg::GetIntFromEditBox(const CString str, int &output);
// Dialog Data
	enum { IDD = IDD_MODBUD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	CEdit m_acc;
	CComboBox m_MotionSeg;
//	CEdit m_Speed;
//	CEdit m_Acceleration;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCommand();
//	float m_fAcceleration;
//	float m_fSpeed;
	int m_nMotionSegIndex;
	afx_msg void OnSelchangeMotionsegment();
	CEdit m_Speed;
	CEdit m_Acceleration;
	CEdit m_Deceleration;
	CEdit m_Position;
	CEdit m_AccJerk;

	CMotionControl m_MotionCtl;
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnBnClickedSinglestepstart();
	afx_msg void OnBnClickedSinglestepstart();
	afx_msg void OnBnClickedSinglestep();
	afx_msg void OnBnClickedButton1();

	int m_nSegNumAddress;
	int m_nStartAddress;
	int m_nCompleteAddress;
	int m_nCaptureAddress;
	int m_nZAddress;
	int m_nProfileAddress;


};
