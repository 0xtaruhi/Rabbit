//---------------------------------------------------------------------
// FILE: ProgramVLFD.cpp
// AUTHOR: Yi-Huei Chung
// ABSTRATE:
// FUNCTION:
// MODIFICATION HISTORY:
//	2010.05.3 Create File. For Fudan FPGA board
//
//  (c) copyright 2005 SMIMS Technology Corp. All rights reserved
//---------------------------------------------------------------------
#include "pch.h"
#include "ProgramVLFD.h"

using namespace std;
//---------------------------------------------------------------------------
//Name:TSMIMSVLFDProgrammer
//description: TSMIMSVLFDProgrammer
//parameter:
//	 void:
//return:
//       NONE
//---------------------------------------------------------------------------
TSMIMSVLFDProgrammer::TSMIMSVLFDProgrammer(void)
{
	memset(ErrorMsg, 0, 128);
}
//---------------------------------------------------------------------------
//Name:StartProgram
//description: StartProgram
//parameter:
//      int iBoard:
//		const char * g_szBitFile1:
//return:
//      bool
//---------------------------------------------------------------------------
bool TSMIMSVLFDProgrammer::StartProgram(int iBoard, const char* g_szBitFile1)
{

//#ifdef	_DEBUG
//	char cmsg[512];
//	sprintf(cmsg, "[SMIMS]Pgm_VLXFD.dll[StartProgram][iBoard=%d]", iBoard);
//	OutputDebugString(cmsg);
//	sprintf(cmsg, "[SMIMS]Pgm_VLXFD.dll[StartProgram][g_szBitFile1=%s]", g_szBitFile1);
//	OutputDebugString(cmsg);
//#endif
	if (strlen(g_szBitFile1) != 0)
	{
		ProgramFPGA(iBoard, g_szBitFile1);
	}

	if (g_iErrCode != _SUCCESS_)
	{
		g_iProgress = -1;

		sprintf(ErrorMsg, "FPGA program failed! %s", errorstr[g_iErrCode].c_str());
		//�������һ�£�����errorstr��Ϣ

		return false;
	}

//#ifdef	_DEBUG
//	sprintf(cmsg, "[SMIMS]Pgm_VLXFD.dll[StartProgram][return true][g_iProgress = %d]", g_iProgress);
//	OutputDebugString(cmsg);
//#endif

	g_iProgress = 101;

	return true;
}
//---------------------------------------------------------------------------
//Name:GetLastErrorMsg
//description: GetLastErrorMsg
//parameter:
//      void:
//return:
//      char *
//---------------------------------------------------------------------------
char* TSMIMSVLFDProgrammer::GetLastErrorMsg(void)
{
	return ErrorMsg;
}

