
#ifndef SERIAL_CHECK_H
#define SERIAL_CHECK_H

//for check Serial Number
static char Table1[16] = {'Z','Q','W','R','T','Y','U','I','P','L','A','B','C','D','E','F'};
static char Table2[16] = {'K','J','H','G','F','D','S','A','Z','X','C','V','B','N','M','L'};
static char Table3[16] = {'G','H','Y','T','R','F','V','B','N','J','U','I','K','S','X','O'};
bool CheckSerialNO(const char * SerialNO, char * cCID);
bool ValidateSN(const char * SerialNO);
bool CheckSoftwareVersion(const char * cSerialNO);
bool CheckProduct(const char * cSerialNO);
bool CheckMonth(const char * cSerialNO);
bool CheckYear(const char * cSerialNO);
bool CheckSerialNO(const char * cSerialNO);
bool CheckEncrypt(const char * cSerialNO);
char GetTable(char cValue, const char * Table);
int GetIntValue(const char * cValue);
char IntToChar(int iValue);
uint16_t ConvertToWORD(const char * Orig_char);
int HexToInt(const char * cHexStr);

#endif
