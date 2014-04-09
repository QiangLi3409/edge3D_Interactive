#include "stdafx.h"
#include "MotionControl.h"


CMotionControl::CMotionControl(void)
{

	//OpenModbus( 9, 9600*2);
	m_nPort = 9;
	m_nBaudrate = 19200;

}


CMotionControl::~CMotionControl(void)
{
	CloseModubus();
}

void CMotionControl::SetAcc(float facc)
{
	m_fAcc = facc;
}

void CMotionControl::SetPos(float fpos)
{
	m_fPos = fpos;
}

void CMotionControl::SetDece(float fdece)
{
	m_fDece = fdece;
}

void CMotionControl::SetJerk(float fjerk)
{
	m_fJerk = fjerk;
}

void CMotionControl::SetSpeed(float fspeed)
{
	m_fSpeed = fspeed;
}

CMotionControl::CMotionControl(int mbaddr)
{
	m_nMBBaseAdress = mbaddr;
}

void CMotionControl::SetSegNum( int nSegNum)
{
	// m_nSegNum is from 0 for compute modbus address 
	m_nSegNum = nSegNum-1;
	
}

bool CMotionControl::OpenModbus( int nPort, int nBaudrate)
{
	return m_Modbus.Open(nPort,nBaudrate);
}

void CMotionControl::CloseModubus()
{
	m_Modbus.Close();
}

void CMotionControl::Start(unsigned short nStartBitAddress)
{
	m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,nStartBitAddress,0XFFFF);

}
void CMotionControl::Write0ToStart(unsigned short nStartBitAddress)
{
	if (!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,nStartBitAddress,0))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
}
void CMotionControl::WriteSingleStepStart(unsigned short nStartBitAddress)
{
	short value = 0x0001 << m_nSegNum;
	//if (!m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,nStartBitAddress,value))
	if (!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,nStartBitAddress,value))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
		
}
#define PROFILE_NUM 8

void CMotionControl::WriteModubsAll(unsigned short nSegNumMBAddress,unsigned short nProfileMBAddress,unsigned short nStartBitAddress,float fAcc, float fDece, float fPos, float fSpeed, float fJerk )
{
	

	m_nMBBaseAdress = nProfileMBAddress + PROFILE_NUM*m_nSegNum;
	short posH,posL,velH,velL,acc,dec,jerk;
	acc = (short) fAcc;
	dec = (short) fDece;
	jerk = (short) fJerk;
	posH = (short) ((int)fPos/1000);
	posL = (short) ((int)fPos%1000);
	velH = (short) ((int)fSpeed/1000);
	velL = (short) ((int)fSpeed%1000);
	short addr = m_nMBBaseAdress;

	short* profile = new short[PROFILE_NUM];

	profile[0]=posH;profile[1]=posL;profile[2]=velH;profile[3]=velL;profile[4]=acc;profile[5]=dec;profile[6]=0;profile[7]=jerk;

	if ( !m_Modbus.ModbusWriteMutipleDSNoResponse(m_nPort,m_nBaudrate,addr,PROFILE_NUM,profile))
		{
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
	}
	// need to dealy

		Sleep(200);

		if (!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,nSegNumMBAddress,0))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));

		Sleep(50);

		if (!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,nSegNumMBAddress,(short)m_nSegNum+1))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));

		Sleep(200);

	short value = 0x0001 << m_nSegNum;
	//if (!m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,nStartBitAddress,value))
	if (!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,nStartBitAddress,value))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
		
	delete [] profile;
}

