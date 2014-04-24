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
	DDX_Control(pDX, IDC_Z_MOTION_SEG, m_ZMotionSegNum);
}


BEGIN_MESSAGE_MAP(CManualMotionCtlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_COMMAND, &CManualMotionCtlDlg::OnBnClickedCommand)
	ON_CBN_SELCHANGE(IDC_MOTIONSEGMENT, &CManualMotionCtlDlg::OnSelchangeMotionsegment)
	ON_WM_TIMER()
	
	

	ON_BN_CLICKED(IDC_BUTTON1, &CManualMotionCtlDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_MOVE_SEQUENCE, &CManualMotionCtlDlg::OnBnClickedMoveSequence)
	ON_CBN_SELCHANGE(IDC_Y_MOTION_SEG, &CManualMotionCtlDlg::OnSelchangeYMotionSeg)
	ON_BN_CLICKED(IDC_BUTTON2, &CManualMotionCtlDlg::OnBnClickedButton2)
	//ON_BN_CLICKED(IDC_TEST_MOVE, &CManualMotionCtlDlg::OnBnClickedTestMove)
	ON_BN_CLICKED(IDC_RESTALL, &CManualMotionCtlDlg::OnBnClickedRestall)

	//ON_BN_CLICKED(IDC_RUN_AB_SEQUENCE, &CManualMotionCtlDlg::OnBnClickedRunAbSequence)
	ON_BN_CLICKED(IDC_16POINTS, &CManualMotionCtlDlg::OnBnClicked16points)

	ON_BN_CLICKED(IDC_GOTO0, &CManualMotionCtlDlg::OnBnClickedGoto0)
	ON_BN_CLICKED(IDC_BUTTON_SINGLE_Z, &CManualMotionCtlDlg::OnBnClickedButtonSingleZ)
	//ON_BN_CLICKED(IDC_RESET_Z, &CManualMotionCtlDlg::OnBnClickedResetZ)
	ON_BN_CLICKED(IDC_16POINTS_XYZ, &CManualMotionCtlDlg::OnBnClicked16pointsXyz)
	ON_BN_CLICKED(IDC_RESTALL, &CManualMotionCtlDlg::OnBnClickedRestall)
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
		m_ZMotionSegNum.AddString(tmp);
	}

	m_MotionSeg.SetCurSel(0);
	m_YMotionSeg.SetCurSel(0);

	m_ZMotionSegNum.SetCurSel(0);
	m_nMotionSegIndex = 1;
	m_nYMotionSegIndex = 1;

	m_nStartCycle =0;

	GetDlgItem(IDC_PORT)->SetWindowTextA("4");
	GetDlgItem(IDC_BAUDRATE)->SetWindowTextA("38400");
	GetDlgItem(IDC_STARTADDRESS)->SetWindowTextA("424976");
	GetDlgItem(IDC_XCAPTUREADDRESS)->SetWindowTextA("429473");
	GetDlgItem(IDC_YCAPTUREADDRESS)->SetWindowTextA("429503");
	
	GetDlgItem(IDC_ZADDRESS)->SetWindowTextA("429533");
	GetDlgItem(IDC_COMPLETEADDRESS)->SetWindowTextA("424977");
	GetDlgItem(IDC_SEGNUMADDRESS)->SetWindowTextA("404498");
	GetDlgItem(IDC_PROFILEADDRESS)->SetWindowTextA("400200");
	

	GetDlgItem(IDC_SPEED)->SetWindowTextA("8000");
	GetDlgItem(IDC_ACCELERATION)->SetWindowTextA("30");
	GetDlgItem(IDC_DECELERATION)->SetWindowTextA("50");
	GetDlgItem(IDC_JERK)->SetWindowTextA("0");
	
	GetDlgItem(IDC_Y_START_ADDRESS)->SetWindowTextA("424978");
	GetDlgItem(IDC_Y_COMPLETE_ADDRESS)->SetWindowTextA("424979");
	GetDlgItem(IDC_Y_SEG_NUM_ADDRESS)->SetWindowTextA("404497");
	GetDlgItem(IDC_Y_PROFILE_ADDRESS)->SetWindowTextA("400320");
	
	GetDlgItem(IDC_SEQUENCE_NUM)->SetWindowTextA("400008");
	GetDlgItem(IDC_START_CYCLE)->SetWindowTextA("400009");
	
	
	SetTimer(TIMER_CHECK_START_CYCLE,100,NULL);
	SetUpMotionCtl();
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

void CManualMotionCtlDlg::OnSelchangeYMotionSeg()
{
	// TODO: Add your control notification handler code here
		CString str;
		m_nYMotionSegIndex = m_MotionSeg.GetCurSel()+1;
		if( m_nYMotionSegIndex < 0 ) return;
       // m_Acceleration.SetWindowTextA("");
		//m_Speed.SetWindowTextA("");
		//m_Deceleration.SetWindowTextA("");
		m_Position.SetWindowTextA("");
		//m_Jerk.SetWindowTextA("");

}


void CManualMotionCtlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == TIMER_CHECK_START_CYCLE)
	{
		
		m_nStartCycle = m_MotionCtl.ReadStartCycle();
    	if ( m_nStartCycle > 0)
		JamesStartCycle();
	}

	CDialogEx::OnTimer(nIDEvent);
}




void CManualMotionCtlDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
		
	UpdateData();
	CString str;
	SetUpMotionCtl();
	
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
	
	m_MotionCtl.ResetAll();

	if (m_MotionCtl.OneXSegMentMove(m_nMotionSegIndex,fspeed,facc,fdece,fpos,fjerk))
	{
		str.Format("suceed Motion seg:%d position:%3.2f  ",m_nMotionSegIndex,fpos);

	}
	else
		str = m_MotionCtl.GetMotionCtlMessage();
	
		
	GetDlgItem(IDC_STATUS)->SetWindowText(str);
	
}


