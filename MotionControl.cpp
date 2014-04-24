#include "stdafx.h"
#include "MotionControl.h"


CMotionControl::CMotionControl(void)
{

	m_nPort = 4;
	m_nBaudrate = 9600*4;
		
	/*nZStartAddr = 424980;
	nZCompleteAddr = 424981;
	nZSegNumAddr   = 404496;
	nZProfileAddr = 400440;

		
	GetDlgItem(IDC_Y_START_ADDRESS)->SetWindowTextA("424978");
	GetDlgItem(IDC_Y_COMPLETE_ADDRESS)->SetWindowTextA("424979");
	GetDlgItem(IDC_Y_SEG_NUM_ADDRESS)->SetWindowTextA("404497");
	GetDlgItem(IDC_Y_PROFILE_ADDRESS)->SetWindowTextA("400320");

		GetDlgItem(IDC_STARTADDRESS)->SetWindowTextA("424976");
	GetDlgItem(IDC_XCAPTUREADDRESS)->SetWindowTextA("429473");
	GetDlgItem(IDC_YCAPTUREADDRESS)->SetWindowTextA("429503");
	
	GetDlgItem(IDC_ZADDRESS)->SetWindowTextA("429533");
	GetDlgItem(IDC_COMPLETEADDRESS)->SetWindowTextA("424977");
	GetDlgItem(IDC_SEGNUMADDRESS)->SetWindowTextA("404498");
	GetDlgItem(IDC_PROFILEADDRESS)->SetWindowTextA("400200");*/
		m_nSERIAL_MODBUS_OFFSET  = (400000+1);

	 m_nZStartAddr= 424980- m_nSERIAL_MODBUS_OFFSET;
	 m_nZCompleteAddr= 424981 - m_nSERIAL_MODBUS_OFFSET;
	 m_nZSegNumAddr = 404496 - m_nSERIAL_MODBUS_OFFSET;
	 m_nZProfileAddr = 400440- m_nSERIAL_MODBUS_OFFSET;

	 m_nXStartAddr = 424976- m_nSERIAL_MODBUS_OFFSET ;
     m_nXCompleteAddr = 424977- m_nSERIAL_MODBUS_OFFSET ;
	 m_nXProfileAddr = 400200- m_nSERIAL_MODBUS_OFFSET; 
	 m_nXSegNumAddr = 404498- m_nSERIAL_MODBUS_OFFSET; 

	 m_nYStartAddr = 424978- m_nSERIAL_MODBUS_OFFSET;
	 m_nYCompleteAddr = 424979- m_nSERIAL_MODBUS_OFFSET;
	 m_nYProfileAddr = 400320- m_nSERIAL_MODBUS_OFFSET;
	 m_nYSegNumAddr = 404497- m_nSERIAL_MODBUS_OFFSET;

	m_nXCaptureAddr = 429473- m_nSERIAL_MODBUS_OFFSET;
	m_nYCaptureAddr = 429503- m_nSERIAL_MODBUS_OFFSET;
	m_nZAddr = 429533- m_nSERIAL_MODBUS_OFFSET;

	m_nSequenceAddr = 400008- m_nSERIAL_MODBUS_OFFSET;
	m_nVecIDAddr = 400007- m_nSERIAL_MODBUS_OFFSET;

	m_nStartCycleAddr = 400009- m_nSERIAL_MODBUS_OFFSET;


	 m_nMEASURE_OFFSET =650;


}




