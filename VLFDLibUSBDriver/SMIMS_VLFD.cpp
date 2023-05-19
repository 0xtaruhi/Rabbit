#include "pch.h"
#include "SMIMS_API.h"
#include "SMIMS_VLFD.h"
#include "ProgramVLFD.h"
#include "serialcheck.h"

using namespace std;

#define	DATA_PER_TIMES_WORD_DATA_SIZE  (512 * 4)

//---------------------------------------------------------------------------
// Global Variable
static USB_HANDLE hUSBDeviceHandle[ FUDAN_FPGA_NUM ] = {0};
//static USB_HANDLE hUSBDeviceHandle_Read[ FUDAN_FPGA_NUM ] = {0};
static SMIMS_CFGSpace CFGSpace[ FUDAN_FPGA_NUM ];
static WORD EncryptTable[ FUDAN_FPGA_NUM ][32];
static unsigned encindex[ FUDAN_FPGA_NUM ], decindex[ FUDAN_FPGA_NUM ];
//---------------------------------------------------------------------------
// Save Last Error Msg
#define	ERROR_MSG_LENGTH	512
static char ErrorMsg[ FUDAN_FPGA_NUM ][ ERROR_MSG_LENGTH ];
static void PrintErrorMsg(int iBoard, const char * cMsg);
//---------------------------------------------------------------------------

BOOL VLFD_ProgramFPGA(int iBoard, const char * BitFile)
{
	if ( iBoard >= FUDAN_FPGA_NUM )
	{
		PrintErrorMsg(iBoard, "Exceed max board count.");
		return FALSE;
	}

	if ( strlen(BitFile) != 0 )
    {
        TSMIMSVLFDProgrammer * m_Program;
		m_Program = new TSMIMSVLFDProgrammer();

		if ( m_Program->StartProgram(iBoard, BitFile) )
        {
		}
		else
		{
			PrintErrorMsg(iBoard, m_Program->GetLastErrorMsg());
			delete m_Program;
     		return FALSE;
        }
		delete m_Program;
		return TRUE;
	}
	else
	{
		PrintErrorMsg(iBoard, "Please check .bit filename.");
		return FALSE;
	}
	return TRUE;
}

BOOL VLFD_IO_ProgramFPGA(int iBoard, const char* BitFile)
{
	if (iBoard >= FUDAN_FPGA_NUM)
	{
		PrintErrorMsg(iBoard, "Exceed max board count.");
		return FALSE;
	}

	if (strlen(BitFile) != 0)
	{
		TSMIMSVLFDProgrammer* m_Program;
		m_Program = new TSMIMSVLFDProgrammer();

		if (m_Program->StartProgram(iBoard, BitFile))
		{
		}
		else
		{
			PrintErrorMsg(iBoard, m_Program->GetLastErrorMsg());
			delete m_Program;
			return FALSE;
		}
		delete m_Program;
		return TRUE;
	}
	else
	{
		PrintErrorMsg(iBoard, "Please check .bit filename.");
		return FALSE;
	}
	return TRUE;
}