//void CManualMotionCtlDlg::OnBnClickedReset()
//{
//	// TODO: Add your control notification handler code here
//	CString str;
//
//		GetDlgItem(IDC_SEGNUMADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, m_nSegNumAddress))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//	GetDlgItem(IDC_PORT)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, m_MotionCtl.m_nPort))
//	{
//		AfxMessageBox("wrong port value");
//		return;
//	}
//	
//	
//	GetDlgItem(IDC_BAUDRATE)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, m_MotionCtl.m_nBaudrate))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//		GetDlgItem(IDC_STARTADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, m_nStartAddress))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//	for ( int i=0 ; i< 1 ;i++)
//	{
//		m_MotionCtl.Reset(m_nSegNumAddress-SERIAL_MODBUS_OFFSET,m_nStartAddress-SERIAL_MODBUS_OFFSET);
//		CString temp;
//		temp.Format("Reste X %d",i);
//    	GetDlgItem(IDC_STATUS)->SetWindowText(temp);
//	}
//}

//void CManualMotionCtlDlg::OneSegMentMove(CMotionControl MotionCtl,int nSeg,float fspeed,float facc,float fdece,float fpos,float fjerk,int nSegNumAdrr, int nProfileAddr, int nStartAddr, int nCompleteAddr,int nXCaptureAddr, int nZAddr)
//{
//	
//	CString str;
//	
//	MotionCtl.SetAcc(facc);
//	MotionCtl.SetDece(fdece);
//	MotionCtl.SetJerk(fjerk);
//	MotionCtl.SetPos(fpos);
//	MotionCtl.SetSpeed(fspeed);
//	str.Format("Motion seg:%d speed:%3.2f acceleration:%3.2f deceleration:%3.2f position:%3.2f jerk:%3.2f ",nSeg, fspeed,facc,fdece,fpos,fjerk);
//
//	MotionCtl.SetSegNum(nSeg);
//
//	CTime t1;float elapse_time;
//	clock_t begin_time = clock();
//	MotionCtl.WriteProfile(nProfileAddr-SERIAL_MODBUS_OFFSET);
//	elapse_time = float( clock () - begin_time ) /  CLOCKS_PER_SEC*1000;
//    begin_time = clock();
//
//	MotionCtl.WriteSegNum(nSegNumAdrr-SERIAL_MODBUS_OFFSET);
//	elapse_time = float( clock () - begin_time ) /  CLOCKS_PER_SEC*1000;
//    begin_time = clock();
//
//	
//	MotionCtl.WriteSingleStepStart(nStartAddr-SERIAL_MODBUS_OFFSET);
//    elapse_time = float( clock () - begin_time ) /  CLOCKS_PER_SEC*1000;
//    begin_time = clock();
//
//	str.Format("seg %d write to start time %3.2f",nSeg,elapse_time);
//
//	GetDlgItem(IDC_STATUS)->SetWindowTextA(str);
//
//
//	// this is needed because it will alway give false complete 
//	Sleep(60);
//	
//
//
//	CString str1;
//		
//	str1.Format("Seg %d start",nSeg);
//
//	str1 = str1 + str;
//	
//	GetDlgItem(IDC_STATUS)->SetWindowText(str1);
//
//	UpdateData(false);
//
//	//SetTimer(1,100,NULL);
//
//	int nCompleteBitAddress=nCompleteAddr-SERIAL_MODBUS_OFFSET;
//
//	int nXCaptureAddress = nXCaptureAddr-SERIAL_MODBUS_OFFSET;
//	int nZCaptureAddress = nZAddr - SERIAL_MODBUS_OFFSET;
//	short capture;
//	float z_value = -1.;
//	while (1)
//	{
//		int result = MotionCtl.ReadSingleStep( nCompleteBitAddress,  nXCaptureAddress,  nZCaptureAddress, capture, z_value);
//
//		if (result == 0) {str.Format("Seg %d, sucessful capture:%d z:%3.2f",nSeg,capture,z_value); break;}
//
//		if (result == 2) {str.Format("Seg %d, error capture vlaue 0XFFFF",nSeg); AfxMessageBox(str); break;}
//
//		if (result == 3) {str.Format("Seg %d, error Modbus",nSeg); AfxMessageBox(str);break;}
//
//		if (result ==1 ) {str.Format("Seg %d waiting for complete",nSeg);GetDlgItem(IDC_STATUS)->SetWindowText(str);	UpdateData(false);}
//
//	}
//		
//	GetDlgItem(IDC_STATUS)->SetWindowText(str);	UpdateData(false);
//
//}


