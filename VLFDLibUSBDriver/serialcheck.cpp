#include "pch.h"
#include "SMIMS_API.h"
#include "serialcheck.h"


static char Table1[16] = {'Z','Q','W','R','T','Y','U','I','P','L','A','B','C','D','E','F'};
static char Table2[16] = {'K','J','H','G','F','D','S','A','Z','X','C','V','B','N','M','L'};
static char Table3[16] = {'G','H','Y','T','R','F','V','B','N','J','U','I','K','S','X','O'};
//---------------------------------------------------------------------------
//Name: CheckSerialNO
//description: CheckSerialNO
//parameter:
//      char * SerialNO:
//		char * cCID:
//return:
//      bool:
//---------------------------------------------------------------------------
bool CheckSerialNO(const char * SerialNO, char * cCID)
{
	char cSerialNO_1[5];
	char cSerialNO_2[5];
	char cSerialNO_3[5];
	char cSerialNO_4[5];
	char cPID[5];
	char * cTemp;
	int iSerialLen;
	char cLocalSerialNO[21];

	//Copy serial number to temp variable before parsing
	memset(cSerialNO_1, 0, 5);
	memset(cSerialNO_2, 0, 5);
	memset(cSerialNO_3, 0, 5);
	memset(cSerialNO_4, 0, 5);
	memset(cPID, 0, 5);
	memset(cLocalSerialNO, 0, 21);

	iSerialLen = (int)strlen(SerialNO);
	if ( iSerialLen != 29 )
	{
		return false;
	}

	strncpy(cSerialNO_1, SerialNO, 4);

	cTemp = strstr((char *)SerialNO, "-");
	if ( cTemp == NULL )
	{
		return false;
	}

	strncpy(cSerialNO_2, cTemp+1, 4);

	cTemp = strstr(cTemp+1, "-");
	if ( cTemp == NULL )
	{
		return false;
	}

	strncpy(cSerialNO_3, cTemp+1, 4);

	cTemp = strstr(cTemp+1, "-");
	if ( cTemp == NULL )
	{
		return false;
	}

	strncpy(cSerialNO_4, cTemp+1, 4);

	cTemp = strstr(cTemp+1, "-");
	if ( cTemp == NULL )
	{
		return false;
	}

	strncpy(cCID, cTemp+1, 4);

	cTemp = strstr(cTemp+1, "-");
	if ( cTemp == NULL )
	{
		return false;
	}

	strncpy(cPID, cTemp+1, 4);

	sprintf(cLocalSerialNO, "%s%s%s%s",cSerialNO_1,cSerialNO_2,cSerialNO_3,cSerialNO_4);

	return ValidateSN(cLocalSerialNO);
}
//---------------------------------------------------------------------------
//Name: ValidateSN
//description: ValidateSN
//parameter:
//      char * SerialNO:
//return:
//      bool:
//---------------------------------------------------------------------------
bool ValidateSN(const char * cSerialNO)
{
        //檢查軟體版本
        if ( !CheckSoftwareVersion(cSerialNO) )
        {
            return false;
        }

        //檢查產品
        if ( !CheckProduct(cSerialNO) )
        {
            return false;
        }

        //檢查年份
        if ( !CheckYear(cSerialNO) )
        {
            return false;
        }

        //檢查月份
        if ( !CheckMonth(cSerialNO) )
        {
            return false;
        }

        //檢查序號
        if ( !CheckSerialNO(cSerialNO) )
        {
            return false;
        }

        //檢查加密
        if ( !CheckEncrypt(cSerialNO) )
        {
            return false;
        }

		return true;
}
//---------------------------------------------------------------------------
//Name: CheckSoftwareVersion
//description: CheckSoftwareVersion
//parameter:
//	 char * cSerialNO:
//return:
//       bool
//---------------------------------------------------------------------------
bool CheckSoftwareVersion(const char * cSerialNO)
{
        char cNowChar;

        cNowChar = cSerialNO[1];
        if ( !( cNowChar >= '0' && cNowChar <= '9') )
                return false;
        cNowChar = cSerialNO[5];
        if ( !( cNowChar >= '0' && cNowChar <= '9') )
                return false;
        cNowChar = cSerialNO[9];
        if ( !( cNowChar >= '0' && cNowChar <= '9') )
                return false;
        cNowChar = cSerialNO[11];
        if ( !( cNowChar >= '0' && cNowChar <= '9') )
                return false;

        return true;
}
//---------------------------------------------------------------------------
//Name: CheckProduct
//description: CheckProduct
//parameter:
//	 char * cSerialNO:
//return:
//       bool
//---------------------------------------------------------------------------
bool CheckProduct(const char * cSerialNO)
{
        char cPostMsg[128];
        int iCheckOK = 0;
        switch(cSerialNO[ 0 ])
        {
        case 'C':
                iCheckOK = 1;
                sprintf(cPostMsg,"Product : VeriLite PCI C3");
        break;
        case 'D':
                iCheckOK = 1;
                sprintf(cPostMsg,"Product : VeriLite PCI C6");
        break;
        case 'M':
                iCheckOK = 1;
                sprintf(cPostMsg,"Product : VeriLite PCI(For update)");
        break;
        case 'U':
                iCheckOK = 1;
                sprintf(cPostMsg,"Product : VeriLite USB C12");
        break;
        case 'Q':
                iCheckOK = 1;
                sprintf(cPostMsg,"Product : VeriLite USB(For update)");
        break;
        case 'P':
                iCheckOK = 1;
                sprintf(cPostMsg,"Product : VeriLite Xilinx USB XC3S250E");
        break;
        case 'R':
                iCheckOK = 1;
                sprintf(cPostMsg,"Product : VeriLite Xilinx USB XC3S500E");
        break;
        case 'S':
                iCheckOK = 1;
                sprintf(cPostMsg,"Product : VeriEnterprise S80");
        break;
        case 'T':
                iCheckOK = 1;
                sprintf(cPostMsg,"Product : VeriEnterprise Altera S60");
        break;
        case 'V':
                iCheckOK = 1;
                sprintf(cPostMsg,"Product : VeriEnterprise Xilinx VIRTEX-4 XC4VLX60");
        break;
        case 'W':
                iCheckOK = 1;
                sprintf(cPostMsg,"Product : VeriEnterprise Xilinx VIRTEX-4 XC4VLX160");
        break;
        case 'Z':
                iCheckOK = 1;
                sprintf(cPostMsg,"Product : VeriEnterprise(For update)");
        break;
        case 'A':
				iCheckOK = 1;
				sprintf(cPostMsg,"Product : VeriEnterprise Xilinx XC5VLX110");
		break;
		case 'B':
				iCheckOK = 1;
				sprintf(cPostMsg,"Product : VeriEnterprise Xilinx XC5VLX330");
		break;
		case 'E':
				iCheckOK = 1;
				sprintf(cPostMsg,"Product : VeriEnterprise Xilinx XC5VLX330-Dual");
		break;
		case 'F':
				iCheckOK = 1;
				sprintf(cPostMsg,"Product : VeriLite Xilinx Fudan FDP3P7");
		break;
        default:
                iCheckOK = 0;
        break;
        }
        if ( iCheckOK == 1 )
        {
                return true;
        }
        else
                return false;
}
//---------------------------------------------------------------------------
//Name: CheckYear
//description: CheckYear
//parameter:
//	 char * cSerialNO:
//return:
//       bool
//---------------------------------------------------------------------------
bool CheckYear(const char * cSerialNO)
{
        char cNowChar = cSerialNO[12];

        if ( (cNowChar >= '0' && cNowChar <= '9') ||
             (cNowChar >= 'A' && cNowChar <= 'Z') )
                return true;
        else
                return false;
}
//---------------------------------------------------------------------------
//Name: CheckMonth
//description: CheckMonth
//parameter:
//	 char * cSerialNO:
//return:
//       bool
//---------------------------------------------------------------------------
bool CheckMonth(const char * cSerialNO)
{
        char cNowChar = cSerialNO[15];

        if ( (cNowChar >= '0' && cNowChar <= '9') ||
             (cNowChar >= 'A' && cNowChar <= 'C') )
                return true;
        else
                return false;
}
//---------------------------------------------------------------------------
//Name: CheckSerialNO
//description: CheckSerialNO
//parameter:
//	 char * cSerialNO:
//return:
//       bool
//---------------------------------------------------------------------------
bool CheckSerialNO(const char * cSerialNO)
{
        char cNowChar;

        cNowChar = cSerialNO[14];
        if ( !((cNowChar >= '0' && cNowChar <= '9') ||
               (cNowChar >= 'A' && cNowChar <= 'Z')) )
                return false;
        cNowChar = cSerialNO[7];
        if ( !((cNowChar >= '0' && cNowChar <= '9') ||
               (cNowChar >= 'A' && cNowChar <= 'Z')) )
                return false;
        cNowChar = cSerialNO[3];
        if ( !((cNowChar >= '0' && cNowChar <= '9') ||
               (cNowChar >= 'A' && cNowChar <= 'Z')) )
                return false;
        return true;
}
//---------------------------------------------------------------------------
//Name: CheckSerialNO
//description: CheckSerialNO
//parameter:
//	 char * cSerialNO:
//return:
//       bool
//---------------------------------------------------------------------------
bool CheckEncrypt(const char * cSerialNO)
{
        char cHexValue[7], cIntValue_r[8], cIntValue[8];
        char cTrans[9];
        char cGetCh;
        int iValue;
        char cValue[9];

        memset(cHexValue, 0, 7);
        cGetCh = GetTable(cSerialNO[2], Table1);
        if ( cGetCh == '!' ) return false;
        cHexValue[ 0 ] = cGetCh;

        cGetCh = GetTable(cSerialNO[4], Table2);
        if ( cGetCh == '!' ) return false;
        cHexValue[ 1 ] = cGetCh;

        cGetCh = GetTable(cSerialNO[6], Table3);
        if ( cGetCh == '!' ) return false;
        cHexValue[ 2 ] = cGetCh;

        cGetCh = GetTable(cSerialNO[8], Table3);
        if ( cGetCh == '!' ) return false;
        cHexValue[ 3 ] = cGetCh;

        cGetCh = GetTable(cSerialNO[10], Table2);
        if ( cGetCh == '!' ) return false;
        cHexValue[ 4 ] = cGetCh;

        cGetCh = GetTable(cSerialNO[13], Table1);
        if ( cGetCh == '!' ) return false;
        cHexValue[ 5 ] = cGetCh;

        //加上 '$' 可以計算 Hex
        //sprintf(cTrans,"$%s",cHexValue);
        sprintf(cTrans,"%s",cHexValue);



        memset(cIntValue_r, 0, 8);
        sprintf(cIntValue_r,"%07d", HexToInt(cTrans));

        memset(cIntValue, 0, 8);

        cIntValue[ 2 ] = cIntValue_r[ 0 ];
        cIntValue[ 3 ] = cIntValue_r[ 1 ];
        cIntValue[ 6 ] = cIntValue_r[ 2 ];
        cIntValue[ 5 ] = cIntValue_r[ 3 ];
        cIntValue[ 0 ] = cIntValue_r[ 4 ];
        cIntValue[ 1 ] = cIntValue_r[ 5 ];
        cIntValue[ 4 ] = cIntValue_r[ 6 ];

        iValue = GetIntValue( &cSerialNO[14] )
               +  GetIntValue( &cSerialNO[7] ) * 36
               +  GetIntValue( &cSerialNO[3] ) * 36 * 36
               +  GetIntValue( &cSerialNO[15] ) * 36 * 36 * 36
               +  GetIntValue( &cSerialNO[12] ) * 36 * 36 * 36 * 12;

        memset(cValue, 0, 8);
        sprintf(cValue,"%07d", iValue);

        if ( strncmp(cIntValue, cValue, 7) == 0 )
                return true;
        else
                return false;
}
//---------------------------------------------------------------------------
//Name: GetTable
//description: GetTable
//parameter:
//	 char cValue:
//return:
//       char
//---------------------------------------------------------------------------
char GetTable(char cValue, const char * Table)
{
        int i;
        int iFound = 0;

        for(i=0; i<16; i++)
        {
                if ( cValue == Table[ i ] )
                {
                        iFound = 1;
                        break;
                }
        }
        if ( iFound == 0 )
                return '!';
        else
                return IntToChar(i);
}
//---------------------------------------------------------------------------
//Name: GetIntValue
//description: GetIntValue
//parameter:
//	 char * cValue:
//return:
//       int
//---------------------------------------------------------------------------
int GetIntValue(const char * cValue)
{
        if ( cValue[0] >= '0' && cValue[0] <= '9')
        {
                return cValue[0] - '0';
        }
        else
        if ( cValue[0] >= 'A' && cValue[0] <= 'Z')
        {
                return cValue[0] - 'A' + 10;
        }
        else
                return 0;
}
//---------------------------------------------------------------------------
//Name: IntToChar
//description: IntToChar
//parameter:
//	 int iValue:
//return:
//       char
//---------------------------------------------------------------------------
char IntToChar(int iValue)
{
        if ( iValue >= 0 && iValue <= 9)
        {
                return iValue + '0';
        }
        else
        if ( iValue >= 10 && iValue <= 36)
        {
                return iValue + 'A' - 10;
        }
        else
                return '0';
}

