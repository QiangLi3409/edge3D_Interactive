#include "stdafx.h"
#include "MotionControl.h"


CMotionControl::CMotionControl(void)
{

	//OpenModbus( 9, 9600*2);
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
	m_nSegNum = nSegNum;
	
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
	m_Modbus.ModbusWriteDS(9,9200*2,nStartBitAddress,0XFFFF);

}

void CMotionControl::WriteSingleStepStart(unsigned short nStartBitAddress)
{
	short value = 0x0001 << m_nSegNum;
	if (!m_Modbus.ModbusWriteDS(9,9200*2,nStartBitAddress,value))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
		
}

void CMotionControl::WriteProfile(unsigned short nProfileMBAddress)
{
	m_nMBBaseAdress = nProfileMBAddress + 7*m_nSegNum;
	short posH,posL,velH,velL,acc,dec,jerk;
	acc = (short) m_fAcc;
	dec = (short) m_fDece;
	jerk = (short) m_fJerk;
	posH = (short) ((int)m_fPos/1000);
	posL = (short) ((int)m_fPos%1000);
	velH = (short) ((int)m_fSpeed/1000);
	velL = (short) ((int)m_fSpeed%1000);
	short addr = m_nMBBaseAdress;
	if ( !m_Modbus.ModbusWriteDS(9,9200*2,addr++,posH) ||
	!m_Modbus.ModbusWriteDS(9,9200*2,addr++,posL) ||
	!m_Modbus.ModbusWriteDS(9,9200*2,addr++,velH) ||
	!m_Modbus.ModbusWriteDS(9,9200*2,addr++,velL) ||
	!m_Modbus.ModbusWriteDS(9,9200*2,addr++,acc)  ||
	!m_Modbus.ModbusWriteDS(9,9200*2,addr++,dec)  ||
	!m_Modbus.ModbusWriteDS(9,9200*2,addr++,jerk))
	{
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
	}
	
}

void CMotionControl::WriteSegNum(unsigned short nSegNumAddress)
{
	if (!m_Modbus.ModbusWriteDS(9,9200*2,nSegNumAddress,(short)m_nSegNum))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
		

}
int CMotionControl::ReadSingleStep(unsigned short nCompleteBitAddress, unsigned short nCaptureAddress,unsigned short nZCaptureAddress,short& capture, float& z_value)
{
	

	unsigned short pollstart = nCompleteBitAddress;



	short* value = new short[1];

	short* captureMB = new short[2];
			
	if (!m_Modbus.ModbusReadDSOneByOne(9,9200*2,nCompleteBitAddress,1,value))
		AfxMessageBox(m_Modbus.modbusStatus.c_str());

	if( *value & (0x0001 << m_nSegNum))
	{
		pollstart = nCaptureAddress + m_nSegNum*2;
		m_Modbus.ModbusReadDSOneByOne(9,9200*2,pollstart,2,captureMB);
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
		m_Modbus.ModbusReadFloat(9,9200*2,pollstart,1,&z_value);

		// reset move complete
		short addr = m_nMBBaseAdress;
		m_Modbus.ModbusWriteDS(9,9200*2,addr,0);
		return 0;
	}
	return 1;
}