//void CManualMotionCtlDlg::OneSegMentMove(int nSeg,float fspeed,float facc,float fdece,float fpos,float fjerk)
//{
//	m_MotionSeg.SetCurSel(nSeg-1);
//	
//	CString str;
//
//
//	str.Format("%4.2f",facc);
//	m_Acceleration.SetWindowTextA(str);
//	str.Format("%4.2f",fspeed);
//    m_Speed.SetWindowTextA(str);
//	str.Format("%4.2f",fdece);
//    m_Deceleration.SetWindowTextA(str);
//	str.Format("%4.2f",fpos);
//	m_Position.SetWindowTextA(str);
//	str.Format("%4.2f",fjerk);
//	m_Jerk.SetWindowTextA(str);
//	
//	
//	m_MotionCtl.SetAcc(facc);
//	m_MotionCtl.SetDece(fdece);
//	m_MotionCtl.SetJerk(fjerk);
//	m_MotionCtl.SetPos(fpos);
//	m_MotionCtl.SetSpeed(fspeed);
//	str.Format("X Motion seg:%d speed:%3.2f acceleration:%3.2f deceleration:%3.2f position:%3.2f jerk:%3.2f ",nSeg, fspeed,facc,fdece,fpos,fjerk);
//
//	m_MotionCtl.SetSegNum(nSeg);
//
//	 m_MotionCtl.WriteProfile(m_nProfileAddress-SERIAL_MODBUS_OFFSET);
//	
//	m_MotionCtl.WriteSegNum(m_nSegNumAddress-SERIAL_MODBUS_OFFSET);
//	
//	m_MotionCtl.WriteSingleStepStart(m_nStartAddress-SERIAL_MODBUS_OFFSET);
//
//
//
//	CString str1;
//		
//	str1.Format("Seg %d start",nSeg);
//
//	str1 = str1 + str;
//	
//	GetDlgItem(IDC_STATUS)->SetWindowText(str1);
//
//	UpdateData(false);
//
//	//SetTimer(1,100,NULL);
//
//	int nCompleteBitAddress=m_nCompleteAddress-SERIAL_MODBUS_OFFSET;
//	int nXCaptureAddress = m_nXCaptureAddress-SERIAL_MODBUS_OFFSET;
//	int nZCaptureAddress = m_nZAddress - SERIAL_MODBUS_OFFSET;
//	short capture;
//	float z_value = -1.;
//	// read capture and z value 
//	while (1)
//	{
//		int result = m_MotionCtl.ReadSingleStep( nCompleteBitAddress,  nXCaptureAddress,  nZCaptureAddress, capture, z_value);
//
//		if (result == 0) {str.Format("Seg %d, sucessful capture:%d z:%3.2f",nSeg,capture,z_value); break;}
//
//		if (result == 2) {str.Format("Seg %d, error capture vlaue 0XFFFF",nSeg); AfxMessageBox(str); break;}
//
//		if (result == 3) {str.Format("Seg %d, error Modbus",nSeg); AfxMessageBox(str);break;}
//
//		if (result ==1 ) {str.Format("Seg %d waiting for complete",nSeg);GetDlgItem(IDC_STATUS)->SetWindowText(str);	UpdateData(false);}
//
//	}
//		
//	GetDlgItem(IDC_STATUS)->SetWindowText(str);	UpdateData(false);
//
//}


void CManualMotionCtlDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here


	// TODO: Add your control notification handler code here
		
	UpdateData();
	CString str;
	SetUpMotionCtl();




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
	
	m_MotionCtl.ResetAll();


	if (m_MotionCtl.OneYSegMentMove(m_nMotionSegIndex,fspeed,facc,fdece,fpos,fjerk))
	{
		str.Format("suceed Motion seg:%d position:%3.2f  ",m_nMotionSegIndex,fpos);

	}
	else
		str = m_MotionCtl.GetMotionCtlMessage();
	  
		
	GetDlgItem(IDC_STATUS)->SetWindowText(str);

}



