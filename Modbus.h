#pragma once

#include "Serial.h"
#include <string>


using namespace std;

union FloatBytes
{
        float   f;
        unsigned char   c[0];
};

class CModbus
{
public:
	CModbus(void);
	~CModbus(void);

	CSerial sp;
	string modbusStatus;

	bool Open(int nPort, int baudRate);
    bool Close();


private:
	 void GetCRC(byte* message,int length,  byte* CRC);
	 void BuildMessage(byte address, byte type, unsigned short start, unsigned short registers, byte* message,int nLength);
	 bool CheckResponse(byte* response,int nLength); 
	 void GetResponse(byte* response,int nLength);
     
public:
	bool SendFc16(byte address, unsigned short start, unsigned short registers, short* values);
	bool SendFc3(byte address, unsigned short start, unsigned short registers, short* values);
	bool SendFc6(byte address, unsigned short start, short value);
    void ConvertBytesToFloat(int pollLength,short*values, float* readvalues);

	bool ModbusWriteOneDS(int nPort, int baudRate,unsigned short reg_start, short value);

	bool ModbusWriteMutipleDS(int nPort, int baudRate,unsigned short reg_start, unsigned short writeLength,  short* value);

	bool SendFc16NoResponse(byte address, unsigned short start, unsigned short registers, short* values);
	bool SendFc3NoCRC(byte address, unsigned short start, unsigned short registers, short* values);
	
	bool ModbusReadDSOneByOne(int nPort, int baudRate, unsigned short pollStart, unsigned short pollLength, short* values);
	bool ModbusReadDSMultiple(int nPort, int baudRate, unsigned short pollStart, unsigned short pollLength, short* values);
	bool ModbusReadFloat( unsigned short pollStart, unsigned short Length,float* fvalues);

	bool ModbusReadFloat(int nPort, int baudRate, unsigned short pollStart, unsigned short Length,float* fvalues);
	

};

