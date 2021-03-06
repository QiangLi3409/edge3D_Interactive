#pragma once
#include "afxwin.h"
#include "MotionControl.h"
#include "Showpic.h"

// CManualMotionCtlDlg dialog
#define TIMER_CHECK_START_CYCLE 100
#define TIMER_INIT_CAMERA 101


#define CAM_WIDTH  2304
#define CAM_HEIGHT 1296
#define CAM_BITS   24
	

class CManualMotionCtlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualMotionCtlDlg)

public:
	CManualMotionCtlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManualMotionCtlDlg();
	bool GetDoubleFromEditBox(const CString str, float &output);
	bool CManualMotionCtlDlg::GetIntFromEditBox(const CString str, int &output);

//	void OneSegMentMove(int nSeg,float fspeed,float facc,float fdece,float fpos,float fjerk);
	
//	void OneSegMentMove(CMotionControl MotionCtl,int nSeg,float fspeed,float facc,float fdece,float fpos,float fjerk,int nSegNumAddr, int nProfileAddr, int nStartAddr, int nCompleteAddr,int nXCaptureAddr=0, int nZAddr=0 );

//	void OneSegMentMove(CMotionControl MotionCtl,int nSeg,float fspeed,float facc,float fdece,float fXpos,float fYpos,float fjerk,int nXSegNumAddr, int nXProfileAddr, int nXStartAddr, int nXCompleteAddr,int nYSegNumAddr, int nYProfileAddr, int nYStartAddr, int nYCompleteAddr,int nXCaptureAddr, int nZAddr );

	void JamesStartCycle();
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
	short m_nStartCycle;

	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnBnClickedSinglestepstart();
	afx_msg void OnBnClickedSinglestepstart();
	afx_msg void OnBnClickedSinglestep();
	afx_msg void OnBnClickedButton1();

	int m_nProfileAddress;
	int m_nSegNumAddress;
	int m_nStartAddress;
	int m_nCompleteAddress;
	int m_nXCaptureAddress;
	int m_nZAddress;
	


	int m_nYProfileAddress;
	int m_nYSegNumAddress;
	int m_nYStartAddress;
	int m_nYCompleteAddress;

	CString m_StartCycleFilename;




//	afx_msg void OnBnClickedReset();
//	afx_msg void OnBnClickedMoveSequence();
	CComboBox m_YMotionSeg;
	afx_msg void OnSelchangeYMotionSeg();
//	afx_msg void OnBnClickedResetY();
	afx_msg void OnBnClickedButton2();
//	afx_msg void OnBnClickedTestMove();
//	afx_msg void OnBnClickedRestall();
//	afx_msg void OnBnClickedWriteTest();
	afx_msg void OnBnClickedReadTest();

	afx_msg void OnBnClicked16points();
	afx_msg void OnStnClickedStatus();
	afx_msg void OnBnClickedGoto0();
	afx_msg void OnBnClickedButtonSingleZ();
	CComboBox m_ZMotionSegNum;
//	afx_msg void OnBnClickedResetZ();
	afx_msg void OnBnClicked16pointsXyz();
//	afx_msg void OnBnClickedRestall();
	void SetUpMotionCtl(void);
	afx_msg void OnBnClickedTestRun();
	afx_msg void OnBnClickedTestCamCapture();
	CComboBox m_ctrlDevice;
	afx_msg void OnBnClickedInitcam();
	CStatic m_ctrlLiveVideo;
//	CShowpic m_CaptureImg;
	afx_msg void OnBnClickedButtonCapture();
	afx_msg void OnLbnSelchangePos();
};