BOOL VLFD_AppOpen(int iBoard, const char *  SerialNo)
{
	printf("Yu's Driver VLFD_AppOpen");
	//SB_DeviceDescriptor DevDscr;
	SMIMS_CFGSpace EncryptCFG;
	char cCID[5];
	WORD wCID;
	char VLFDName[10];

	memset(cCID, 0, 5);
	memset(VLFDName, 0, 10);

	if ( iBoard >= FUDAN_FPGA_NUM )
	{
		PrintErrorMsg(iBoard, "Exceed max board count.");
		return FALSE;
	}

	//Check If correct Serial Number
	if ( !CheckSerialNO(SerialNo, cCID) )
	{
		PrintErrorMsg(iBoard, "Illegal Serial Number!");
		return FALSE;
	}

	if ( hUSBDeviceHandle[ iBoard ] != NULL )
	{
		PrintErrorMsg(iBoard, "Already Open this Board!");
		return FALSE;
	}

	wCID = ConvertToWORD(cCID);

	hUSBDeviceHandle[ iBoard ] = SMIMS_DriverOpen("VLX2", iBoard);
	if(hUSBDeviceHandle[ iBoard ] == NULL)
	{
		sprintf(ErrorMsg[iBoard], "Can not open SMIMS FUDAN[%d] USB Device.", iBoard);
		return FALSE;
	}

	//======Read USB Device Descriptor======
	// if(!SMIMS_GetUSBDeviceDescriptor(hUSBDeviceHandle[ iBoard ], &DevDscr))
	// {
	// 		PrintErrorMsg(iBoard, "Can not Get SMIMS USB Device Descriptor.");
	// 		CloseHandle( hUSBDeviceHandle[ iBoard ] );
	// 		hUSBDeviceHandle[ iBoard ] = NULL;
	// 		return FALSE;
	// }
	// //======Check Vendor ID & Product ID=======
	// if((DevDscr.VendorID != DW_VID) || (DevDscr.ProductID != DW_PID))
	// {
	// 		PrintErrorMsg(iBoard, "This SMIMS USB Device can not use this SDK.");
	// 		CloseHandle( hUSBDeviceHandle[ iBoard ] );
	// 		hUSBDeviceHandle[ iBoard ] = NULL;
	// 		return FALSE;
	// }

	//= Read SMIMS Encrypt Table ========
	if(!SMIMS_EncryptTableRead(hUSBDeviceHandle[ iBoard ], EncryptTable[ iBoard ]))
	{
		PrintErrorMsg(iBoard, "Read Encrypt Table error.");
		SMIMS_DriverClose( hUSBDeviceHandle[ iBoard ] );
		hUSBDeviceHandle[ iBoard ] = NULL;
		return FALSE;
	}

	//= Decode Encrypt Table ================
	SMIMS_EncryptTableDecode(EncryptTable[ iBoard ], &encindex[ iBoard ], &decindex[ iBoard ]);

	//= Read SMIMS Engine Register File =========
	if(!SMIMS_CFGSpaceRead(hUSBDeviceHandle[ iBoard ], &EncryptCFG))
	{
		PrintErrorMsg(iBoard, "Read configuration space error.");
		SMIMS_DriverClose( hUSBDeviceHandle[ iBoard ] );
		hUSBDeviceHandle[ iBoard ] = NULL;
		return FALSE;
	}

	// Decrypt data
	SMIMS_DecryptCopy(CFGSpace[ iBoard ].CFG, EncryptCFG.CFG, sizeof(CFGSpace[ iBoard ]) / sizeof(WORD), EncryptTable[ iBoard ], &decindex[ iBoard ]);

	//= Check SMIMS Engine version =============================
	if(SMIMS_Version(&CFGSpace[ iBoard ]) < SMIMS_VERSION)
	{
		PrintErrorMsg(iBoard, "Version error.");
		SMIMS_DriverClose( hUSBDeviceHandle[ iBoard ] );
		hUSBDeviceHandle[ iBoard ] = NULL;
		return FALSE;
	}

	//= Check FPGA Program =======
	if( !SMIMS_IsFPGAProgram(&CFGSpace[ iBoard ]) )
	{
		PrintErrorMsg(iBoard, "FPGA does not program!");
		SMIMS_DriverClose( hUSBDeviceHandle[ iBoard ] );
		hUSBDeviceHandle[ iBoard ] = NULL;
		return FALSE;
	}

	//= Check AppMode Ability =======
	if( !SMIMS_VeriSDKAbility(&CFGSpace[ iBoard ]) )
	{
		PrintErrorMsg(iBoard, "Not support VeriSDK mode!");
		SMIMS_DriverClose( hUSBDeviceHandle[ iBoard ] );
		hUSBDeviceHandle[ iBoard ] = NULL;
		return FALSE;
	}

	//= Set License Key =============================
	WORD SecurityKey = SMIMS_GetSecurityKey(&CFGSpace[ iBoard ]);
	WORD LicenseKey = SMIMS_LicenseGen(SecurityKey, wCID);

	SMIMS_SetLicenseKey(&CFGSpace[ iBoard ], LicenseKey);

	SMIMS_SetVeriComm_ClockCheck(&CFGSpace[ iBoard ], FALSE);  // disable regular clock check
	SMIMS_SetModeSelector(&CFGSpace[ iBoard ], 0x01);          // Set SMIMS Engine I/O to AppSlaveFIFO16 Mode
	SMIMS_SetVeriSDK_ChannelSelector(&CFGSpace[ iBoard ], 0);    // Set VeriSDK Channel = 0

	// Encrypt data
	SMIMS_EncryptCopy(EncryptCFG.CFG, CFGSpace[ iBoard ].CFG, sizeof(CFGSpace[ iBoard ]) / sizeof(WORD), EncryptTable[ iBoard ], &encindex[ iBoard ]);

	//= Write back to SMIMS Engine Register File ====
	if(!SMIMS_CFGSpaceWrite(hUSBDeviceHandle[ iBoard ], &EncryptCFG))
	{
		PrintErrorMsg(iBoard, "Write configuration space error.");
		SMIMS_DriverClose( hUSBDeviceHandle[ iBoard ] );
		hUSBDeviceHandle[ iBoard ] = NULL;
		return FALSE;
	}

	//= Send command, Active Application Mode ====
	if(!SMIMS_VeriSDKActive( hUSBDeviceHandle[ iBoard ] ))
	{
		PrintErrorMsg(iBoard, "Send SMIMS_AppSlaveFIFO16Active error.");
		SMIMS_DriverClose( hUSBDeviceHandle[ iBoard ] );
		hUSBDeviceHandle[ iBoard ] = NULL;
		return FALSE;
	}

	//
	// Open Read USB Handle, use 2 USB handle to read and write
	//
	// hUSBDeviceHandle_Read[ iBoard ] = SMIMS_DriverOpen("VLX2", iBoard);
	// if(hUSBDeviceHandle_Read[ iBoard ] == NULL)
	// {
	// 	//PrintErrorMsg("Can not open SMIMS VLX2 USB Device hUSBDeviceHandle_Read.");
	// 	sprintf(ErrorMsg[iBoard] ,"Can not open SMIMS VLX2-%d USB Device  hUSBDeviceHandle_Read.", iBoard);
	// 	SMIMS_DriverClose( hUSBDeviceHandle[ iBoard ] );
	// 	hUSBDeviceHandle[ iBoard ] = NULL;
	// 	return FALSE;
	// }

	return TRUE;
}