//---------------------------------------------------------------------------
//Name:ConvertToWORD
//description: ConvertToWORD
//parameter:
//	 	char * Orig_char:
//return:
//      uint16_t
//---------------------------------------------------------------------------
uint16_t ConvertToWORD(const char * Orig_char)
{
	uint16_t wRet;

	wRet = 0x0000;
	for(int i=0; i<4; i++)
	{
		if ( i > 0 )
			wRet <<= 4;

		if ( Orig_char[ i ] >= 'A' && Orig_char[ i ] <= 'F')
		{
			wRet += Orig_char[ i ] - 'A' + 10;
		}
		else
		if ( Orig_char[ i ] >= 'a' && Orig_char[ i ] <= 'f')
		{
			wRet += Orig_char[ i ] - 'a' + 10;
		}
		else
		if ( Orig_char[ i ] >= '0' && Orig_char[ i ] <= '9')
		{
			wRet += Orig_char[ i ] - '0';
		}
		else
		{
			return 0;
		}
	}
	return wRet;
}
//---------------------------------------------------------------------------
//Name: HexToInt
//description: 將字串轉為16進位整數, 如果出現非 '0'~'9'的字元回傳-1
//	       如果前兩個字元無0x or 0X 將整個字串視為16進位數
//parameter:
//       char * cHexStr:
//return:
//       int:出現非 '0'~'9'的字元回傳-1
//---------------------------------------------------------------------------
int HexToInt(const char * cHexStr)
{
	int Strlen = strlen(cHexStr);
        char cNowHexStr[128];
        int iValue = 0;

        if ( Strlen <= 0 )
        	return -1;

        if ( strlen(cHexStr) > 2 )
        {
        	if ( (cHexStr[0] == '0' && cHexStr[1] == 'x') ||
                     (cHexStr[0] == '0' && cHexStr[1] == 'X') )
                {
                	strcpy(cNowHexStr, cHexStr + 2);
                        Strlen = strlen(cNowHexStr);
                }
                else
                {
        		strcpy(cNowHexStr, cHexStr);
        		Strlen = strlen(cNowHexStr);
                }
        }
        else
        {
        	strcpy(cNowHexStr, cHexStr);
        	Strlen = strlen(cNowHexStr);
        }

        iValue = 0;
        for(int i=0; i<Strlen; i++)
        {
                if ( i > 0 )
        		iValue *= 16;

                if ( cNowHexStr[ i ] >= 'A' && cNowHexStr[ i ] <= 'F')
                {
	                iValue += cNowHexStr[ i ] - 'A' + 10;
                }
                else
                if ( cNowHexStr[ i ] >= 'a' && cNowHexStr[ i ] <= 'f')
                {
                        iValue += cNowHexStr[ i ] - 'a' + 10;
                }
                else
                if ( cNowHexStr[ i ] >= '0' && cNowHexStr[ i ] <= '9')
                {
                        iValue += cNowHexStr[ i ] - '0';
                }
                else
                	return -1;
        }

        return iValue;
}