//void CManualMotionCtlDlg::OnBnClickedTestMove()
//{
//	
//	// TODO: Add your control notification handler code here
//
//	char szFilters[]= "Ted Files (*.sp3)|*.sp3|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
//	CString m_strPathname; 
// Create an Open dialog; the default file name extension is ".my".
//    CFileDialog fileDlg (TRUE, "sp3", "*.sp3",
//    OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
//
// Display the file dialog. When user clicks OK, fileDlg.DoModal()
// returns IDOK.
//if( fileDlg.DoModal ()==IDOK )
//{
//	m_strPathname = fileDlg.GetPathName();
//	//strcpy_s(filename,200,m_strPathname);
//}
//else
//	return;
//
//	int nLength = 0;
//	
//	FILE* fp;
//	fp = fopen(m_strPathname.GetBuffer(m_strPathname.GetLength()), "r");
//    fscanf(fp,"%d\n",&nLength);
//	
//	
//    int *X_pos = new int [nLength]; 
//	int *Y_pos = new int [nLength]; 
//	
//    for(int i=0;i<nLength;i++)
//	{
//		fscanf(fp,"%d %d\n",&X_pos[i],&Y_pos[i]);
//	}
//	fclose(fp);
//
//
//	OnBnClickedRestall();
//	CString str;
//
//	GetDlgItem(IDC_PORT)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, m_MotionCtl.m_nPort))
//	{
//		AfxMessageBox("wrong port value");
//		return;
//	}
//	
//	GetDlgItem(IDC_BAUDRATE)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, m_MotionCtl.m_nBaudrate))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//
//	int nXStartAddr,nYStartAddr, nXCompleteAddr, nYCompleteAddr, nXProfileAddr, nYProfileAddr, nXSegNumAddr, nYSegNumAddr;
//	int nXCaptureAddr,nYCaptureAddr, nZAddr;
//
//	int nSequenceAddr;
//
//
//	
//
//	
//
//	UpdateData();
//
//    GetDlgItem(IDC_SEQUENCE_NUM)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, nSequenceAddr))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//
//
//	GetDlgItem(IDC_SEGNUMADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, nXSegNumAddr))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//
//	GetDlgItem(IDC_PROFILEADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, nXProfileAddr))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//	
//
//	GetDlgItem(IDC_STARTADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, nXStartAddr))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//		
//	GetDlgItem(IDC_COMPLETEADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, nXCompleteAddr))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//
//
//	GetDlgItem(IDC_Y_SEG_NUM_ADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, nYSegNumAddr))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//
//	GetDlgItem(IDC_Y_PROFILE_ADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, nYProfileAddr))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//	
//
//	GetDlgItem(IDC_Y_START_ADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, nYStartAddr))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//		
//	GetDlgItem(IDC_Y_COMPLETE_ADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, nYCompleteAddr))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//
//			
//	GetDlgItem(IDC_XCAPTUREADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, nXCaptureAddr))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//
//		GetDlgItem(IDC_YCAPTUREADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, nYCaptureAddr))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//
//	GetDlgItem(IDC_ZADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, nZAddr))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//
//
//
//	float fspeed,facc,fdece,fpos,fjerk;
//
//	m_Acceleration.GetWindowTextA(str);
//	if (!GetDoubleFromEditBox(str, facc))
//	{
//		AfxMessageBox("wrong acceleration value");
//		return;
//	}
//	    
//    m_Speed.GetWindowTextA(str);
//	if (!GetDoubleFromEditBox(str, fspeed))
//	{
//		AfxMessageBox("wrong speed value");
//		return;
//	}
//
//    m_Deceleration.GetWindowTextA(str);
//	if (!GetDoubleFromEditBox(str, fdece))
//	{
//		AfxMessageBox("wrong deceleration value");
//		return;
//	}
//
//	m_Jerk.GetWindowTextA(str);
//	if (!GetDoubleFromEditBox(str, fjerk))
//	{
//		AfxMessageBox("wrong jerk value");
//		return;
//	}
//
//	// write sequence num
//	nSequenceAddr -=  SERIAL_MODBUS_OFFSET;
//	
//		float *xcapture, *ycapture, *zvalues;
//
//		xcapture = new float[nLength];
//		ycapture = new float[nLength];
//		zvalues = new float[nLength];
//
//		nXCaptureAddr -= SERIAL_MODBUS_OFFSET;
//		nYCaptureAddr -= SERIAL_MODBUS_OFFSET;
//		nZAddr -= SERIAL_MODBUS_OFFSET;
//
//	int nSequenceNum = nLength/15 +1;
//
//	for(int i=0 ; i<nSequenceNum; i++ )
//	{
//		m_MotionCtl.m_Modbus.ModbusWriteOneDS(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,nSequenceAddr,i+1);
//
//		int start = i*15;
//
//		int end = (i+1)*15;
//
//		if( end > nLength) end = nLength;
//
//		for ( int j=start ;j <end ; j++)
//		{
//			float x = X_pos[j]/4.;
//		float y = Y_pos[j]/4.;
//		switch (j)
//		{
//		
//		case 1:
//			x = x - MEASURE_OFFSET;x = x<0?0:x;
//			OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, y, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nXCaptureAddr, nZAddr );
//		    OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, x, fjerk,nXSegNumAddr, nXProfileAddr, nXStartAddr, nXCompleteAddr,nXCaptureAddr, nZAddr);
//			break;
//		case 2:
//		    OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, x, fjerk,nXSegNumAddr, nXProfileAddr, nXStartAddr, nXCompleteAddr,nXCaptureAddr, nZAddr);
//		    OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, y, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nXCaptureAddr, nZAddr );
//			break;
//
//		case 3:
//			x = x + MEASURE_OFFSET;
//			OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, y, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nXCaptureAddr, nZAddr);
//		    OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, x, fjerk,nXSegNumAddr, nXProfileAddr, nXStartAddr, nXCompleteAddr,nXCaptureAddr, nZAddr);
//			break;
//		
//		case 4:
//		     OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, x, fjerk,nXSegNumAddr, nXProfileAddr, nXStartAddr, nXCompleteAddr,nXCaptureAddr, nZAddr);
//			 OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, y, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nXCaptureAddr, nZAddr );
//		  
//			 break;
//		
//		case 5:
//			y= y + MEASURE_OFFSET;
//			OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, x, fjerk,nXSegNumAddr, nXProfileAddr, nXStartAddr, nXCompleteAddr,nXCaptureAddr, nZAddr);
//			OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, y, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nXCaptureAddr, nZAddr );
//		
//			break;
//		case 6:
//		    OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, y, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nXCaptureAddr, nZAddr );
//		    OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, x, fjerk,nXSegNumAddr, nXProfileAddr, nXStartAddr, nXCompleteAddr,nXCaptureAddr, nZAddr);
//			break;
//		case 7:
//			y = y -MEASURE_OFFSET;y = y<0?0:y;
//			OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, x, fjerk,nXSegNumAddr, nXProfileAddr, nXStartAddr, nXCompleteAddr,nXCaptureAddr, nZAddr);
//			OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, y, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nXCaptureAddr, nZAddr );
//			break;
//		default:
//		    OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, x, fjerk,nXSegNumAddr, nXProfileAddr, nXStartAddr, nXCompleteAddr,nXCaptureAddr, nZAddr);
//			OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, y, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nXCaptureAddr, nZAddr );
//			}
//
//		}
//
//					m_MotionCtl.m_Modbus.ModbusReadFloat(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,nXCaptureAddr,end - start,xcapture+start);
//
//		m_MotionCtl.m_Modbus.ModbusReadFloat(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,nYCaptureAddr,end - start,ycapture+start);
//
//		m_MotionCtl.m_Modbus.ModbusReadFloat(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,nZAddr,end - start,zvalues+start);
//
//	}
//	// read capure and z value
//
//
//		// reset sequence as 0
//
//		m_MotionCtl.m_Modbus.ModbusWriteOneDS(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,nSequenceAddr,0);
//
//					
//		CString temp;
//		CListBox* list = (CListBox*) GetDlgItem(IDC_POS);
//
//		for( int i=0;i<nLength;i++)
//		{
//				temp.Format("seg %d x:%3.2f y:%3.2f z:%3.2f",i+1,xcapture[i],ycapture[i],zvalues[i]);
//		        list->AddString(temp);
//
//		}
//
//		// write the result file;
//
//		CFile	myFile;
//      if ( myFile.Open( _T("capture_z.txt"), CFile::modeCreate |   
//   CFile::modeReadWrite | CFile::modeNoTruncate ) )
//{
//    myFile.SeekToEnd();CString szBuffer;
//
//	CString t = CTime::GetCurrentTime().Format("%H:%M");
//    
//	szBuffer.Append(t);
//	szBuffer.Append("\n");
//
//	
//	CString temp;
//
//	temp.Format("speed:%6.4f acce:%6.4f dece:%6.4f \n",fspeed, facc,fdece);
//
//	szBuffer.Append(temp);
//
//	temp.Format("X capture left: %6.4f\n",xcapture[1]);
//	szBuffer.Append(temp);
//
//	temp.Format("X capture left Z:%6.4f\n",zvalues[1]);
//	szBuffer.Append(temp);
//	temp.Format("X capture right: %6.4f\n",xcapture[3]);
//	szBuffer.Append(temp);
//	temp.Format("X capture right Z:%6.4f\n",zvalues[3]);
//	szBuffer.Append(temp);
//	temp.Format("Y capture up: %6.4f\n",ycapture[5]);
//	szBuffer.Append(temp);
//	temp.Format("Y capture up Z:%6.4f\n",zvalues[5]);
//	szBuffer.Append(temp);
//	temp.Format("Y capture down: %6.4f\n",ycapture[7]);
//	szBuffer.Append(temp);
//	temp.Format("Y capture down Z:%6.4f\n",zvalues[7]);
//	szBuffer.Append(temp);
//
//
//	temp.Format( "%6.4f %6.4f %6.4f\n",xcapture[1],-1.,zvalues[1]);
//	szBuffer.Append(temp);
//	temp.Format("%6.4f %6.4f %6.4f\n",xcapture[3],-1.,zvalues[3]);
//	szBuffer.Append(temp);
//	temp.Format("%6.4f %6.4f %6.4f\n",-1.,ycapture[5],zvalues[5]);
//	szBuffer.Append(temp);
//	temp.Format("%6.4f %6.4f %6.4f\n",-1.,ycapture[7],zvalues[7]);
//	szBuffer.Append(temp);
//
//	char xyzfilename[200];
//
//	sprintf(xyzfilename,"%s_XYZ.txt",m_strPathname.GetBuffer(m_strPathname.GetLength()-3));
//
//	FILE* fp = fopen(xyzfilename,"w");
//	fprintf(fp,"%d\n",4);
//	fprintf(fp, "%6.4f %6.4f %6.4f\n",xcapture[1],-1.,zvalues[1]);
//	fprintf(fp, "%6.4f %6.4f %6.4f\n",xcapture[3],-1.,zvalues[3]);
//	fprintf(fp, "%6.4f %6.4f %6.4f\n",-1.,ycapture[5],zvalues[5]);
//	fprintf(fp, "%6.4f %6.4f %6.4f\n",-1.,ycapture[7],zvalues[7]);
//	fclose(fp);
//
//
//
//	myFile.Write(szBuffer.GetBuffer(szBuffer.GetLength()),szBuffer.GetLength());
//	
//		
//}
//	 myFile.Close();
//		
//	UpdateData(false);
//
//	delete [] X_pos;
//	delete [] Y_pos;
//	SetTimer(TIMER_CHECK_START_CYCLE,100,NULL);
//
//	
//    
//
//	
//}