BOOL VLFD_IO_Open(int iBoard, const char* SerialNo)
{
	SMIMS_CFGSpace EncryptCFG;
	char cCID[5];
	WORD wCID;
	char VLFDName[10];

	memset(cCID, 0, 5);
	memset(VLFDName, 0, 10);

	if (iBoard >= FUDAN_FPGA_NUM)
	{
		PrintErrorMsg(iBoard, "Exceed max board count.");
		return FALSE;
	}

	//Check If correct Serial Number
	if (!CheckSerialNO(SerialNo, cCID))
	{
		PrintErrorMsg(iBoard, "Illegal Serial Number!");
		return FALSE;
	}

	if (hUSBDeviceHandle[iBoard] != NULL)
	{
		PrintErrorMsg(iBoard, "Already Open this Board!");
		return FALSE;
	}

	wCID = ConvertToWORD(cCID);

	hUSBDeviceHandle[iBoard] = SMIMS_DriverOpen("VLX2", iBoard);
	if (hUSBDeviceHandle[iBoard] == NULL)
	{
		sprintf(ErrorMsg[iBoard], "Can not open SMIMS FUDAN[%d] USB Device.", iBoard);
		return FALSE;
	}

	//= Read SMIMS Encrypt Table ========
	if (!SMIMS_EncryptTableRead(hUSBDeviceHandle[iBoard], EncryptTable[iBoard]))
	{
		PrintErrorMsg(iBoard, "Read Encrypt Table error.");
		SMIMS_DriverClose(hUSBDeviceHandle[iBoard]);
		hUSBDeviceHandle[iBoard] = NULL;
		return FALSE;
	}

	//= Decode Encrypt Table ================
	SMIMS_EncryptTableDecode(EncryptTable[iBoard], &encindex[iBoard], &decindex[iBoard]);

	//= Read SMIMS Engine Register File =========
	if (!SMIMS_CFGSpaceRead(hUSBDeviceHandle[iBoard], &EncryptCFG))
	{
		PrintErrorMsg(iBoard, "Read configuration space error.");
		SMIMS_DriverClose(hUSBDeviceHandle[iBoard]);
		hUSBDeviceHandle[iBoard] = NULL;
		return FALSE;
	}

	// Decrypt data
	SMIMS_DecryptCopy(CFGSpace[iBoard].CFG, EncryptCFG.CFG, sizeof(CFGSpace[iBoard]) / sizeof(WORD), EncryptTable[iBoard], &decindex[iBoard]);

	//= Check SMIMS Engine version =============================
	if (SMIMS_Version(&CFGSpace[iBoard]) < SMIMS_VERSION)
	{
		PrintErrorMsg(iBoard, "Version error.");
		SMIMS_DriverClose(hUSBDeviceHandle[iBoard]);
		hUSBDeviceHandle[iBoard] = NULL;
		return FALSE;
	}

	//= Check FPGA Program =======
	if (!SMIMS_IsFPGAProgram(&CFGSpace[iBoard]))
	{
		PrintErrorMsg(iBoard, "FPGA does not program!");
		SMIMS_DriverClose(hUSBDeviceHandle[iBoard]);
		hUSBDeviceHandle[iBoard] = NULL;
		return FALSE;
	}

	//= Check VeriComm Mode Ability =======
	if (!SMIMS_VeriCommAbility(&CFGSpace[iBoard]))
	{
		PrintErrorMsg(iBoard, "Not support VeriComm mode!");
		SMIMS_DriverClose(hUSBDeviceHandle[iBoard]);
		hUSBDeviceHandle[iBoard] = NULL;
		return FALSE;
	}

	//= Set License Key =============================
	WORD SecurityKey = SMIMS_GetSecurityKey(&CFGSpace[iBoard]);
	WORD LicenseKey = SMIMS_LicenseGen(SecurityKey, wCID);

	SMIMS_SetLicenseKey(&CFGSpace[iBoard], LicenseKey);

	//=========================
	// VersiComm
	int clock_high_delay = 11;
	int clock_low_delay = 11;
	SMIMS_SetVeriComm_ClockHighDelay(&CFGSpace[iBoard], clock_high_delay);
	SMIMS_SetVeriComm_ClockLowDelay(&CFGSpace[iBoard], clock_low_delay);
	SMIMS_SetVeriComm_ISV(&CFGSpace[iBoard], 0);
	SMIMS_SetVeriComm_ClockCheck(&CFGSpace[iBoard], FALSE);  // disable regular clock check
	SMIMS_SetModeSelector(&CFGSpace[iBoard], 0);  // Set SMIMS Engine I/O to VeriComm Mode
	//=========================
	// SDK
	//SMIMS_SetVeriComm_ClockCheck(&CFGSpace[ iBoard ], FALSE);  // disable regular clock check
	//SMIMS_SetModeSelector(&CFGSpace[ iBoard ], 0x01);          // Set SMIMS Engine I/O to AppSlaveFIFO16 Mode
	//SMIMS_SetVeriSDK_ChannelSelector(&CFGSpace[ iBoard ], 0);    // Set VeriSDK Channel = 0
	//=========================

	// Encrypt data
	SMIMS_EncryptCopy(EncryptCFG.CFG, CFGSpace[iBoard].CFG, sizeof(CFGSpace[iBoard]) / sizeof(WORD), EncryptTable[iBoard], &encindex[iBoard]);

	//= Write back to SMIMS Engine Register File ====
	if (!SMIMS_CFGSpaceWrite(hUSBDeviceHandle[iBoard], &EncryptCFG))
	{
		PrintErrorMsg(iBoard, "Write configuration space error.");
		SMIMS_DriverClose(hUSBDeviceHandle[iBoard]);
		hUSBDeviceHandle[iBoard] = NULL;
		return FALSE;
	}

	// Send command, Active VeriComm module
	if (!SMIMS_VeriCommActive(hUSBDeviceHandle[iBoard]))
	{
		PrintErrorMsg(iBoard, "Can not Active VeriComm module.");
		SMIMS_DriverClose(hUSBDeviceHandle[iBoard]);
		hUSBDeviceHandle[iBoard] = NULL;
		return FALSE;
	}

	return TRUE;
}

