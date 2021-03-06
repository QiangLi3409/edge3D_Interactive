#include "stdafx.h"
#include "Modbus.h"


CModbus::CModbus(void)
{
	
}


CModbus::~CModbus(void)
{
	
	Close();

}


bool CModbus::Open(int nPort, int baudRate )
{
	//Ensure port isn't already opened:
	char szPort[20];

	wsprintf( szPort, "COM%d", nPort );

			if(!sp.IsOpened())
            {
              try
                {
                    if(!sp.Open(nPort,baudRate))
					{
						AfxMessageBox("CanNOt open serial port");
						return false;

					}
                }
			  catch (exception err)
                {
					
					modbusStatus = "Error open excption";
                    return false;
                }
                modbusStatus = string(szPort) + " opened successfully";
                return true;
            }
            else
            {
                modbusStatus = string(szPort) +" already opened";
                return false;
            }

}

bool CModbus::Close()
{
	 if (sp.IsOpened())
            {
                try
                {
                    if(!sp.Close())
					{
						AfxMessageBox("can not close serial port");

					}
                }
                catch (exception err)
                {
                    modbusStatus = "Error closing excption";
                    return false;
                }
               
                return true;
            }
            else
            {
                modbusStatus = "is not open";
                return false;
            }
}

void CModbus::GetCRC(byte* message, int nLength,  byte* CRC)
{
	 //Function expects a modbus message of any length as well as a 2 byte CRC array in which to 
            //return the CRC values:

            unsigned short CRCFull = 0xFFFF;
            byte CRCHigh = 0xFF, CRCLow = 0xFF;
            char CRCLSB;

            for (int i = 0; i < (nLength) - 2; i++)
            {
                CRCFull = (unsigned short)(CRCFull ^ message[i]);

                for (int j = 0; j < 8; j++)
                {
                    CRCLSB = (char)(CRCFull & 0x0001);
                    CRCFull = (unsigned short)((CRCFull >> 1) & 0x7FFF);

                    if (CRCLSB == 1)
                        CRCFull = (unsigned short)(CRCFull ^ 0xA001);
                }
            }
            CRC[1] = CRCHigh = (byte)((CRCFull >> 8) & 0xFF);
            CRC[0] = CRCLow = (byte)(CRCFull & 0xFF);
}

void CModbus::BuildMessage(byte address, byte type, unsigned short start, unsigned short registers, byte* message,int nLength)
{
	 //Array to receive CRC bytes:
            byte CRC[2];

            message[0] = address;
            message[1] = type;
            message[2] = (byte)(start >> 8);
            message[3] = (byte)start;
            message[4] = (byte)(registers >> 8);
            message[5] = (byte)registers;

            GetCRC(message,nLength, CRC);
            message[nLength - 2] = CRC[0];
            message[nLength - 1] = CRC[1];
}


bool CModbus::CheckResponse(byte* response,int nLength)
        {
            //Perform a basic CRC check:
            byte CRC[2];
            GetCRC(response,nLength,CRC);
            if (CRC[0] == response[nLength - 2] && CRC[1] == response[nLength - 1])
                return true;
            else
                return false;
        }
     
void CModbus::GetResponse(byte* response, int nLength)
        {
            //There is a bug in .Net 2.0 DataReceived Event that prevents people from using this
            //event as an interrupt to handle data (it doesn't fire all of the time).  Therefore
            //we have to use the ReadByte command for a fixed length as it's been shown to be reliable.


			int index =0 ;
			while(1)
            {
				if ( sp.ReadDataWaiting() == nLength)
				{
					sp.ReadData( response,nLength);
					break;
				}
				//response[i] = (byte)(sp.ReadByte());
            }


	

        }


bool CModbus::SendFc3NoCRC(byte address, unsigned short start, unsigned short registers,  short* values)
{
	//Ensure port is open:
            if (sp.IsOpened())
            {
                //Clear in/out buffers:
                
                //Function 3 request is always 8 bytes:
                byte message[8];
                //Function 3 response buffer:
				int response_length = 5 + 2 * registers;
                byte* response = new byte[response_length];

				response[response_length-1] = response[response_length-2] = 255;
                //Build outgoing modbus message:
                BuildMessage(address, (byte)3, start, registers, message,8);
                //Send modbus message to Serial Port:
                try
                {
				
				 int bytewritten = sp.SendData((char*) message, 8);
				  GetResponse(response,5 + 2 * registers);
    			
   	
				}
                catch (exception err)
                {
                    modbusStatus = "Error in read event";
                    return false;
                }
                //Evaluate message:
				if (CheckResponse(response,response_length))
                {
                    //Return requested register values:
                    for (int i = 0; i < (response_length - 5) / 2; i++)
                    {
                        values[i] = response[2 * i + 3];
                        values[i] <<= 8;
                        values[i] += response[2 * i + 4];
                    }
                    modbusStatus = "Read successful";
                    return true;
                }
				else
				{
					   modbusStatus = "CRC error ";
					return false;
				}
                           
				delete [] response;
			
			}
            else
            {
                modbusStatus = "Serial port not open";
                return false;
            }
}



