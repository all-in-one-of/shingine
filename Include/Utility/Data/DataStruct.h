#pragma once
#include "../Common.h"
#include <string.h>

namespace 
{
namespace DataStruct
{
    void GetStride(const String &typeName, unsigned char &stride)
    {
        stride = 1;
        if (typeName == "int") stride = 4;
        if (typeName == "unsigned int") stride = 4;
        if (typeName == "unsigned short") stride = 2;
        if (typeName == "short") stride = 2;
        if (typeName == "float") stride = 4;
        // if (typeName == "unsigned char") stride = 1;
        if(typeName == "uid") stride = 4;
    }
    void UnpackUInt32(unsigned int &val, unsigned char *bytes)
    {
        val =   (0xff000000 & (bytes[0] << 24)) |
                (0xff0000 & (bytes[1] << 16)) |
                (0xff00 & (bytes[2] << 8)) |
                (0xff & bytes[3]);
    }

    unsigned int GetUInt32(unsigned char *bytes)
    {
        unsigned int val;
        UnpackUInt32(val, bytes);
        return val;
    }

    void UnpackInt32(int &val, unsigned char *bytes)
    {
        unsigned int value;
        UnpackUInt32(value, bytes);
        val = (int) value;
    }

    int GetInt32(unsigned char *bytes)
    {
        int val;
        UnpackInt32(val, bytes);
        return val;
    }

    void UnpackUShort(unsigned short &val, unsigned char *bytes)
    {
        val =   (0xff00 & (bytes[0] << 8)) |
                (0xff & bytes[1]);
    }
   
    unsigned short GetUShort(unsigned char *bytes)
    {
        unsigned short val;
        UnpackUShort(val, bytes);
        return val;
    }

    void UnpackShort(short &val, unsigned char *bytes)
    {
        unsigned short value;
        UnpackUShort(value, bytes);
        val = (short) value;
    }

    short GetShort(unsigned char *bytes)
    {
        short val;
        UnpackShort(val, bytes);
        return val;
    }

    void UnpackFloat(float &val, unsigned char *bytes)
    {
        memcpy(&val, bytes, sizeof(val));
    }

    float GetFloat(unsigned char *bytes)
    {
        float val;
        UnpackFloat(val, bytes);
        return val;
    }
}
}

