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
	DDX_Control(pDX, IDC_JERK, m_Jerk);
	DDX_Control(pDX, IDC_Y_MOTION_SEG, m_YMotionSeg);
}


BEGIN_MESSAGE_MAP(CManualMotionCtlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_COMMAND, &CManualMotionCtlDlg::OnBnClickedCommand)
	ON_CBN_SELCHANGE(IDC_MOTIONSEGMENT, &CManualMotionCtlDlg::OnSelchangeMotionsegment)
	ON_WM_TIMER()
	
	

	ON_BN_CLICKED(IDC_BUTTON1, &CManualMotionCtlDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RESET, &CManualMotionCtlDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_MOVE_SEQUENCE, &CManualMotionCtlDlg::OnBnClickedMoveSequence)
	ON_CBN_SELCHANGE(IDC_Y_MOTION_SEG, &CManualMotionCtlDlg::OnSelchangeYMotionSeg)
	ON_BN_CLICKED(IDC_RESET_Y, &CManualMotionCtlDlg::OnBnClickedResetY)
	ON_BN_CLICKED(IDC_BUTTON2, &CManualMotionCtlDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_TEST_MOVE, &CManualMotionCtlDlg::OnBnClickedTestMove)
END_MESSAGE_MAP()


// CManualMotionCtlDlg message handlers


BOOL CManualMotionCtlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	CString tmp;
	for( int i=1; i<16; i++ )
	{
		tmp.Format("Motion Seg %d",i);
		m_MotionSeg.AddString(tmp);
		m_YMotionSeg.AddString(tmp);
	}

	m_MotionSeg.SetCurSel(0);
	m_YMotionSeg.SetCurSel(0);

	m_nMotionSegIndex = 1;
	m_nYMotionSegIndex = 1;



	GetDlgItem(IDC_PORT)->SetWindowTextA("7");
	GetDlgItem(IDC_BAUDRATE)->SetWindowTextA("19200");
	GetDlgItem(IDC_STARTADDRESS)->SetWindowTextA("424976");
	GetDlgItem(IDC_CAPTUREADDRESS)->SetWindowTextA("400301");
	GetDlgItem(IDC_ZADDRESS)->SetWindowTextA("429273");
	GetDlgItem(IDC_COMPLETEADDRESS)->SetWindowTextA("424977");
	GetDlgItem(IDC_SEGNUMADDRESS)->SetWindowTextA("404498");
	GetDlgItem(IDC_PROFILEADDRESS)->SetWindowTextA("400200");
	

	GetDlgItem(IDC_SPEED)->SetWindowTextA("2000");
	GetDlgItem(IDC_ACCELERATION)->SetWindowTextA("10");
	GetDlgItem(IDC_DECELERATION)->SetWindowTextA("10");
	GetDlgItem(IDC_JERK)->SetWindowTextA("0");
	
	GetDlgItem(IDC_Y_START_ADDRESS)->SetWindowTextA("424978");
	GetDlgItem(IDC_Y_COMPLETE_ADDRESS)->SetWindowTextA("424979");
	GetDlgItem(IDC_Y_SEG_NUM_ADDRESS)->SetWindowTextA("404497");
	GetDlgItem(IDC_Y_PROFILE_ADDRESS)->SetWindowTextA("400320");
	
	




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

bool CManualMotionCtlDlg::GetIntFromEditBox(const CString str, int &output)
{
	    if ( str.GetLength() == 0)
	    {
		  return false;
	    }
	
	    const size_t newsizea = (str.GetLength() + 1);
		char *nstringa = new char[newsizea];
		strcpy_s(nstringa, newsizea, str);
		output = atoi(nstringa);
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
		m_nMotionSegIndex = m_MotionSeg.GetCurSel()+1;
		if( m_nMotionSegIndex < 0 ) return;
       // m_Acceleration.SetWindowTextA("");
		//m_Speed.SetWindowTextA("");
		//m_Deceleration.SetWindowTextA("");
		m_Position.SetWindowTextA("");
		//m_Jerk.SetWindowTextA("");

}


void CManualMotionCtlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == 1)
	{
		int nCompleteBitAddress=m_nCompleteAddress-SERIAL_MODBUS_OFFSET;
		int nCaptureAddress = m_nCaptureAddress-SERIAL_MODBUS_OFFSET;
		int nZCaptureAddress = m_nZAddress - SERIAL_MODBUS_OFFSET;

		short capture;
		float z_value;CString str;
		switch (m_MotionCtl.ReadSingleStep( nCompleteBitAddress,  nCaptureAddress,  nZCaptureAddress, capture, z_value))
		{
		case 0:
			KillTimer(1);
			
			str.Format("sucessful Read Z:%3.2f Ok",z_value);
			GetDlgItem(IDC_STATUS)->SetWindowText(str);
			//AfxMessageBox(str);
			break;
		case 1:
			break;
		case 2:
			str = ("error capture value is 0xFFFF");
			KillTimer(1);
			GetDlgItem(IDC_STATUS)->SetWindowText(str);
			break;
		case 3:
			str= ("error in modbus");
			KillTimer(1);
			GetDlgItem(IDC_STATUS)->SetWindowText(str);
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



	GetDlgItem(IDC_PORT)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_MotionCtl.m_nPort))
	{
		AfxMessageBox("wrong port value");
		return;
	}
	
	

	GetDlgItem(IDC_BAUDRATE)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_MotionCtl.m_nBaudrate))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	GetDlgItem(IDC_STARTADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nStartAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	
	GetDlgItem(IDC_COMPLETEADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nCompleteAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

			
	GetDlgItem(IDC_CAPTUREADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nCaptureAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}


	GetDlgItem(IDC_ZADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nZAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	GetDlgItem(IDC_SEGNUMADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nSegNumAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	GetDlgItem(IDC_PROFILEADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nProfileAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
	

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
	

	m_Jerk.GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, fjerk))
	{
		AfxMessageBox("wrong jerk value");
		return;
	}
	
	
	str.Format("Motion seg:%d acceleration:%3.2f speed:%3.2f deceleration:%3.2f position:%3.2f jerk:%3.2f ",m_nMotionSegIndex, fspeed,facc,fdece,fpos,fjerk);
	//AfxMessageBox(str);

	OneSegMentMove( m_nMotionSegIndex, fspeed, facc, fdece, fpos, fjerk);

/*
	m_MotionCtl.SetAcc(facc);
	m_MotionCtl.SetDece(fdece);
	m_MotionCtl.SetJerk(fjerk);
	m_MotionCtl.SetPos(fpos);
	m_MotionCtl.SetSpeed(fspeed);
	
	m_MotionCtl.SetSegNum(m_nMotionSegIndex);

	CTime t1;
	const clock_t begin_time = clock();
    m_MotionCtl.WriteProfile(m_nProfileAddress-SERIAL_MODBUS_OFFSET);
	float elapse_time = float( clock () - begin_time ) /  CLOCKS_PER_SEC*1000;

	m_MotionCtl.WriteSegNum(m_nSegNumAddress-SERIAL_MODBUS_OFFSET);
	
	m_MotionCtl.WriteSingleStepStart(m_nStartAddress-SERIAL_MODBUS_OFFSET);


	CString str1;
		
	str1.Format("%s,%3.2f","started single movemente:",elapse_time);

	str1 = str1 + str;
	
	GetDlgItem(IDC_STATUS)->SetWindowText(str1);
	//AfxMessageBox("started");
	//int len = 1;
	//short *captureH = new short[len];
	//m_MotionCtl.m_Modbus.ModbusReadDSOneByOne(7,9200*2,400200-SERIAL_MODBUS_OFFSET,len,captureH);
	//m_MotionCtl.m_Modbus.ModbusReadDSOneByOne(7,9200*2,424976-SERIAL_MODBUS_OFFSET,len,captureH);

	SetTimer(1,100,NULL);
	*/
}


void CManualMotionCtlDlg::OnBnClickedReset()
{
	// TODO: Add your control notification handler code here
	CString str;

		GetDlgItem(IDC_SEGNUMADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nSegNumAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
	GetDlgItem(IDC_PORT)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_MotionCtl.m_nPort))
	{
		AfxMessageBox("wrong port value");
		return;
	}
	
	
	GetDlgItem(IDC_BAUDRATE)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_MotionCtl.m_nBaudrate))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
		GetDlgItem(IDC_STARTADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nStartAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
	m_MotionCtl.Reset(m_nSegNumAddress-SERIAL_MODBUS_OFFSET,m_nStartAddress-SERIAL_MODBUS_OFFSET);
	GetDlgItem(IDC_STATUS)->SetWindowText("Reset X");
}