//void CManualMotionCtlDlg::OneSegMentMove(CMotionControl MotionCtl,int nSeg,float fspeed,float facc,float fdece,float fXpos,float fYpos,float fjerk,int nXSegNumAddr, int nXProfileAddr, int nXStartAddr, int nXCompleteAddr,int nYSegNumAddr, int nYProfileAddr, int nYStartAddr, int nYCompleteAddr,int nXCaptureAddr, int nZAddr )
//
//{
//	
//	
//	
//	
//	m_MotionCtl.SetAcc(facc);
//	m_MotionCtl.SetDece(fdece);
//	m_MotionCtl.SetJerk(fjerk);
//
//	m_MotionCtl.SetSpeed(fspeed);
//
//    m_MotionCtl.SetSegNum(nSeg);
//	
//	m_MotionCtl.WriteModubsAll(nXSegNumAddr-SERIAL_MODBUS_OFFSET,nXProfileAddr-SERIAL_MODBUS_OFFSET,nXStartAddr-SERIAL_MODBUS_OFFSET, facc,  fdece,  fXpos,  fspeed,  fjerk );
//	m_MotionCtl.WriteModubsAll(nYSegNumAddr-SERIAL_MODBUS_OFFSET,nYProfileAddr-SERIAL_MODBUS_OFFSET,nYStartAddr-SERIAL_MODBUS_OFFSET, facc,  fdece,  fYpos,  fspeed,  fjerk );
//	
//		Sleep(60);
//	CString str;
//	str.Format("started motion:%d x:%4.2f y:%4.2f ",nSeg, fXpos,fYpos);
//
//	GetDlgItem(IDC_STATUS)->SetWindowText(str);
//
//	UpdateData(false);
//
//	//SetTimer(1,100,NULL);
//
//	short capture;
//	float z_value = -1.;
//	// read capture and z value 
//	while (1)
//	{
//		int resultx = m_MotionCtl.ReadSingleStep( nXCompleteAddr-SERIAL_MODBUS_OFFSET,  nXCaptureAddr-SERIAL_MODBUS_OFFSET,  nZAddr-SERIAL_MODBUS_OFFSET, capture, z_value);
//		int resulty = m_MotionCtl.ReadSingleStep( nYCompleteAddr-SERIAL_MODBUS_OFFSET,  nXCaptureAddr-SERIAL_MODBUS_OFFSET,  nZAddr-SERIAL_MODBUS_OFFSET, capture, z_value);
//
//
//		if (resultx == 0 && resulty == 0) 
//		{
//			str.Format("Seg %d, sucessful capture:%d z:%3.2f",nSeg,capture,z_value); 
//			break;
//
//		}
//
//		if (resultx == 2) {str.Format("Seg %d, x error capture vlaue 0XFFFF",nSeg); AfxMessageBox(str); break;}
//
//		if (resultx == 3) {str.Format("Seg %d, x error Modbus",nSeg); AfxMessageBox(str);break;}
//
//		if (resulty == 2) {str.Format("Seg %d, Y error capture vlaue 0XFFFF",nSeg); AfxMessageBox(str); break;}
//
//		if (resulty == 3) {str.Format("Seg %d, Y error Modbus",nSeg); AfxMessageBox(str);break;}
//
//		if (resultx ==1 || resulty == 1 ) {str.Format("Seg %d waiting for complete",nSeg);
//		
//		}
//
//	}
//		
//	GetDlgItem(IDC_STATUS)->SetWindowText(str);	UpdateData(false);
//}