bool CModbus::SendFc3(byte address, unsigned short start, unsigned short registers,  short* values)
{
	//Ensure port is open:
            if (sp.IsOpened())
            {
                //Clear in/out buffers:
                
                //Function 3 request is always 8 bytes:
                byte message[8];
                //Function 3 response buffer:
				int response_length = 5 + 2 * registers;
                byte* response = new byte[response_length];
                //Build outgoing modbus message:
                BuildMessage(address, (byte)3, start, registers, message,8);
                //Send modbus message to Serial Port:
                try
                {
					int bytewritten = sp.SendData((char*) message, 8);
				
                    GetResponse(response,5 + 2 * registers);
                }
                catch (exception err)
                {
                    modbusStatus = "Error in read event";
                    return false;
                }
                //Evaluate message:
				if (CheckResponse(response,response_length))
                {
                    //Return requested register values:
                    for (int i = 0; i < (response_length - 5) / 2; i++)
                    {
                        values[i] = response[2 * i + 3];
                        values[i] <<= 8;
                        values[i] += response[2 * i + 4];
                    }
                    modbusStatus = "Read successful";
                    return true;
                }
                else
                {
                    modbusStatus = "CRC error";
                    return false;
                }
            
				delete [] response;
			
			}
            else
            {
                modbusStatus = "Serial port not open";
                return false;
            }
}

bool CModbus::SendFc16NoResponse(byte address, unsigned short start, unsigned short registers, short* values)
        {
            //Ensure port is open:
            if (sp.IsOpened())
            {
              
                //Message is 1 addr + 1 fcn + 2 start + 2 reg + 1 count + 2 * reg vals + 2 CRC
                int nLength = 9 + 2 * registers;

				byte* message = new byte[9 + 2 * registers];
                //Function 16 response is fixed at 8 bytes
                byte response[8];

		        //Add bytecount to message:
                message[6] = (byte)(registers * 2);
                //Put write values into message prior to sending:
                for (int i = 0; i < registers; i++)
                {
                    message[7 + 2 * i] = (byte)(values[i] >> 8);
                    message[8 + 2 * i] = (byte)(values[i]);
                }
                //Build outgoing message:
                BuildMessage(address, (byte)16, start, registers, message,nLength);
                
                //Send Modbus message to Serial Port:
                try
                {

					
  				sp.SendData((char*) message,nLength);
				delete [] message;
				GetResponse(response,8);
				
                }
                catch (exception err)
                {
                    modbusStatus = "Error in write event: ";
                    return false;
                }
                //Evaluate message:
                if (CheckResponse(response,8))
                {
                    modbusStatus = "Write successful";
                    return true;
                }
                else
                {
                    modbusStatus = "CRC error";
                    return false;
                }
				
            }
            else
            {
                modbusStatus = "Serial port not open";
                return false;
            }

			
        
 }

bool CModbus::SendFc16(byte address, unsigned short start, unsigned short registers, short* values)
        {
            //Ensure port is open:
            if (sp.IsOpened())
            {
              
                //Message is 1 addr + 1 fcn + 2 start + 2 reg + 1 count + 2 * reg vals + 2 CRC
                int nLength = 9 + 2 * registers;

				byte* message = new byte[9 + 2 * registers];
                //Function 16 response is fixed at 8 bytes
                byte response[8];
                //Add bytecount to message:
                message[6] = (byte)(registers * 2);
                //Put write values into message prior to sending:
                for (int i = 0; i < registers; i++)
                {
                    message[7 + 2 * i] = (byte)(values[i] >> 8);
                    message[8 + 2 * i] = (byte)(values[i]);
                }
                //Build outgoing message:
                BuildMessage(address, (byte)16, start, registers, message,nLength);
                
                //Send Modbus message to Serial Port:
                try
                {
					sp.SendData((char*) message,nLength);
			
					GetResponse(response,8);
                }
                catch (exception err)
                {
                    modbusStatus = "Error in write event: ";
                    return false;
                }
                //Evaluate message:
                if (CheckResponse(response,8))
                {
                    modbusStatus = "Write successful";
                    return true;
                }
                else
                {
                    modbusStatus = "CRC error";
                    return false;
                }
            delete [] message;
			}
            else
            {
                modbusStatus = "Serial port not open";
                return false;
            }
        
 }



bool CModbus::SendFc6(byte address, unsigned short start, short value)
        {
            //Ensure port is open:
            if (sp.IsOpened())
            {
              
                

				byte* message = new byte[8];
                //Function 16 response is fixed at 8 bytes
                byte response[8];
          
                //Put write values into message prior to sending:
			

				 byte CRC[2];

            message[0] = address;
            message[1] = 6;
            message[2] = (byte)(start >> 8);
            message[3] = (byte)start;
   			message[4] = (byte)(value >> 8);
			message[5] = (byte)(value);
			GetCRC(message,8, CRC);
            message[8 - 2] = CRC[0];
            message[8 - 1] = CRC[1];
           //Send Modbus message to Serial Port:
                try
                {
					sp.SendData((char*) message,8);
			
					GetResponse(response,8);
                }
                catch (exception err)
                {
                    modbusStatus = "Error in write event: ";
                    return false;
                }
                //Evaluate message:
                if (CheckResponse(response,8))
                {
                    modbusStatus = "Write successful";
                    return true;
                }
                else
                {
                    modbusStatus = "CRC error";
                    return false;
                }
            delete [] message;
			}
            else
            {
                modbusStatus = "Serial port not open";
                return false;
            }
        
 }

