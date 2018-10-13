#pragma once
#include <string>
#include "../../Utility/SSD.h"
#include "ISceneReader.h"

class CSceneReaderBinary : public ISceneReader
{
public:
    CSceneReaderBinary(const CString &fileName);
    virtual ~CSceneReaderBinary();
    virtual bool Open();
    virtual void Close();
    virtual CString GetLastError() { return LastError; }

    virtual void ReadHeader(SSD::SHeader &header);
    virtual void ReadUShort(unsigned short &nodeCount);
    virtual void ReadByte(unsigned char &val);
    virtual void ReadUInt32(unsigned int &val);
    virtual SSD::SNode* ReadNode();
    virtual SSD::SAttribute* ReadAttribute();
    // virtual 
private:
    CString FileName;
    CString LastError;
    std::ifstream FileStream;
};