BOOL VLFD_AppFIFOReadData(int iBoard, WORD *Buffer, unsigned size)
{
	printf("Yu's Driver VLFD_AppFIFOReadData");
	WORD EncryptBuffer[ 4096 ];
	WORD * pEncryptBuffer;
	bool bNeedRelease;

	if ( iBoard >= FUDAN_FPGA_NUM )
	{
		PrintErrorMsg(iBoard,"Exceed max board count.");
		return FALSE;
	}

	//if ( hUSBDeviceHandle_Read[ iBoard ] == NULL )
	if ( hUSBDeviceHandle[ iBoard ] == NULL )
	{
		PrintErrorMsg(iBoard, "SMIMS VLX2 USB is not initialed hUSBDeviceHandle.");
		return FALSE;
	}

	if ( size > 4096)
	{
		bNeedRelease = true;
		pEncryptBuffer = new WORD[size];
	}
	else
	{
		bNeedRelease = false;
		pEncryptBuffer = EncryptBuffer;
	}

	//if ( !SMIMS_FIFO_Read(hUSBDeviceHandle_Read[ iBoard ], EncryptBuffer, size) )
	if ( !SMIMS_FIFO_Read(hUSBDeviceHandle[ iBoard ], pEncryptBuffer, size) )
	{
		if ( bNeedRelease )
			delete[] pEncryptBuffer;
		return false;
	}
	else
	{
		// Decrypt data
		SMIMS_DecryptCopy(Buffer, pEncryptBuffer, size, EncryptTable[ iBoard ], &decindex[ iBoard ]);

		if ( bNeedRelease )
			delete[] pEncryptBuffer;
		return TRUE;
	}
}

