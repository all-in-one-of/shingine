#pragma once
#include "IDataNode.h"

namespace SSD { struct Node; struct Attribute; }

class DataNode : public IDataNode 
{
public:
    DataNode(SSD::Node* node);
    virtual ~DataNode();
    virtual String Name();
    virtual std::vector<ISerialized*> GetAttributes();
    virtual std::vector<IDataNode*> GetNodes();
    virtual ISerialized* Deserialize();
    virtual unsigned int GetUniqueID();
    virtual void SetUniqueID(const unsigned int &newID);

    static void GetStride(const String &typeName, unsigned char &stride);
private:
    ISerialized* MakeTypedAttribute(SSD::Attribute* attribute);
    String NodeName;
    std::vector<ISerialized*> Attributes;
    std::vector<IDataNode*> Nodes;
    unsigned int UniqueID = -1;
};
