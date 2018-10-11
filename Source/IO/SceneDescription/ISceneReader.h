#pragma once
#include <fstream>
#include "Common.h"
#include "SSD.h"

class ISceneReader
{
public:
    virtual ~ISceneReader() {};

    virtual CString GetLastError() = 0;
    virtual bool Open() = 0;
    virtual void Close() = 0;


    virtual void ReadHeader(SSD::SHeader &header) = 0;
    virtual void ReadUShort(unsigned short &nodeCount) = 0;
    virtual void ReadByte(unsigned char &val) = 0;
    virtual void ReadUInt32(unsigned int &val) = 0;
    virtual SSD::SNode* ReadNode() = 0;
    virtual SSD::SAttribute* ReadAttribute() = 0;

    // virtual 
protected:
    CString FileName;
    CString LastError;
    std::ifstream FileStream;
};