void CManualMotionCtlDlg::OneSegMentMove(CMotionControl MotionCtl,int nSeg,float fspeed,float facc,float fdece,float fpos,float fjerk,int nSegNumAdrr, int nProfileAddr, int nStartAddr, int nCompleteAddr,int nCaptureAddr, int nZAddr)
{
	
	CString str;
	
	MotionCtl.SetAcc(facc);
	MotionCtl.SetDece(fdece);
	MotionCtl.SetJerk(fjerk);
	MotionCtl.SetPos(fpos);
	MotionCtl.SetSpeed(fspeed);
	str.Format("Motion seg:%d speed:%3.2f acceleration:%3.2f deceleration:%3.2f position:%3.2f jerk:%3.2f ",nSeg, fspeed,facc,fdece,fpos,fjerk);

	MotionCtl.SetSegNum(nSeg);

	MotionCtl.WriteProfile(nProfileAddr-SERIAL_MODBUS_OFFSET);
	
	MotionCtl.WriteSegNum(nSegNumAdrr-SERIAL_MODBUS_OFFSET);
	
	MotionCtl.WriteSingleStepStart(nStartAddr-SERIAL_MODBUS_OFFSET);

	Sleep(500);

	CString str1;
		
	str1.Format("Seg %d start",nSeg);

	str1 = str1 + str;
	
	GetDlgItem(IDC_STATUS)->SetWindowText(str1);

	UpdateData(false);

	//SetTimer(1,100,NULL);

	int nCompleteBitAddress=nCompleteAddr-SERIAL_MODBUS_OFFSET;

	int nCaptureAddress = nCaptureAddr-SERIAL_MODBUS_OFFSET;
	int nZCaptureAddress = nZAddr - SERIAL_MODBUS_OFFSET;
	short capture;
	float z_value = -1.;
	// read capture and z value 
	while (1)
	{
		int result = MotionCtl.ReadSingleStep( nCompleteBitAddress,  nCaptureAddress,  nZCaptureAddress, capture, z_value);

		if (result == 0) {str.Format("Seg %d, sucessful capture:%d z:%3.2f",nSeg,capture,z_value); break;}

		if (result == 2) {str.Format("Seg %d, error capture vlaue 0XFFFF",nSeg); AfxMessageBox(str); break;}

		if (result == 3) {str.Format("Seg %d, error Modbus",nSeg); AfxMessageBox(str);break;}

		if (result ==1 ) {str.Format("Seg %d waiting for complete",nSeg);GetDlgItem(IDC_STATUS)->SetWindowText(str);	UpdateData(false);}

	}
		
	GetDlgItem(IDC_STATUS)->SetWindowText(str);	UpdateData(false);

}
void CManualMotionCtlDlg::OneSegMentMove(int nSeg,float fspeed,float facc,float fdece,float fpos,float fjerk)
{
	m_MotionSeg.SetCurSel(nSeg-1);
	
	CString str;


	str.Format("%4.2f",facc);
	m_Acceleration.SetWindowTextA(str);
	str.Format("%4.2f",fspeed);
    m_Speed.SetWindowTextA(str);
	str.Format("%4.2f",fdece);
    m_Deceleration.SetWindowTextA(str);
	str.Format("%4.2f",fpos);
	m_Position.SetWindowTextA(str);
	str.Format("%4.2f",fjerk);
	m_Jerk.SetWindowTextA(str);
	
	
	m_MotionCtl.SetAcc(facc);
	m_MotionCtl.SetDece(fdece);
	m_MotionCtl.SetJerk(fjerk);
	m_MotionCtl.SetPos(fpos);
	m_MotionCtl.SetSpeed(fspeed);
	str.Format("X Motion seg:%d speed:%3.2f acceleration:%3.2f deceleration:%3.2f position:%3.2f jerk:%3.2f ",nSeg, fspeed,facc,fdece,fpos,fjerk);

	m_MotionCtl.SetSegNum(nSeg);

	 m_MotionCtl.WriteProfile(m_nProfileAddress-SERIAL_MODBUS_OFFSET);
	
	m_MotionCtl.WriteSegNum(m_nSegNumAddress-SERIAL_MODBUS_OFFSET);
	
	m_MotionCtl.WriteSingleStepStart(m_nStartAddress-SERIAL_MODBUS_OFFSET);

	Sleep(500);

	CString str1;
		
	str1.Format("Seg %d start",nSeg);

	str1 = str1 + str;
	
	GetDlgItem(IDC_STATUS)->SetWindowText(str1);

	UpdateData(false);

	//SetTimer(1,100,NULL);

	int nCompleteBitAddress=m_nCompleteAddress-SERIAL_MODBUS_OFFSET;
	int nCaptureAddress = m_nCaptureAddress-SERIAL_MODBUS_OFFSET;
	int nZCaptureAddress = m_nZAddress - SERIAL_MODBUS_OFFSET;
	short capture;
	float z_value = -1.;
	// read capture and z value 
	while (1)
	{
		int result = m_MotionCtl.ReadSingleStep( nCompleteBitAddress,  nCaptureAddress,  nZCaptureAddress, capture, z_value);

		if (result == 0) {str.Format("Seg %d, sucessful capture:%d z:%3.2f",nSeg,capture,z_value); break;}

		if (result == 2) {str.Format("Seg %d, error capture vlaue 0XFFFF",nSeg); AfxMessageBox(str); break;}

		if (result == 3) {str.Format("Seg %d, error Modbus",nSeg); AfxMessageBox(str);break;}

		if (result ==1 ) {str.Format("Seg %d waiting for complete",nSeg);GetDlgItem(IDC_STATUS)->SetWindowText(str);	UpdateData(false);}

	}
		
	GetDlgItem(IDC_STATUS)->SetWindowText(str);	UpdateData(false);

}
void CManualMotionCtlDlg::OnBnClickedMoveSequence()
{
	// TODO: Add your control notification handler code here

    UpdateData();
	CString str;
	GetDlgItem(IDC_PORT)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_MotionCtl.m_nPort))
	{
		AfxMessageBox("wrong port value");
		return;
	}
	
	GetDlgItem(IDC_BAUDRATE)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_MotionCtl.m_nBaudrate))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	GetDlgItem(IDC_STARTADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nStartAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	
	GetDlgItem(IDC_COMPLETEADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nCompleteAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

			
	GetDlgItem(IDC_CAPTUREADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nCaptureAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}


	GetDlgItem(IDC_ZADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nZAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	GetDlgItem(IDC_SEGNUMADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nSegNumAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	GetDlgItem(IDC_PROFILEADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_nProfileAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
	

	// This are address 

	float fspeed,facc,fdece,fjerk;

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


	m_Jerk.GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, fjerk))
	{
		AfxMessageBox("wrong jerk value");
		return;
	}
	
	int nSeg = 1;

	float pos[2];
	pos[0] = 5000;
	pos[1] = 0;

	for ( nSeg = 1; nSeg < 16; nSeg++)
	{
		if ( nSeg == 11)
			int a = 1;
	 
		fspeed = 500 + (nSeg-1) * 200.;

		if ((nSeg-1)%2 == 0) 
			OneSegMentMove( nSeg, fspeed, facc, fdece, pos[0], fjerk);
		else
			OneSegMentMove( nSeg, fspeed, facc, fdece, pos[1], fjerk);

		
	}
	
}


void CManualMotionCtlDlg::OnSelchangeYMotionSeg()
{
	// TODO: Add your control notification handler code here

	// TODO: Add your control notification handler code here
		CString str;
		m_nYMotionSegIndex = m_YMotionSeg.GetCurSel()+1;
		if( m_nYMotionSegIndex < 0 ) return;

		GetDlgItem(IDC_Y_POSITION)->SetWindowTextA("");
    

}


void CManualMotionCtlDlg::OnBnClickedResetY()
{
	// TODO: Add your control notification handler code here


	// TODO: Add your control notification handler code here
	CString str;
	int nYSegNumAddress,nYStartAddress;

		GetDlgItem(IDC_Y_SEG_NUM_ADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nYSegNumAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
	GetDlgItem(IDC_PORT)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_MotionCtl.m_nPort))
	{
		AfxMessageBox("wrong port value");
		return;
	}
	
	
	GetDlgItem(IDC_BAUDRATE)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_MotionCtl.m_nBaudrate))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
		GetDlgItem(IDC_Y_START_ADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nYStartAddress))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
	m_MotionCtl.Reset(nYSegNumAddress-SERIAL_MODBUS_OFFSET,nYStartAddress-SERIAL_MODBUS_OFFSET);
	GetDlgItem(IDC_STATUS)->SetWindowText("Reset Y");
}


void CManualMotionCtlDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here


	// TODO: Add your control notification handler code here
		
	UpdateData();
	CString str;
	

	GetDlgItem(IDC_PORT)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_MotionCtl.m_nPort))
	{
		AfxMessageBox("wrong port value");
		return;
	}
	
	

	GetDlgItem(IDC_BAUDRATE)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, m_MotionCtl.m_nBaudrate))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
	int nYStartAddr, nYCompleteAddr, nYSegNumAddr, nYProfileAddr, nCaptureAddr, nZAddr;

	GetDlgItem(IDC_Y_START_ADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nYStartAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	
	GetDlgItem(IDC_Y_COMPLETE_ADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nYCompleteAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

			
	GetDlgItem(IDC_CAPTUREADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nCaptureAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}


	GetDlgItem(IDC_ZADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nZAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	GetDlgItem(IDC_Y_SEG_NUM_ADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nYSegNumAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	GetDlgItem(IDC_Y_PROFILE_ADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nYProfileAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
	

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

	GetDlgItem(IDC_Y_POSITION)->GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, fpos))
	{
		AfxMessageBox("wrong position value");
		return;
	}
	

	m_Jerk.GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, fjerk))
	{
		AfxMessageBox("wrong jerk value");
		return;
	}
	
	
	str.Format("Motion seg:%d acceleration:%3.2f speed:%3.2f deceleration:%3.2f position:%3.2f jerk:%3.2f ",m_nYMotionSegIndex, fspeed,facc,fdece,fpos,fjerk);
	//AfxMessageBox(str);

	OneSegMentMove(m_MotionCtl, m_nYMotionSegIndex, fspeed, facc, fdece, fpos, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nCaptureAddr, nZAddr );


/*
	m_MotionCtl.SetAcc(facc);
	m_MotionCtl.SetDece(fdece);
	m_MotionCtl.SetJerk(fjerk);
	m_MotionCtl.SetPos(fpos);
	m_MotionCtl.SetSpeed(fspeed);
	
	m_MotionCtl.SetSegNum(m_nMotionSegIndex);

	CTime t1;
	const clock_t begin_time = clock();
    m_MotionCtl.WriteProfile(m_nProfileAddress-SERIAL_MODBUS_OFFSET);
	float elapse_time = float( clock () - begin_time ) /  CLOCKS_PER_SEC*1000;

	m_MotionCtl.WriteSegNum(m_nSegNumAddress-SERIAL_MODBUS_OFFSET);
	
	m_MotionCtl.WriteSingleStepStart(m_nStartAddress-SERIAL_MODBUS_OFFSET);


	CString str1;
		
	str1.Format("%s,%3.2f","started single movemente:",elapse_time);

	str1 = str1 + str;
	
	GetDlgItem(IDC_STATUS)->SetWindowText(str1);
	//AfxMessageBox("started");
	//int len = 1;
	//short *captureH = new short[len];
	//m_MotionCtl.m_Modbus.ModbusReadDSOneByOne(7,9200*2,400200-SERIAL_MODBUS_OFFSET,len,captureH);
	//m_MotionCtl.m_Modbus.ModbusReadDSOneByOne(7,9200*2,424976-SERIAL_MODBUS_OFFSET,len,captureH);

	SetTimer(1,100,NULL);
	*/
}


void CManualMotionCtlDlg::OnBnClickedTestMove()
{
	// TODO: Add your control notification handler code here


}