BOOL VLFD_AppFIFOWriteData(int iBoard, WORD *Buffer, unsigned size)
{
	//printf("Yu's Driver VLFD_AppFIFOWriteData");
	WORD EncryptBuffer[ 4096 ];
	WORD * pEncryptBuffer;
	bool bNeedRelease;

	if ( iBoard >= FUDAN_FPGA_NUM )
	{
		PrintErrorMsg(iBoard, "Exceed max board count.");
		return FALSE;
	}

	if ( hUSBDeviceHandle[ iBoard ] == NULL )
	{
		PrintErrorMsg(iBoard, "SMIMS VLX2 USB is not initialed.");
		return FALSE;
	}

	if ( size > 4096)
	{
		bNeedRelease = true;
		pEncryptBuffer = new WORD[size];
	}
	else
	{
		bNeedRelease = false;
		pEncryptBuffer = EncryptBuffer;
	}

	// Encrypt data
	SMIMS_EncryptCopy(pEncryptBuffer, Buffer, size, EncryptTable[ iBoard ], &encindex[ iBoard ]);

	if ( !SMIMS_FIFO_Write(hUSBDeviceHandle[ iBoard ], pEncryptBuffer, size) )
	{
		if ( bNeedRelease )
			delete[] pEncryptBuffer;
		return false;
	}
	else
	{
		if ( bNeedRelease )
			delete[] pEncryptBuffer;
		return true;
	}
}

