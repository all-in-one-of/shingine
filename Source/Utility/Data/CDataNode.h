#pragma once
#include "IDataNode.h"

namespace SSD { struct SNode; struct SAttribute; }

class CDataNode : public IDataNode 
{
public:
    CDataNode(SSD::SNode* node);
    virtual ~CDataNode();
    virtual short ID();
    virtual short ParentID();
    virtual CString Name();
    virtual std::vector<ITypedAttribute*> GetAttributes();
    virtual std::vector<IDataNode*> GetNodes();

    static void GetStride(const CString &typeName, unsigned char &stride);
private:
    ITypedAttribute* MakeTypedAttribute(SSD::SAttribute* attribute);

    short NodeID, NodeParentID;
    CString NodeName;
    std::vector<ITypedAttribute*> Attributes;
    std::vector<IDataNode*> Nodes;
};