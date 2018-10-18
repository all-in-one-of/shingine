#pragma once
#include "IDataNode.h"

namespace SSD { struct SNode; struct SAttribute; }

class CDataNode : public IDataNode 
{
public:
    CDataNode(SSD::SNode* node);
    virtual ~CDataNode();
    virtual CString Name();
    virtual std::vector<ISerialized*> GetAttributes();
    virtual std::vector<IDataNode*> GetNodes();
    virtual ISerialized* Deserialize();

    static void GetStride(const CString &typeName, unsigned char &stride);
private:
    ISerialized* MakeTypedAttribute(SSD::SAttribute* attribute);

    CString NodeName;
    std::vector<ISerialized*> Attributes;
    std::vector<IDataNode*> Nodes;
};