CMotionControl::~CMotionControl(void)
{
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


void CMotionControl::SetSegNum( int nSegNum)
{
	// m_nSegNum is from 0 for compute modbus address 
	m_nSegNum = nSegNum-1;
	
}

void CMotionControl::SetXModbusAddr(int nStartAddr, int nCompleteAddr, int nSegNumAddr, int nProfileAddr)
{
	
	 m_nXStartAddr = nStartAddr -m_nSERIAL_MODBUS_OFFSET;
     m_nXCompleteAddr = nCompleteAddr -m_nSERIAL_MODBUS_OFFSET;
	 m_nXProfileAddr = nProfileAddr -m_nSERIAL_MODBUS_OFFSET; 
	 m_nXSegNumAddr = nSegNumAddr-m_nSERIAL_MODBUS_OFFSET; 


}

void CMotionControl::SetYModbusAddr(int nStartAddr, int nCompleteAddr, int nSegNumAddr, int nProfileAddr)
{
	
	 m_nYStartAddr = nStartAddr -m_nSERIAL_MODBUS_OFFSET;
     m_nYCompleteAddr = nCompleteAddr -m_nSERIAL_MODBUS_OFFSET;
	 m_nYProfileAddr = nProfileAddr -m_nSERIAL_MODBUS_OFFSET; 
	 m_nYSegNumAddr = nSegNumAddr-m_nSERIAL_MODBUS_OFFSET; 
}
void CMotionControl::SetZModbusAddr(int nStartAddr, int nCompleteAddr, int nSegNumAddr, int nProfileAddr)
{
	
	 m_nZStartAddr = nStartAddr -m_nSERIAL_MODBUS_OFFSET;
     m_nZCompleteAddr = nCompleteAddr-m_nSERIAL_MODBUS_OFFSET ;
	 m_nZProfileAddr = nProfileAddr -m_nSERIAL_MODBUS_OFFSET; 
	 m_nZSegNumAddr = nSegNumAddr-m_nSERIAL_MODBUS_OFFSET; 
}

void CMotionControl::WriteSingleStepStart(unsigned short nStartBitAddress)
{
	short value = 0x0001 << m_nSegNum;
	if (!m_Modbus.ModbusWriteOneDS(m_nPort,m_nBaudrate,nStartBitAddress,value))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
		
}

void CMotionControl::WriteProfile(unsigned short nProfileMBAddress)
{

	int m_nMBBaseAdress = nProfileMBAddress + PROFILE_NUM*m_nSegNum;
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

	if ( !m_Modbus.ModbusWriteMutipleDS(m_nPort,m_nBaudrate,addr,PROFILE_NUM,profile))
		{
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
	}

	delete [] profile;
	// need to dealy


}
bool CMotionControl::OneXSegMentMove(int nSeg, float fspeed,float facc,float fdece,float fpos,float fjerk)
{
		
	SetAcc(facc);
	SetDece(fdece);
	SetJerk(fjerk);
	SetPos(fpos);
	SetSpeed(fspeed);
	SetSegNum(nSeg);
		
	WriteProfile(m_nXProfileAddr);
	WriteSegNum(m_nXSegNumAddr);
	WriteSingleStepStart(m_nXStartAddr);
   
	Sleep(60);
	
		
	while (1)
	{
		int result = ReadSingleStep( m_nXCompleteAddr);

		if (result == 0) {m_message.Format("X Seg %d, sucessful",nSeg); return true; break;}
			
		if (result == 2) {m_message.Format("X Seg %d, error Modbus",nSeg); return false; break;}

		if (result == 1 ) {m_message.Format("X Seg %d waiting for complete",nSeg);}

	}
		
	return true;
}

bool CMotionControl::OneYSegMentMove(int nSeg, float fspeed,float facc,float fdece,float fpos,float fjerk)
{
		
	SetAcc(facc);
	SetDece(fdece);
	SetJerk(fjerk);
	SetPos(fpos);
	SetSpeed(fspeed);
	SetSegNum(nSeg);
		
	WriteProfile(m_nYProfileAddr);
	WriteSegNum(m_nYSegNumAddr);
	WriteSingleStepStart(m_nYStartAddr);
   
	Sleep(60);
	
		
	while (1)
	{
		int result = ReadSingleStep( m_nYCompleteAddr);

		if (result == 0) {m_message.Format("Y Seg %d, sucessful",nSeg); return true; break;}
			
		if (result == 2) {m_message.Format("Y Seg %d, error Modbus",nSeg); return false; break;}

		if (result == 1 ) {m_message.Format("Y Seg %d waiting for complete",nSeg);}

	}
		
	return true;
}



bool CMotionControl::OneZSegMentMove(int nSeg, float fspeed,float facc,float fdece,float fpos,float fjerk)
{
		
	SetAcc(facc);
	SetDece(fdece);
	SetJerk(fjerk);
	SetPos(fpos);
	SetSpeed(fspeed);
	SetSegNum(nSeg);
		
	WriteProfile(m_nZProfileAddr);
	WriteSegNum(m_nZSegNumAddr);
	WriteSingleStepStart(m_nZStartAddr);
   
	Sleep(60);
	
		
	while (1)
	{
		int result = ReadSingleStep( m_nZCompleteAddr);

		if (result == 0) {m_message.Format("Z Seg %d, sucessful",nSeg); return true; break;}
			
		if (result == 2) {m_message.Format("Z Seg %d, error Modbus",nSeg); return false; break;}

		if (result == 1 ) {m_message.Format("Z Seg %d waiting for complete",nSeg);}

	}
	
	return true;
}


short CMotionControl::ReadStartCycle()
{
	short value;	
	m_Modbus.ModbusReadDSOneByOne(m_nPort,m_nBaudrate,m_nStartCycleAddr,1,&value);
	return value;
}

void CMotionControl::ResetAll()
{
	Reset(m_nXSegNumAddr,m_nXStartAddr);
	Reset(m_nYSegNumAddr,m_nYStartAddr);
	Reset(m_nZSegNumAddr,m_nZStartAddr);

}
void CMotionControl::Reset(unsigned short nSegNumMBAddress,unsigned short nStartBitAddress)
{
	
	if (!m_Modbus.ModbusWriteOneDS(m_nPort,m_nBaudrate,nSegNumMBAddress,0))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
		
	if (!m_Modbus.ModbusWriteOneDS(m_nPort,m_nBaudrate,nStartBitAddress,0))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));

}