BOOL VLFD_IO_WriteReadData(int iBoard, WORD* WriteBuffer, WORD* ReadBuffer, unsigned size)
{
	WORD EncryptBuffer[8192];
	int iNowPos = 0;

	if (iBoard >= FUDAN_FPGA_NUM)
	{
		PrintErrorMsg(iBoard, "Exceed max board count.");
		return FALSE;
	}

	//if ( hUSBDeviceHandle_Read[ iBoard ] == NULL )
	if (hUSBDeviceHandle[iBoard] == NULL)
	{
		PrintErrorMsg(iBoard, "SMIMS Fudan USB is not initialed hUSBDeviceHandle.");
		return FALSE;
	}

	if (size > DATA_PER_TIMES_WORD_DATA_SIZE)
	{
		char cMsg[128];
		sprintf(cMsg, "SMIMS Fudan USB (size is overflow)(size=%d)", size);
		PrintErrorMsg(iBoard, cMsg);
		return false;
	}

	// Encrypt data
	SMIMS_EncryptCopy(EncryptBuffer, WriteBuffer, size, EncryptTable[iNowPos], &encindex[iNowPos]);

	// Send first VeriComm data
	if (!SMIMS_FIFO_Write(hUSBDeviceHandle[iBoard], EncryptBuffer, size))
	{
		PrintErrorMsg(iBoard, "Send fifo data error.");
		return false;
	}

	// Receive VeriComm data
	if (!SMIMS_FIFO_Read(hUSBDeviceHandle[iBoard], EncryptBuffer, size))
	{
		PrintErrorMsg(iBoard, "Receive fifo data error.");
		return false;
	}

	// Decrypt data
	SMIMS_DecryptCopy(ReadBuffer, EncryptBuffer, size, EncryptTable[iNowPos], &decindex[iNowPos]);

	return true;
}

BOOL VLFD_AppChannelSelector(int iBoard, BYTE channel)
{
	printf("Yu's Driver VLFD_AppChannelSelector");
	SMIMS_CFGSpace EncryptCFG;

	if ( iBoard >= FUDAN_FPGA_NUM )
	{
		PrintErrorMsg(iBoard, "Exceed max board count.");
		return FALSE;
	}

	if ( hUSBDeviceHandle[ iBoard ] == NULL )
	{
		PrintErrorMsg(iBoard, "SMIMS VLX2 USB is not initialed.");
		return FALSE;
	}

	// 結束 Application Mode, 將 VLX2 設定為 “命令模式”
	if(!SMIMS_CommandActive( hUSBDeviceHandle[ iBoard ] ))
	{
		PrintErrorMsg(iBoard, "Send SMIMS_CommandActive error.");
		return FALSE;
	}

	//= Write to CFG Space ========================
	SMIMS_SetVeriComm_ClockCheck(&CFGSpace[ iBoard ], FALSE);  // disable regular clock check
	SMIMS_SetModeSelector(&CFGSpace[ iBoard ], 0x01);          // Set SMIMS Engine I/O to AppSlaveFIFO16 Mode
	SMIMS_SetVeriSDK_ChannelSelector(&CFGSpace[ iBoard ], channel);    // Set AppChannel = channel

	// Encrypt data
	SMIMS_EncryptCopy(EncryptCFG.CFG, CFGSpace[ iBoard ].CFG, sizeof(CFGSpace[ iBoard ]) / sizeof(WORD), EncryptTable[ iBoard ], &encindex[ iBoard ]);

	if(!SMIMS_CFGSpaceWrite(hUSBDeviceHandle[ iBoard ], &EncryptCFG))
	{
		PrintErrorMsg(iBoard, "Write VLX2 configuration space error.");
		return FALSE;
	}

	//= Send command, Active SMIMS_VeriSDKActive module =============
	if(!SMIMS_VeriSDKActive(hUSBDeviceHandle[ iBoard ]))
	{
		PrintErrorMsg(iBoard, "Send VLX2 command error.");
		return FALSE;
	}

	return TRUE;
}
BOOL VLFD_AppClose(int iBoard)
{
	printf("Yu's Driver VLFD_AppClose");
	if ( iBoard >= FUDAN_FPGA_NUM )
	{
		PrintErrorMsg(iBoard, "Exceed max board count.");
		return FALSE;
	}

	if ( hUSBDeviceHandle[ iBoard ] == NULL )
		return TRUE;

	// 結束 Application Mode, 將 VeriEnterprise 設定為 “命令模式”  (Note 2)
	if(!SMIMS_CommandActive( hUSBDeviceHandle[ iBoard ] ))
	{
		PrintErrorMsg(iBoard, "Send SMIMS_CommandActive error.");
		SMIMS_DriverClose( hUSBDeviceHandle[ iBoard ] );
		//SMIMS_DriverClose( hUSBDeviceHandle_Read[ iBoard ] );
		hUSBDeviceHandle[ iBoard ] = NULL;
		//hUSBDeviceHandle_Read[ iBoard ] = NULL;
		return FALSE;
	}

	if ( SMIMS_DriverClose( hUSBDeviceHandle[ iBoard ] ) == 0 )
	{
		PrintErrorMsg(iBoard, "Can not close USB Handle");
		hUSBDeviceHandle[ iBoard ] = NULL;
		return FALSE;
	}

	// if ( SMIMS_DriverClose( hUSBDeviceHandle_Read[ iBoard ] ) == 0 )
	// {
	// 	PrintErrorMsg(iBoard, "Can not close USB Handle_Read");
	// 	hUSBDeviceHandle_Read[ iBoard ] = NULL;
	// 	return FALSE;
	// }

	hUSBDeviceHandle[ iBoard ] = NULL;
	//hUSBDeviceHandle_Read[ iBoard ] = NULL;
	return TRUE;
}


