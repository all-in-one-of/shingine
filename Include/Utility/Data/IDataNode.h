#pragma once
#include <vector>
#include "../Common.h"

class ISerialized;

class IDataNode
{
public:
    virtual ~IDataNode() {};
    virtual unsigned int GetUniqueID() = 0;
    virtual void SetUniqueID(const unsigned int &newID) = 0;
    virtual CString Name() = 0;
    virtual std::vector<ISerialized*> GetAttributes() = 0;
    virtual std::vector<IDataNode*> GetNodes() = 0;
    virtual ISerialized* Deserialize() = 0;
};
