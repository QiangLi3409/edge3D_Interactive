// ManualMotionCtlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "edge3D Studio.h"
#include "ManualMotionCtlDlg.h"
#include "afxdialogex.h"

#define SERIAL_MODBUS_OFFSET (400000+1)

// CManualMotionCtlDlg dialog

IMPLEMENT_DYNAMIC(CManualMotionCtlDlg, CDialogEx)

CManualMotionCtlDlg::CManualMotionCtlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualMotionCtlDlg::IDD, pParent)
	
{

}

CManualMotionCtlDlg::~CManualMotionCtlDlg()
{
}

void CManualMotionCtlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_ACCELERATION, m_acc);
	DDX_Control(pDX, IDC_MOTIONSEGMENT, m_MotionSeg);
	//  DDX_Control(pDX, IDC_SPEED, m_Speed);
	//  DDX_Control(pDX, IDC_ACCELERATION, m_Acceleration);
	DDX_Control(pDX, IDC_SPEED, m_Speed);
	DDX_Control(pDX, IDC_ACCELERATION, m_Acceleration);
	DDX_Control(pDX, IDC_DECELERATION, m_Deceleration);
	DDX_Control(pDX, IDC_POSITION, m_Position);
	DDX_Control(pDX, IDC_EDIT1, m_AccJerk);
}


BEGIN_MESSAGE_MAP(CManualMotionCtlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_COMMAND, &CManualMotionCtlDlg::OnBnClickedCommand)
	ON_CBN_SELCHANGE(IDC_MOTIONSEGMENT, &CManualMotionCtlDlg::OnSelchangeMotionsegment)
	ON_WM_TIMER()
	
	

	ON_BN_CLICKED(IDC_BUTTON1, &CManualMotionCtlDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CManualMotionCtlDlg message handlers


BOOL CManualMotionCtlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	CString tmp;
	for( int i=0; i<15; i++ )
	{
		tmp.Format("Motion Seg %d",i);
		m_MotionSeg.AddString(tmp);
	}

	m_MotionSeg.SetCurSel(0);
	m_nMotionSegIndex = 0;

	return TRUE;
}

bool CManualMotionCtlDlg::GetDoubleFromEditBox(const CString str, float &output)
{
	    if ( str.GetLength() == 0)
	    {
		  return false;
	    }
	
	    const size_t newsizea = (str.GetLength() + 1);
		char *nstringa = new char[newsizea];
		strcpy_s(nstringa, newsizea, str);
		output = atof(nstringa);
		delete [] nstringa;
		return true;

}
void CManualMotionCtlDlg::OnBnClickedCommand()
{


	
}


void CManualMotionCtlDlg::OnSelchangeMotionsegment()
{
	// TODO: Add your control notification handler code here
		CString str;
		m_nMotionSegIndex = m_MotionSeg.GetCurSel();
		if( m_nMotionSegIndex < 0 ) return;
        m_Acceleration.SetWindowTextA("");
		m_Speed.SetWindowTextA("");
		m_Deceleration.SetWindowTextA("");
		m_Position.SetWindowTextA("");
		m_AccJerk.SetWindowTextA("");

}


void CManualMotionCtlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == 1)
	{
		int nCompleteBitAddress=424977-SERIAL_MODBUS_OFFSET;
		int nCaptureAddress = 400301-SERIAL_MODBUS_OFFSET;
		int nZCaptureAddress = 429273 - SERIAL_MODBUS_OFFSET;
		short capture;
		float z_value;
		switch (m_MotionCtl.ReadSingleStep( nCompleteBitAddress,  nCaptureAddress,  nZCaptureAddress, capture, z_value))
		{
		case 0:
			KillTimer(1);
			AfxMessageBox("read OK");
			break;
		case 1:
			break;
		case 2:
			AfxMessageBox("error capture value is 0xFFFF");
			KillTimer(1);
			break;
		}


	}

	CDialogEx::OnTimer(nIDEvent);
}




void CManualMotionCtlDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
		
	UpdateData();
	CString str;

	
	float fspeed,facc,fdece,fpos,fjerk;

	m_Acceleration.GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, facc))
	{
		AfxMessageBox("wrong acceleration value");
		return;
	}
	    
    m_Speed.GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, fspeed))
	{
		AfxMessageBox("wrong speed value");
		return;
	}

    m_Deceleration.GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, fdece))
	{
		AfxMessageBox("wrong deceleration value");
		return;
	}

	m_Position.GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, fpos))
	{
		AfxMessageBox("wrong position value");
		return;
	}
	

	m_AccJerk.GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, fjerk))
	{
		AfxMessageBox("wrong jerk value");
		return;
	}
	
	m_MotionCtl.SetAcc(facc);
	m_MotionCtl.SetDece(fdece);
	m_MotionCtl.SetJerk(fjerk);
	m_MotionCtl.SetPos(fpos);
	m_MotionCtl.SetSpeed(fspeed);
	str.Format("Motion seg:%d acceleration:%3.2f speed:%3.2f deceleration:%3.2f position:%3.2f jerk:%3.2f ",m_nMotionSegIndex, fspeed,facc,fdece,fpos,fjerk);
	//AfxMessageBox(str);

	m_MotionCtl.SetSegNum(m_nMotionSegIndex);
    m_MotionCtl.WriteSegNum(400300-SERIAL_MODBUS_OFFSET);
	m_MotionCtl.WriteProfile(400200-SERIAL_MODBUS_OFFSET);
	m_MotionCtl.WriteSingleStepStart(424976-SERIAL_MODBUS_OFFSET);
	
	CString str1 = "started single movemente:";

	str1 = str1 + str;

	GetDlgItem(IDC_STATUS)->SetWindowText(str1);
	//AfxMessageBox("started");
	int len = 1;
	short *captureH = new short[len];

	//m_MotionCtl.m_Modbus.ModbusReadDSOneByOne(9,9200*2,400200-SERIAL_MODBUS_OFFSET,len,captureH);
	m_MotionCtl.m_Modbus.ModbusReadDSOneByOne(9,9200*2,424976-SERIAL_MODBUS_OFFSET,len,captureH);

	SetTimer(1,100,NULL);
}