//void CManualMotionCtlDlg::OnBnClickedRestall()
//{
//	// TODO: Add your control notification handler code here
//	OnBnClickedReset();
//		OnBnClickedResetY();
//		OnBnClickedResetZ();
//}


//void CManualMotionCtlDlg::OnBnClickedWriteTest()
//{
//	// TODO: Add your control notification handler code here
//
//		// TODO: Add your control notification handler code here
//	CString str;
//
//		GetDlgItem(IDC_SEGNUMADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, m_nSegNumAddress))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//	GetDlgItem(IDC_PORT)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, m_MotionCtl.m_nPort))
//	{
//		AfxMessageBox("wrong port value");
//		return;
//	}
//	
//	
//	GetDlgItem(IDC_BAUDRATE)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, m_MotionCtl.m_nBaudrate))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//	GetDlgItem(IDC_STARTADDRESS)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, m_nStartAddress))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//	m_MotionCtl.Reset(m_nSegNumAddress-SERIAL_MODBUS_OFFSET,m_nStartAddress-SERIAL_MODBUS_OFFSET);
//
//	GetDlgItem(IDC_STATUS)->SetWindowText("Reset X");
//
//
//}


void CManualMotionCtlDlg::OnBnClickedReadTest()
{
	// TODO: Add your control notification handler code here

}


//#define TOUCH 