void CMotionControl::WriteProfile(unsigned short nProfileMBAddress)
{
	
	m_nMBBaseAdress = nProfileMBAddress + PROFILE_NUM*m_nSegNum;
	short posH,posL,velH,velL,acc,dec,jerk;
	acc = (short) m_fAcc;
	dec = (short) m_fDece;
	jerk = (short) m_fJerk;
	posH = (short) ((int)m_fPos/1000);
	posL = (short) ((int)m_fPos%1000);
	velH = (short) ((int)m_fSpeed/1000);
	velL = (short) ((int)m_fSpeed%1000);
	short addr = m_nMBBaseAdress;

	short* profile = new short[PROFILE_NUM];

	profile[0]=posH;profile[1]=posL;profile[2]=velH;profile[3]=velL;profile[4]=acc;profile[5]=dec;profile[6]=0;profile[7]=jerk;

	if ( !m_Modbus.ModbusWriteMutipleDSNoResponse(m_nPort,m_nBaudrate,addr,PROFILE_NUM,profile))
		{
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
	}
	// need to dealy
	Sleep(200);
/*	if ( !m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,addr++,posH) ||
	!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,addr++,posL) ||
	!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,addr++,velH) ||
	!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,addr++,velL) ||
	!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,addr++,acc)  ||
	!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,addr++,dec)  ||
	!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,++addr,jerk))
	{
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
	}*/

	/*if ( !m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,addr++,posH) ||
	!m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,addr++,posL) ||
	!m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,addr++,velH) ||
	!m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,addr++,velL) ||
	!m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,addr++,acc)  ||
	!m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,addr++,dec)  ||
	!m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,++addr,jerk))
	{
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
	}*/
	
}


void CMotionControl::Reset(unsigned short nSegNumMBAddress,unsigned short nStartBitAddress)
{
	// m_nSegNum is 0 but in modbus is 1
/*	if (!m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,nSegNumMBAddress,0))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
	if (!m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,nStartBitAddress,0))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));*/
	const clock_t begin_time = clock();
	if (!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,nSegNumMBAddress,0))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
	

	if (!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,nStartBitAddress,0))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
	float elaps = clock() - begin_time;

}

void CMotionControl::WriteSegNum(unsigned short nSegNumAddress)
{
	// m_nSegNum is 0 but in modbus is 1
	//if (!m_Modbus.ModbusWriteDS(m_nPort,m_nBaudrate,nSegNumAddress,(short)m_nSegNum+1))
	//	AfxMessageBox((m_Modbus.modbusStatus.c_str()));

		if (!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,nSegNumAddress,(short)m_nSegNum+1))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
		

}
int CMotionControl::ReadSingleStep(unsigned short nCompleteBitAddress, unsigned short nCaptureAddress,unsigned short nZCaptureAddress,short& capture, float& z_value)
{
	
	unsigned short pollstart = nCompleteBitAddress;
	short value[1] ; //= new short[1];
    short captureMB[2]; // = new short[2];
			
	if (!m_Modbus.ModbusReadDSOneByOneNoCRC(m_nPort,m_nBaudrate,nCompleteBitAddress,1,value))
		AfxMessageBox(m_Modbus.modbusStatus.c_str());

	//	if (!m_Modbus.ModbusReadDSOneByOne(m_nPort,m_nBaudrate,nCompleteBitAddress,1,value))
	//	AfxMessageBox(m_Modbus.modbusStatus.c_str());


	if( *value & (0x0001 << m_nSegNum))
	{
		
		return 0;
		pollstart = nCaptureAddress + m_nSegNum*2;

		if (!m_Modbus.ModbusReadDSOneByOneNoCRC(m_nPort,m_nBaudrate,pollstart,2,captureMB))
		{return 3;}
	//	if (!m_Modbus.ModbusReadDSOneByOne(m_nPort,m_nBaudrate,pollstart,2,captureMB))
	//	{return 3;}

		
		// read capture
		short val = captureMB[0]*1000+captureMB[1];

		if( val == 0xFFFF) 
		{
			AfxMessageBox("wrong capture value");
			return 2;
		}

		capture = val;
		// read z value
		pollstart = nZCaptureAddress + m_nSegNum*2;
			if (!m_Modbus.ModbusReadFloatNoCRC(m_nPort,m_nBaudrate,pollstart,1,&z_value))
			return 3;
		
		//if (!m_Modbus.ModbusReadFloat(m_nPort,m_nBaudrate,pollstart,1,&z_value))
		//	return 3;

		// reset move complete
		short addr = m_nMBBaseAdress;
		if(!m_Modbus.ModbusWriteDSNoResponse(m_nPort,m_nBaudrate,addr,0))
			return 3;
		return 0;
	}
	return 1;
}