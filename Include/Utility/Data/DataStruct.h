#pragma once
class String;
namespace DataStruct
{
void GetStride(const String &typeName, unsigned char &stride);
void UnpackUInt32(unsigned int &val, unsigned char *bytes);
void UnpackUShort(unsigned short &val, unsigned char *bytes);
void UnpackShort(short &val, unsigned char *bytes);
void UnpackInt32(int &val, unsigned char *bytes);
void UnpackFloat(float &val, unsigned char *bytes);

unsigned int GetUInt32(unsigned char *bytes);
int GetInt32(unsigned char *bytes);
unsigned short GetUShort(unsigned char *bytes);
short GetShort(unsigned char *bytes);
float GetFloat(unsigned char *bytes);
} // namespace DataStruct