void CManualMotionCtlDlg::OnBnClicked16points()
{
/*
#ifndef TOUCH
	char szFilters[]= "Ted Files (*.sp3)|*.sp3|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CString m_strPathname; 
// Create an Open dialog; the default file name extension is ".my".
    CFileDialog fileDlg (TRUE, "txt", "*.txt",
    OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

// Display the file dialog. When user clicks OK, fileDlg.DoModal()
// returns IDOK.
if( fileDlg.DoModal ()==IDOK )
{
	m_strPathname = fileDlg.GetPathName();
	//strcpy_s(filename,200,m_strPathname);
}
else
	return;


	KillTimer(TIMER_CHECK_START_CYCLE);
	OnBnClickedRestall();
	int i,j;
	int nLength = 0;
	
	FILE* fp;

	//fp = fopen("C:\\data\\frame_sensorpoints[ID_2][Wrap_16]-new1.txt", "r");

		fp = fopen(m_strPathname.GetBuffer(m_strPathname.GetLength()), "r");
	

    fscanf(fp,"%d\n",&nLength);
	

	MOVEPOINTS *points = new MOVEPOINTS [nLength];

	
 
    for( i=0;i<nLength;i++)
	{
		fscanf(fp,"%d %d %d %d\n",&points[i].id,&points[i].vect_id,&points[i].x,&points[i].y);
	}
	fclose(fp);

	int xc = points[0].x;
	int yc = points[0].y;

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

	int nXStartAddr,nYStartAddr, nXCompleteAddr, nYCompleteAddr, nXProfileAddr, nYProfileAddr, nXSegNumAddr, nYSegNumAddr;
	int nXCaptureAddr,nYCaptureAddr, nZAddr;

	int nSequenceAddr;
	
	UpdateData();

    GetDlgItem(IDC_SEQUENCE_NUM)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nSequenceAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}


	GetDlgItem(IDC_SEGNUMADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nXSegNumAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	GetDlgItem(IDC_PROFILEADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nXProfileAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
	

	GetDlgItem(IDC_STARTADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nXStartAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
		
	GetDlgItem(IDC_COMPLETEADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nXCompleteAddr))
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

			
	GetDlgItem(IDC_XCAPTUREADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nXCaptureAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

		GetDlgItem(IDC_YCAPTUREADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nYCaptureAddr))
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

	m_Jerk.GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, fjerk))
	{
		AfxMessageBox("wrong jerk value");
		return;
	}

	// write sequence num
	nSequenceAddr -=  SERIAL_MODBUS_OFFSET;
	float *xcapture, *ycapture, *zvalues;
	xcapture = new float[nLength];
	ycapture = new float[nLength];
	zvalues = new float[nLength];
	nXCaptureAddr -= SERIAL_MODBUS_OFFSET;
	nYCaptureAddr -= SERIAL_MODBUS_OFFSET;
	nZAddr -= SERIAL_MODBUS_OFFSET;

	int nSequenceNum = nLength/15 +1;
	
	for( i=0 ; i<nSequenceNum; i++ )
	{
		
		m_MotionCtl.m_Modbus.ModbusWriteOneDS(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,nSequenceAddr,i+1);
			

		int start = i*15;

		int end = (i+1)*15;

		if( end > nLength) end = nLength;
	    
		for (  j=start ;j <end ; j++)
		{
			
				if(13 == points[j].id)
					int a = 1;
			float x = points[j].x/4.;
			float y = points[j].y/4.;
			switch (points[j].vect_id)
			{
		
			case 2:

				if(points[j].x < xc) x = x - MEASURE_OFFSET;
				if(points[j].x > xc) x = x + MEASURE_OFFSET;
				x = x<0?0:x;
				OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, y, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nXCaptureAddr, nZAddr );
				m_MotionCtl.m_Modbus.ModbusWriteOneDS(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,400007-SERIAL_MODBUS_OFFSET,points[j].vect_id);

				OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, x, fjerk,nXSegNumAddr, nXProfileAddr, nXStartAddr, nXCompleteAddr,nXCaptureAddr, nZAddr);
				break;
		
		
			case 1:
				if(points[j].y < yc) y = y - MEASURE_OFFSET;
				if(points[j].y > yc) y = y + MEASURE_OFFSET;
				y = y<0?0:y;
				OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, x, fjerk,nXSegNumAddr, nXProfileAddr, nXStartAddr, nXCompleteAddr,nXCaptureAddr, nZAddr);
				m_MotionCtl.m_Modbus.ModbusWriteOneDS(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,400007-SERIAL_MODBUS_OFFSET,points[j].vect_id);
				OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, y, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nXCaptureAddr, nZAddr );
				break;
		
			default:
				OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, y, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nXCaptureAddr, nZAddr );
				m_MotionCtl.m_Modbus.ModbusWriteOneDS(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,400007-SERIAL_MODBUS_OFFSET,points[j].vect_id);
				OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, x, fjerk,nXSegNumAddr, nXProfileAddr, nXStartAddr, nXCompleteAddr,nXCaptureAddr, nZAddr);
				
				}

		}

		// need to read all the caputre when sequecne is done
	

		m_MotionCtl.m_Modbus.ModbusReadFloat(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,nXCaptureAddr,end - start,xcapture+start);

		m_MotionCtl.m_Modbus.ModbusReadFloat(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,nYCaptureAddr,end - start,ycapture+start);

		m_MotionCtl.m_Modbus.ModbusReadFloat(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,nZAddr,end - start,zvalues+start);

	} // end of sequence 
	
					
		CString temp;
		CListBox* list = (CListBox*) GetDlgItem(IDC_POS);
		CFile	myFile;CString szBuffer;
		char xyzfilename[200];
		//sprintf(xyzfilename,"%s","xyz.txt");
    
		#ifndef TOUCH
	    sprintf(xyzfilename,"%s_XYZ.txt",m_strPathname.GetBuffer(m_strPathname.GetLength()-3));
#endif 
     fp = fopen(xyzfilename,"w");
		CString t = CTime::GetCurrentTime().Format("%H:%M");
    	szBuffer.Append(t);
		szBuffer.Append("\n");
		for(  i=0;i<nLength;i++)
		{
			switch (points[i].vect_id)
			{
			case 1:
				temp.Format("P_id %d vec_id:1 Y:%3.2f z:%3.2f\n",points[i].id,ycapture[i],zvalues[i]);
		   		fprintf(fp, "%d %6.4f %6.4f %6.4f\n",points[i].id, -1.,ycapture[i],zvalues[i]);
				szBuffer.Append(temp);list->AddString(temp);break;
			case 2:
				temp.Format("P_id %d vec_id:2 X:%3.2f z:%3.2f\n",points[i].id,xcapture[i],zvalues[i]);
				fprintf(fp, "%d %6.4f %6.4f %6.4f\n",points[i].id, xcapture[i],-1.,zvalues[i]);
				szBuffer.Append(temp);list->AddString(temp);break;
			default:
				break;

			}
			
	
		}

		
			if ( myFile.Open( _T("capture_z.txt"), CFile::modeCreate |   
			CFile::modeReadWrite | CFile::modeNoTruncate ) )
			{
				myFile.Write(szBuffer.GetBuffer(szBuffer.GetLength()),szBuffer.GetLength());
	

			}
			myFile.Close();
			fclose(fp);
			UpdateData(false);

	// reset sequence as 0
	OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, 0, fjerk,nXSegNumAddr, nXProfileAddr, nXStartAddr, nXCompleteAddr,nXCaptureAddr, nZAddr);
	OneSegMentMove(m_MotionCtl, (j%15)+1, fspeed, facc, fdece, 0, fjerk,nYSegNumAddr, nYProfileAddr, nYStartAddr, nYCompleteAddr,nXCaptureAddr, nZAddr );
	m_MotionCtl.m_Modbus.ModbusWriteOneDS(m_MotionCtl.m_nPort,m_MotionCtl.m_nBaudrate,nSequenceAddr,0);
	SetTimer(TIMER_CHECK_START_CYCLE,100,NULL);

	MessageBox(xyzfilename);
	#endif */

}


void CManualMotionCtlDlg::JamesStartCycle()
{
	KillTimer(TIMER_CHECK_START_CYCLE);
	
	CString filename = "C:\\data\\startcycle.txt";
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

	m_Jerk.GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, fjerk))
	{
		AfxMessageBox("wrong jerk value");
		return;
	}

	if (0 == m_MotionCtl.MoveByTedFile(filename,fspeed,facc,fdece,fjerk))
	{
		char xyzfilename[200];
	    sprintf(xyzfilename,"%s_XYZ.txt",filename.GetBuffer(filename.GetLength()-3));
		GetDlgItem(IDC_STATUS)->SetWindowTextA(xyzfilename);
	  // MessageBox(xyzfilename);
	}
	else
	{
		MessageBox(m_MotionCtl.GetMotionCtlMessage());

	}
	SetTimer(TIMER_CHECK_START_CYCLE,100,NULL);
}




void CManualMotionCtlDlg::OnBnClickedGoto0()
{
	
		CString str;
	UpdateData();

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

	m_Jerk.GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, fjerk))
	{
		AfxMessageBox("wrong jerk value");
		return;
	}
  
	m_MotionCtl.OneXSegMentMove(m_nMotionSegIndex, fspeed, facc, fdece, 0, fjerk);

	m_MotionCtl.OneYSegMentMove(m_nYMotionSegIndex, fspeed, facc, fdece, 0,fjerk);

	m_MotionCtl.OneZSegMentMove(m_nYMotionSegIndex, fspeed, facc, fdece, 0,fjerk);

	MessageBox("done");


}