void CModbus::ConvertBytesToFloat(int pollLength,short*values, float* readvalues)
	{

		   for (int i = 0; i < (pollLength / 2); i++)
          {
                        int intValue = (int)values[2 * i];
                        intValue <<= 16;
                        intValue += (int)values[2 * i + 1];
                        
						byte test[4];

						test[0] = (unsigned char)intValue & 0xFF;
						test[1] = (unsigned char)((intValue >> 8) & 0xFF);
						test[2] = (unsigned char)((intValue >> 16) & 0xFF);
						test[3] = (unsigned char)((intValue >> 24) & 0xFF);
                        //if (test.Length != 4)
                        //{
                         //   MessageBox.Show("must be 4 byets");
                           //     return;
                        //}
                        for (int j = 0; j < 2; j++)
                        {
                            byte b = test[j];
                            test[j] = test[j + 2];
                            test[j + 2] = b;
                        }
                        //float f = (BitConverter.ToSingle(BitConverter.GetBytes(intValue), 0));
						//float f;
						//memcpy(&f, test, sizeof(float));
						
						 FloatBytes a;
        				for (size_t i = 0; i < sizeof(FloatBytes); ++i)
						{
                					a.c[i] = test[i];
						}
						
						readvalues[i] = a.f;
                    }

	}


bool CModbus::ModbusReadDSMultiple(int nPort, int baudRate, unsigned short pollStart, unsigned short pollLength, short* values)
{
	if (Open(nPort,baudRate)) 
	{
		
		if (!SendFc3(1,pollStart,pollLength,values))
			return false;

	    Close();
		return true;
	}
	else
	{
		modbusStatus = ("can not open serial port");
		return false;
	}

}


bool CModbus::ModbusReadDSOneByOne(int nPort, int baudRate, unsigned short pollStart, unsigned short pollLength, short* values)
{
	if (Open(nPort,baudRate)) 
	{
		int ncount =0;

		for( int i=0 ;i<pollLength;i++)
		{
			short value;
			 try
			  {
			    if (!SendFc3(1,pollStart+i,1,values+i))
					return false;
				
			  }
			 catch(exception err)
				{
					modbusStatus =("Error in read");
					return false;
				}

						
			
		}
        Close();
		return true;
	}
	else
	{
		modbusStatus = ("can not open serial port");
		return false;
	}

}


bool CModbus::ModbusWriteOneDS(int nPort, int baudRate,unsigned short reg_start, short value)
{
	if (Open(nPort,baudRate)) 
	{
		  try
		  {
			  //const clock_t begin_time = clock(); 
			 // short values[2];
			 // values[0]= value;
			 // values[1] = 0;
			 if(! SendFc16(1,reg_start,1,&value))
				 return false;

			  //float elaps = clock() - begin_time;
			 // int a = 1;
			  
		  }
	     catch(exception err)
            {
                modbusStatus =("Error in read");
				Close();
				return false;
            }
	 Close();
	 return true;
  }
else
	{
		modbusStatus = ("can not open serial port");
		return false;
	}
}




bool CModbus::ModbusReadFloat(int nPort, int baudRate, unsigned short pollStart, unsigned short Length,float* fvalues)
{
	
	if (Open(nPort,baudRate))
	{
		  // length of register to read
		  unsigned short pollLength = Length*2;

		  short* values = new short[pollLength];
		  int ncount = 0;

    //	  for( int i=0 ;i<pollLength/2;i++)

		  {
			  	  
		//	  short onefloat[2];

			  
			  try
				  {
					   if (!SendFc3(1,pollStart,pollLength,values))
					   {
						   delete [] values;
						   return false;
					   }


				  }
				 catch(exception err)
					{
						modbusStatus = ("Error in read");
						delete [] values;
						return false;
					}


				  
		  }


		  ConvertBytesToFloat(pollLength,values,fvalues);
		  

		  delete [] values;
    	  Close();
          return true;
	}
	else
	{
		modbusStatus = ("can not open serial port");
		return false;
	}

}


bool CModbus::ModbusWriteMutipleDS(int nPort, int baudRate,unsigned short reg_start, unsigned short  writeLength, short* value)
{
	if (Open(nPort,baudRate)) 
	{
		  try
		  {
			  //const clock_t begin_time = clock(); 
			   if (!SendFc16(1,reg_start,writeLength,value))
				   return false;

			  //float elaps = clock() - begin_time;
			 // int a = 1;
			  
		  }
	     catch(exception err)
            {
                modbusStatus =("Error in read");
				Close();
				return false;
            }
	 Close();
	 return true;
  }
else
	{
		modbusStatus = ("can not open serial port");
		return false;
	}
}

