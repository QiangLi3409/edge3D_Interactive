#pragma once
// wrapper class for motion control

#include "Modbus.h"

#define PROFILE_NUM 8
typedef struct MovePoint
{
	int id;
	int vect_id;
	int x;
	int y;
	int z;
} MOVEPOINTS;

class CMotionControl
{
public:
	CMotionControl(void);
	~CMotionControl(void);

	void SetSerial( int nPort, int nBaudrate) {m_nPort = nPort; m_nBaudrate = nBaudrate;}
	void SetXModbusAddr(int nStartAddr, int nCompleteAddr, int nSegNumAddr, int nProfileAddr);
	void SetYModbusAddr(int nStartAddr, int nCompleteAddr, int nSegNumAddr, int nProfileAddr);
	void SetZModbusAddr(int nStartAddr, int nCompleteAddr, int nSegNumAddr, int nProfileAddr);

	void SetSequenceAddr(int nSequenceAddr) { m_nSequenceAddr = nSequenceAddr - m_nSERIAL_MODBUS_OFFSET; }

	void SetVecIDAddr(int nVectIDAddr) { m_nVecIDAddr = nVectIDAddr - m_nSERIAL_MODBUS_OFFSET; }

	void SetStartCycleAddr( int nStartCycleAddr) {m_nStartCycleAddr = nStartCycleAddr-m_nSERIAL_MODBUS_OFFSET; }

	void SetXYZCaptgureAddr( int nXcapture, int nYcapture, int nZcapture){m_nXCaptureAddr = nXcapture-m_nSERIAL_MODBUS_OFFSET ;m_nYCaptureAddr=nYcapture-m_nSERIAL_MODBUS_OFFSET; m_nZAddr=nZcapture-m_nSERIAL_MODBUS_OFFSET;}

	bool OneXSegMentMove( float fspeed,float facc,float fdece,float fpos,float fjerk);

	bool OneYSegMentMove( float fspeed,float facc,float fdece,float fpos,float fjerk);

	bool OneZSegMentMove( float fspeed,float facc,float fdece,float fpos,float fjerk);

	
	short ReadStartCycle();

	void ResetAll();

	// 0:OK 1: error 
	int MoveByTedFile(CString filename,float fspeed,float facc,float fdece,float fjerk);
	
	int MoveByTedStage1File(CString filename,float fspeed,float facc,float fdece,float fjerk);
	

    // just call this functions for each move

    void WriteProfile( unsigned short nProfileMBAddress);
	void WriteSegNum(unsigned short nSegNumMBAddress);
    void WriteSingleStepStart(unsigned short nStartBitAddress);
	int  ReadSingleStep(unsigned short nCompleteBitAddress);
	void Reset(unsigned short nSegNumMBAddress,unsigned short nStartBitAddress);
    void SetAcc(float facc);
	void SetDece(float fdece);
	void SetJerk(float fjerk);
	void SetSpeed(float fspeed);
	void SetSegNum( int nSegNum);
	void SetPos(float fpos);

	CString GetMotionCtlMessage() {return m_message;}
	
//private:
protected:
	float m_fSpeed,m_fAcc,m_fDece,m_fJerk, m_fPos;
	CModbus m_Modbus;
    int m_nPort;
	int m_nBaudrate;
	int m_nSegNum;

	// modbus address
	int m_nZStartAddr, m_nZCompleteAddr, m_nZSegNumAddr, m_nZProfileAddr;
	int m_nXStartAddr,m_nYStartAddr, m_nXCompleteAddr, m_nYCompleteAddr;
	int m_nXProfileAddr, m_nYProfileAddr, m_nXSegNumAddr, m_nYSegNumAddr;
	int m_nXCaptureAddr,m_nYCaptureAddr, m_nZAddr;
	int m_nSequenceAddr, m_nVecIDAddr;
    int m_nStartCycleAddr;



	int m_nSERIAL_MODBUS_OFFSET;
	int m_nMEASURE_OFFSET;
	
	CString m_message;




};