BOOL VLFD_IO_Close(int iBoard)
{
	if (iBoard >= FUDAN_FPGA_NUM)
	{
		PrintErrorMsg(iBoard, "Exceed max board count.");
		return FALSE;
	}

	if (hUSBDeviceHandle[iBoard] == NULL)
		return TRUE;

	// 結束 Application Mode, 將 VeriEnterprise 設定為 “命令模式”  (Note 2)
	if (!SMIMS_CommandActive(hUSBDeviceHandle[iBoard]))
	{
		PrintErrorMsg(iBoard, "Send SMIMS_CommandActive error.");
		SMIMS_DriverClose(hUSBDeviceHandle[iBoard]);
		hUSBDeviceHandle[iBoard] = NULL;
		return FALSE;
	}

	if (SMIMS_DriverClose(hUSBDeviceHandle[iBoard]) == 0)
	{
		PrintErrorMsg(iBoard, "Can not close USB Handle");
		hUSBDeviceHandle[iBoard] = NULL;
		return FALSE;
	}

	hUSBDeviceHandle[iBoard] = NULL;
	return TRUE;
}

//---------------------------------------------------------------------------
//Name: GetLastErrorMsg
//description: GetLastErrorMsg
//parameter:
//      int iBoard:
//return:
//      char *:
//---------------------------------------------------------------------------
const char * VLFD_GetLastErrorMsg(int iBoard)
{
	if ( iBoard >= FUDAN_FPGA_NUM )
	{
		return "Exceed max board count.";
	}

	return ErrorMsg[iBoard];
}


const char* VLFD_IO_GetLastErrorMsg(int iBoard)
{
	if (iBoard >= FUDAN_FPGA_NUM)
	{
		return "Exceed max board count.";
	}

	return ErrorMsg[iBoard];
}

//---------------------------------------------------------------------------
//Name: PrintErrorMsg
//description: PrintErrorMsg
//parameter:
//		int iBoard:
//      char *:
//return:
//      void:
//---------------------------------------------------------------------------
void PrintErrorMsg(int iBoard, const char * cMsg)
{
	if ( iBoard >= FUDAN_FPGA_NUM )
	{
		return;
	}

	sprintf(ErrorMsg[iBoard],"%s", cMsg);
}