void CMotionControl::WriteSegNum(unsigned short nSegNumAddress)
{
	// m_nSegNum is 0 but in modbus is 1

		if (!m_Modbus.ModbusWriteOneDS(m_nPort,m_nBaudrate,nSegNumAddress,0))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));

		if (!m_Modbus.ModbusWriteOneDS(m_nPort,m_nBaudrate,nSegNumAddress,(short)m_nSegNum+1))
		AfxMessageBox((m_Modbus.modbusStatus.c_str()));
		

}
int CMotionControl::ReadSingleStep(unsigned short nCompleteBitAddress)
{
	
	unsigned short pollstart = nCompleteBitAddress;
	short value[1] ; //= new short[1];
    short captureMB[2]; // = new short[2];
			
	if (!m_Modbus.ModbusReadDSOneByOne(m_nPort,m_nBaudrate,nCompleteBitAddress,1,value))
		return 2;

	if( *value & (0x0001 << m_nSegNum))
	{
		return 0;
	}
	return 1;
}


int CMotionControl::MoveByTedFile(CString filename,float fspeed,float facc,float fdece,float fjerk)
{

	ResetAll();

	int i,j;
	int nLength = 0;
	
	FILE* fp;
	fp = fopen(filename.GetBuffer(filename.GetLength()), "r");
	fscanf(fp,"%d\n",&nLength);
	

	MOVEPOINTS *points = new MOVEPOINTS [nLength];
	 
    for( i=0;i<nLength;i++)
	{
		fscanf(fp,"%d %d %d %d %d\n",&points[i].id,&points[i].vect_id,&points[i].x,&points[i].y,&points[i].z);
	}
	fclose(fp);

	int xc = points[0].x;
	int yc = points[0].y;

	CString str;

	// write sequence num
	float *xcapture, *ycapture, *zvalues;
	xcapture = new float[nLength];
	ycapture = new float[nLength];
	zvalues = new float[nLength];
	
	int nSequenceNum = nLength/15 +1;
	
	for( i=0 ; i<nSequenceNum; i++ )
	{
		
		m_Modbus.ModbusWriteOneDS(m_nPort,m_nBaudrate,m_nSequenceAddr,i+1);
		int start = i*15;
		int end = (i+1)*15;
		if( end > nLength) end = nLength;
	    for (  j=start ;j <end ; j++)
		{
			
			
			float x = points[j].x/4.;
			float y = points[j].y/4.;
			float z = points[j].z;

			switch (points[j].vect_id)
			{
		
			case 2:

				if (!OneZSegMentMove((j%15)+1, fspeed, facc, fdece, z, fjerk )) {return 1;}

				if(points[j].x < xc) x = x - m_nMEASURE_OFFSET;
				if(points[j].x > xc) x = x + m_nMEASURE_OFFSET;
				x = x<0?0:x;
				if (!OneYSegMentMove((j%15)+1, fspeed, facc, fdece, y, fjerk)) {return 1;};

				m_Modbus.ModbusWriteOneDS(m_nPort,m_nBaudrate,m_nVecIDAddr,points[j].vect_id);

				if (!OneXSegMentMove((j%15)+1, fspeed, facc, fdece, x, fjerk)) {return 1;}
				break;
		
		
			case 1:
				OneZSegMentMove((j%15)+1, fspeed, facc, fdece, z, fjerk);


				if(points[j].y < yc) y = y - m_nMEASURE_OFFSET;
				if(points[j].y > yc) y = y + m_nMEASURE_OFFSET;
				y = y<0?0:y;
				if(!OneXSegMentMove((j%15)+1, fspeed, facc, fdece, x, fjerk)) {return 1;}

				m_Modbus.ModbusWriteOneDS(m_nPort,m_nBaudrate,m_nVecIDAddr,points[j].vect_id);
				if(!OneYSegMentMove( (j%15)+1, fspeed, facc, fdece, y, fjerk)){return 1;}
				break;
		
			default:
				if (!OneZSegMentMove((j%15)+1, fspeed, facc, fdece, z, fjerk)) {return 1;}


				if(!OneYSegMentMove((j%15)+1, fspeed, facc, fdece, y, fjerk)){return 1;}
				m_Modbus.ModbusWriteOneDS(m_nPort,m_nBaudrate,m_nVecIDAddr,points[j].vect_id);
				if(!OneXSegMentMove((j%15)+1, fspeed, facc, fdece, x, fjerk)){return 1;}
				
				}

		}

		// need to read all the caputre when sequecne is done
	

		m_Modbus.ModbusReadFloat(m_nPort,m_nBaudrate,m_nXCaptureAddr,end - start,xcapture+start);

		m_Modbus.ModbusReadFloat(m_nPort,m_nBaudrate,m_nYCaptureAddr,end - start,ycapture+start);

		m_Modbus.ModbusReadFloat(m_nPort,m_nBaudrate,m_nZAddr,end - start,zvalues+start);

	} // end of sequence 
	
					
		CString temp;
		CFile	myFile;
		
		m_message.Empty();

		char xyzfilename[200];
		sprintf(xyzfilename,"%s_XYZ.txt",filename.GetBuffer(filename.GetLength()-3));

        fp = fopen(xyzfilename,"w");
		CString t = CTime::GetCurrentTime().Format("%H:%M");
    	m_message.Append(t);
		m_message.Append("\n");
		for(  i=0;i<nLength;i++)
		{
			switch (points[i].vect_id)
			{
			case 1:
				temp.Format("P_id %d vec_id:1 Y:%3.2f z:%3.2f\n",points[i].id,ycapture[i],zvalues[i]);
		   		fprintf(fp, "%d %6.4f %6.4f %6.4f\n",points[i].id, -1.,ycapture[i],zvalues[i]);
				m_message.Append(temp);break;
			case 2:
				temp.Format("P_id %d vec_id:2 X:%3.2f z:%3.2f\n",points[i].id,xcapture[i],zvalues[i]);
				fprintf(fp, "%d %6.4f %6.4f %6.4f\n",points[i].id, xcapture[i],-1.,zvalues[i]);
				m_message.Append(temp);break;
			default:
				break;

			}
			
	
		}

		
			if ( myFile.Open( _T("capture_z.txt"), CFile::modeCreate | 	CFile::modeReadWrite | CFile::modeNoTruncate ) )
			{
				myFile.Write(m_message.GetBuffer(m_message.GetLength()),m_message.GetLength());
	

			}
			myFile.Close();
			fclose(fp);
		

	// reset sequence as 0
	OneZSegMentMove((j%15)+1, fspeed, facc, fdece, 6948, fjerk);

	OneXSegMentMove((j%15)+1, fspeed, facc, fdece, 0, fjerk);
	OneYSegMentMove((j%15)+1, fspeed, facc, fdece, 0, fjerk);
	m_Modbus.ModbusWriteOneDS(m_nPort,m_nBaudrate,m_nSequenceAddr,0);
	return 0;

}