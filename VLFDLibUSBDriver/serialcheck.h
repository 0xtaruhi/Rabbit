
#ifndef SERIAL_CHECK_H
#define SERIAL_CHECK_H

#include <stdint.h>

//for check Serial Number
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