void CManualMotionCtlDlg::OnBnClickedButtonSingleZ()
{
	// TODO: Add your control notification handler code here



		
	UpdateData();

	CString str;
		
	int	ZSegNum = m_ZMotionSegNum.GetCurSel()+1;
    SetUpMotionCtl();

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

	GetDlgItem(IDC_Z_POS)->GetWindowTextA(str);
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
	
	m_MotionCtl.ResetAll();


	if (m_MotionCtl.OneZSegMentMove(ZSegNum,fspeed,facc,fdece,fpos,fjerk))
	{
		str.Format("suceed Motion seg:%d position:%3.2f  ",m_nMotionSegIndex,fpos);

	}
	else
		str = m_MotionCtl.GetMotionCtlMessage();
	  
		
	GetDlgItem(IDC_STATUS)->SetWindowText(str);
}


//void CManualMotionCtlDlg::OnBnClickedResetZ()
//{
//	// TODO: Add your control notification handler code here
//
//
//
//	// TODO: Add your control notification handler code here
//	CString str;
//	int nZStartAddr, nZCompleteAddr, nZSegNumAddr, nZProfileAddr;
//
//	float fspeed,facc,fdece,fpos,fjerk;
//	nZStartAddr = 424980;
//	nZCompleteAddr = 424981;
//	nZSegNumAddr   = 404496;
//	nZProfileAddr = 400440;
//
//	GetDlgItem(IDC_PORT)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, m_MotionCtl.m_nPort))
//	{
//		AfxMessageBox("wrong port value");
//		return;
//	}
//	
//	
//	GetDlgItem(IDC_BAUDRATE)->GetWindowTextA(str);
//	if (!GetIntFromEditBox(str, m_MotionCtl.m_nBaudrate))
//	{
//		AfxMessageBox("wrong baudrate value");
//		return;
//	}
//		
//	m_MotionCtl.Reset(nZSegNumAddr-SERIAL_MODBUS_OFFSET,nZStartAddr-SERIAL_MODBUS_OFFSET);
//	GetDlgItem(IDC_STATUS)->SetWindowText("Reset Z");
//}


void CManualMotionCtlDlg::OnBnClicked16pointsXyz()
{
	// TODO: Add your control notification handler code here


	char szFilters[]= "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CString m_strPathname; 
// Create an Open dialog; the default file name extension is ".my".
    CFileDialog fileDlg (TRUE, "txt", "*.txt",
    OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

// Display the file dialog. When user clicks OK, fileDlg.DoModal()
// returns IDOK.

	auto result = fileDlg.DoModal();
    if(result != IDOK) return; // failed

	m_strPathname = fileDlg.GetPathName();


    CString str;
    SetUpMotionCtl();
    
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

	m_Jerk.GetWindowTextA(str);
	if (!GetDoubleFromEditBox(str, fjerk))
	{
		AfxMessageBox("wrong jerk value");
		return;
	}

	if (0 == m_MotionCtl.MoveByTedFile(m_strPathname,fspeed,facc,fdece,fjerk))
	{
		char xyzfilename[200];
	    sprintf(xyzfilename,"%s_XYZ.txt",m_strPathname.GetBuffer(m_strPathname.GetLength()-3));
	    MessageBox(xyzfilename);
	}
	else
	{
		MessageBox(m_MotionCtl.GetMotionCtlMessage());

	}
}


void CManualMotionCtlDlg::OnBnClickedRestall()
{
	// TODO: Add your control notification handler code here
}


void CManualMotionCtlDlg::SetUpMotionCtl(void)
{
	int nPort;
	CString str;

	GetDlgItem(IDC_PORT)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nPort))
	{
		AfxMessageBox("wrong port value");
		return ;
	}
	int nBaudrate;
	GetDlgItem(IDC_BAUDRATE)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nBaudrate))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	int nXStartAddr,nYStartAddr, nXCompleteAddr, nYCompleteAddr, nXProfileAddr, nYProfileAddr, nXSegNumAddr, nYSegNumAddr;
	int nXCaptureAddr,nYCaptureAddr, nZAddr;

	int nSequenceAddr;
	
	int nZStartAddr, nZCompleteAddr, nZSegNumAddr, nZProfileAddr;


	nZStartAddr = 424980;
	nZCompleteAddr = 424981;
	nZSegNumAddr   = 404496;
	nZProfileAddr = 400440;


    GetDlgItem(IDC_SEQUENCE_NUM)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nSequenceAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}


	GetDlgItem(IDC_SEGNUMADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nXSegNumAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

	GetDlgItem(IDC_PROFILEADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nXProfileAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
	

	GetDlgItem(IDC_STARTADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nXStartAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}
		
	GetDlgItem(IDC_COMPLETEADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nXCompleteAddr))
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

			
	GetDlgItem(IDC_XCAPTUREADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nXCaptureAddr))
	{
		AfxMessageBox("wrong baudrate value");
		return;
	}

		GetDlgItem(IDC_YCAPTUREADDRESS)->GetWindowTextA(str);
	if (!GetIntFromEditBox(str, nYCaptureAddr))
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

	m_MotionCtl.SetSerial(nPort,nBaudrate);
	m_MotionCtl.SetXModbusAddr(nXStartAddr,nXCompleteAddr,nXSegNumAddr,nXProfileAddr);
	m_MotionCtl.SetYModbusAddr(nYStartAddr,nYCompleteAddr,nYSegNumAddr,nYProfileAddr);
	m_MotionCtl.SetZModbusAddr(nZStartAddr,nZCompleteAddr,nZSegNumAddr,nZProfileAddr);
	m_MotionCtl.SetSequenceAddr(nSequenceAddr);
	m_MotionCtl.SetXYZCaptgureAddr(nXCaptureAddr,nYCaptureAddr,nZAddr);


	
}
