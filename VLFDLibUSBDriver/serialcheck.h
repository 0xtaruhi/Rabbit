
#ifndef SERIAL_CHECK_H
#define SERIAL_CHECK_H

//for check Serial Number
static char Table1[16] = {'Z','Q','W','R','T','Y','U','I','P','L','A','B','C','D','E','F'};
static char Table2[16] = {'K','J','H','G','F','D','S','A','Z','X','C','V','B','N','M','L'};
static char Table3[16] = {'G','H','Y','T','R','F','V','B','N','J','U','I','K','S','X','O'};
BOOL CheckSerialNO(const char * SerialNO, char * cCID);
BOOL ValidateSN(const char * SerialNO);
BOOL CheckSoftwareVersion(const char * cSerialNO);
BOOL CheckProduct(const char * cSerialNO);
BOOL CheckMonth(const char * cSerialNO);
BOOL CheckYear(const char * cSerialNO);
BOOL CheckSerialNO(const char * cSerialNO);
BOOL CheckEncrypt(const char * cSerialNO);
char GetTable(char cValue, const char * Table);
int GetIntValue(const char * cValue);
char IntToChar(int iValue);
WORD ConvertToWORD(const char * Orig_char);
int HexToInt(const char * cHexStr);

#endif
