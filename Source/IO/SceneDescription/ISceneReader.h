#pragma once
#include <fstream>
#include <vector>
#include "Common.h"

class IDataNode;

class ISceneReader
{
public:
    virtual ~ISceneReader() {};

    virtual CString GetLastError() = 0;
    virtual bool Open() = 0;
    virtual void ReadNodes(std::vector<IDataNode*> &nodes) = 0;
    virtual void Close() = 0;
};
