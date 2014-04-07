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

	void OneSegMentMove(int nSeg,float fspeed,float facc,float fdece,float fpos,float fjerk);
	
	void OneSegMentMove(CMotionControl MotionCtl,int nSeg,float fspeed,float facc,float fdece,float fpos,float fjerk,int nSegNumAddr, int nProfileAddr, int nStartAddr, int nCompleteAddr,int nCaptureAddr, int nZAddr );


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
	int m_nYMotionSegIndex;

	afx_msg void OnSelchangeMotionsegment();
	CEdit m_Speed;
	CEdit m_Acceleration;
	CEdit m_Deceleration;
	CEdit m_Position;
	CEdit m_Jerk;

	CMotionControl m_MotionCtl;

	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnBnClickedSinglestepstart();
	afx_msg void OnBnClickedSinglestepstart();
	afx_msg void OnBnClickedSinglestep();
	afx_msg void OnBnClickedButton1();

	int m_nProfileAddress;
	int m_nSegNumAddress;
	int m_nStartAddress;
	int m_nCompleteAddress;
	int m_nCaptureAddress;
	int m_nZAddress;
	


	int m_nYProfileAddress;
	int m_nYSegNumAddress;
	int m_nYStartAddress;
	int m_nYCompleteAddress;

	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedMoveSequence();
	CComboBox m_YMotionSeg;
	afx_msg void OnSelchangeYMotionSeg();
	afx_msg void OnBnClickedResetY();
	afx_msg void OnBnClickedButton2();
};
