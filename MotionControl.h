#pragma once

#include "Modbus.h"

class CMotionControl
{
public:
	CMotionControl(void);
	CMotionControl(int mbaddr);

	~CMotionControl(void);
	void SetSpeed( float fspped);
	void SetPos( float fPos);
	void SetAcc( float fAcc);
	void SetDece( float fDece);
	void SetJerk( float fJerk);
	void SetSegNum( int nSegNum);
	

	bool OpenModbus(int nPort, int nBaudrate);
	void CloseModubus();
	void Start(unsigned short nStartBitAddress);
	void WriteSingleStepStart(unsigned short nStartBitAddress);
	void WriteProfile( unsigned short nProfileMBAddress);
	void WriteSegNum(unsigned short nSegNumMBAddress);
	int ReadSingleStep(unsigned short nCompleteBitAddress, unsigned short nCaptureAddress, unsigned short nZCaptureAddress, short& capture, float& z_value);

	int m_nPort;
	int m_nBaudrate;

	
//private:
public:
	float m_fSpeed,m_fAcc,m_fDece,m_fPos,m_fJerk;
	int m_nSegNum;
	short m_nMBBaseAdress;
	CModbus m_Modbus;
	unsigned short m_Monster;

};