//---------------------------------------------------------------------------
//Name:ProgramFPGA
//description: ProgramFPGA  for Fudan FPGA board
//Date: 2009.07.23
//parameter:
//		const char * g_szBitFile:
//return:
//      int
//---------------------------------------------------------------------------
int TSMIMSVLFDProgrammer::ProgramFPGA(int iBoard, const char* g_szBitFile)
{
	hDevice = NULL;
	Buffer = NULL;

	//sprintf(g_szUSBDev,"VLX2-%d", iBoard);

	fi.open(g_szBitFile, ios::in | ios::binary | ios::ate);

	g_iErrCode = _SUCCESS_;
	g_iProgress = 0;

	if (!fi)
	{
		g_iErrCode = _FILE_OPEN_ERROR_;
		return 1;
	}

	BSLen = fi.tellg();
	fi.seekg(0);

	hDevice = SMIMS_DriverOpen("VLX2", iBoard);
	if (hDevice == NULL)
	{
		g_iErrCode = _USB_DRIVER_OPEN_ERROR_;
		return 1;
	}

	//= Read SMIMS Encrypt Table ========
	if (!SMIMS_EncryptTableRead(hDevice, EncryptTable))
	{
		//cout << "Read Encrypt Table error." << endl;
		//CloseHandle(hDevice);
		//return 1;
		g_iErrCode = _READ_ENCRYPT_TABLE_ERROR_;
		return 1;
	}
	//= Decode Encrypt Table ================
	SMIMS_EncryptTableDecode(EncryptTable, &encindex, &decindex);

	//printf("TSMIMSVLFDProgrammer::ProgramFPGA() SMIMS_EncryptTableDecode ok \n");

	//= Read SMIMS Engine CFG Space =============================
	if (!SMIMS_CFGSpaceRead(hDevice, &EncryptCFG))
	{
		g_iErrCode = _READ_CFG_DATA_ERROR_;
		return 1;
	}

	//printf("TSMIMSVLFDProgrammer::ProgramFPGA() SMIMS_CFGSpaceRead ok \n");

	SMIMS_DecryptCopy(CFGSpace.CFG, EncryptCFG.CFG, sizeof(CFGSpace) / sizeof(uint16_t), EncryptTable, &decindex);

	//= Check SMIMS Engine version =============================
	if (SMIMS_Version(&CFGSpace) < SMIMS_VERSION)
	{
		g_iErrCode = _VERSION_ERROR_;
		return 1;
	}

	//printf("TSMIMSVLFDProgrammer::ProgramFPGA() SMIMS_Version ok \n");

	//== Allocate memory ======================================

	BlockSize = SMIMS_GetFIFOSize(&CFGSpace);

	//printf("TSMIMSVLFDProgrammer::ProgramFPGA() BlockSize:%d\n", BlockSize);

	Buffer = new uint16_t[BlockSize];

	if (Buffer == NULL)
	{
		g_iErrCode = _MEMORY_ALLOCATE_ERROR_;
		return 1;
	}

	//== Xininx Programmer begin ============================

	if (!SMIMS_FPGAProgrammerActive(hDevice))
	{
		g_iErrCode = _SEND_CMD_ERROR_;
		return 1;
	}

	j = BlockSize * 2;

	unsigned i = 0;
	unsigned th = 0;
	unsigned tl = 0;
	char ctemp = 0;
	while (!fi.eof())
	{
		while (!fi.eof())
		{
			fi.read(&ctemp, 1);
			if (ctemp >= 0x30 && ctemp <= 0x39)
				ctemp -= 0x30;
			else if (ctemp >= 0x61 && ctemp <= 0x66)
				ctemp -= 0x57;
			else if (ctemp >= 0x41 && ctemp <= 0x46)
				ctemp -= 0x37;
			else
				break;
			th = (th << 4) | ctemp;
		}

		if (ctemp != '_')
			break;

		while (!fi.eof())
		{
			fi.read(&ctemp, 1);

			if (ctemp >= 0x30 && ctemp <= 0x39)
				ctemp -= 0x30;
			else if (ctemp >= 0x61 && ctemp <= 0x66)
				ctemp -= 0x57;
			else if (ctemp >= 0x41 && ctemp <= 0x46)
				ctemp -= 0x37;
			else
				break;
			tl = (tl << 4) | ctemp;
		}

		memcpy((char*)Buffer + i, (char*)&th, 2);
		memcpy((char*)Buffer + i + 2, (char*)&tl, 2);

		while (!fi.eof())
		{
			fi.read(&ctemp, 1);
			if (ctemp == 0x0a)
				break;
		}

		i += 4;

		//printf("TSMIMSVLFDProgrammer::ProgramFPGA() i:%d\n", i);

		if (i >= j)
		{
			SMIMS_EncryptData(Buffer, BlockSize, EncryptTable, &encindex);
			if (!SMIMS_FIFO_Write(hDevice, Buffer, BlockSize))  // Send Programmer data
			{
				g_iErrCode = _SEND_DATA_ERROR_;
				delete[] Buffer;
				fi.close();
				return g_iErrCode;
			}
			i = 0;
		}
	}

	if (i > 0)
	{
		SMIMS_EncryptData(Buffer, i / 2, EncryptTable, &encindex);
		if (!SMIMS_FIFO_Write(hDevice, Buffer, i / 2))  // Send Programmer data
		{
			g_iErrCode = _SEND_DATA_ERROR_;
			delete[] Buffer;
			fi.close();
			return g_iErrCode;
		}
	}

	if (!SMIMS_CommandActive(hDevice))  // XilinxProgrammer module finish
	{
		g_iErrCode = _SEND_CMD_ERROR_;
		return 1;
	}
	//= Read SMIMS Engine CFG Space =============================
	if (!SMIMS_CFGSpaceRead(hDevice, &EncryptCFG))
	{
		g_iErrCode = _READ_CFG_DATA_ERROR_;
		return 1;

	}

	SMIMS_DecryptCopy(CFGSpace.CFG, EncryptCFG.CFG, sizeof(CFGSpace) / sizeof(uint16_t), EncryptTable, &decindex);

	//= Check Xilinx FPGA download success =======
	if (SMIMS_IsFPGAProgram(&CFGSpace))
	{
		g_iProgress = 101;
		//printf("TSMIMSVLFDProgrammer::ProgramFPGA() FPGA does program ok.\n");
	}
	else
	{
		g_iErrCode = _PROGRAM_FIRST_FPGA_FAILED_;
		g_iProgress = -1;
		printf("TSMIMSVLFDProgrammer::ProgramFPGA() FPGA does not program!\n");
	}

	fi.close();

	if (Buffer)
		delete[] Buffer;
	if (hDevice)
		SMIMS_DriverClose(hDevice);

	if (g_iErrCode != _SUCCESS_)
	{
		g_iProgress = -1;

		sprintf(ErrorMsg, "%s", errorstr[g_iErrCode].c_str());

		return 1;
	}

	return 0;
}
