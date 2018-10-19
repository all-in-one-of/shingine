#include "CDataNode.h"
#include "SSD.h"
#include "Struct.h"
#include "CTypedAttribute.h"
#include "CSerializedFactory.h"

CDataNode::CDataNode(SSD::SNode* node)
{
    NodeName = CString(node->Name);
    UniqueID = node->UniqueID;
    for (unsigned int x = 0; x < node->NodeCount; x++)
        Nodes.push_back(new CDataNode(node->Nodes[x]));

    for (unsigned int x = 0; x < node->AttributeCount; x++)
    {
        SSD::SAttribute* attribute = node->Attributes[x];
        CString typeName = CString(attribute->DataType);
        Attributes.push_back(MakeTypedAttribute(attribute));
    }
}

unsigned int CDataNode::GetUniqueID()
{
    return UniqueID;
}

void CDataNode::SetUniqueID(const unsigned int &newID)
{
    UniqueID = newID;
}

CDataNode::~CDataNode() 
{
    for(unsigned int x = 0; x < Attributes.size(); x++)
        delete Attributes[x];

    for(unsigned int x = 0; x < Nodes.size(); x++)
        delete Nodes[x];
}

ISerialized* CDataNode::Deserialize()
{
    ISerializedClass* serializedClass = dynamic_cast<ISerializedClass*>(
        CSerializedFactory::CreateInstance(Name().GetStdString()));

    serializedClass->SetUniqueID(UniqueID);

    if (serializedClass == NULL) return serializedClass;

    for (unsigned char x = 0; x < Attributes.size(); x++)
        serializedClass->SetAttribute(Attributes[x]);

    for (unsigned char x = 0; x < Nodes.size(); x++)
    {
        ISerialized* deserializedDataNode = Nodes[x]->Deserialize();
        serializedClass->SetAttribute(deserializedDataNode);
    }
    return serializedClass;
}


ISerialized* CDataNode::MakeTypedAttribute(SSD::SAttribute* attribute)
{
    // unpack data here
    CString name = CString(attribute->Name);
    CString typeName = CString(attribute->DataType);

    if (typeName == "char")
    {
        char* temp = new char[attribute->ElementCount + 1];
        for (unsigned x = 0; x < attribute->ElementCount; x++)
            temp[x] = attribute->Values[x];
        temp[attribute->ElementCount] = '\0';
        CString tempStr = CString(temp);
        delete[] temp;
        return new CTypedAttributeValue<CString>(name, typeName, tempStr);
    }
    
    unsigned char stride;
    DataStruct::GetStride(typeName, stride);

    std::vector<int> data_int;
    std::vector<unsigned int> data_uint;
    std::vector<unsigned short> data_ushort;
    std::vector<short> data_short;
    std::vector<float> data_float;
    std::vector<unsigned char> data_uchar;

    for (unsigned int x = 0; x < attribute->ElementCount * stride; x+=stride)
    {
        unsigned char* value = new unsigned char[stride];
        for (unsigned char y = 0; y < stride; y++)
            value[y] = attribute->Values[x + y];

        if (typeName == "int") 
        {
            if (attribute->ElementCount == 1)
                return new CTypedAttributeValue<int>(name, typeName, DataStruct::GetInt32(value));    
            data_int.push_back(DataStruct::GetInt32(value));
        
        }
        if (typeName == "uid")
        {
            unsigned int val = DataStruct::GetUInt32(value);
            return new CAttributeUniqueId(name, val);
        }
        if (typeName == "unsigned int") 
        {
            if (attribute->ElementCount == 1)
                return new CTypedAttributeValue<unsigned int>(name, typeName, DataStruct::GetUInt32(value));    
            data_uint.push_back(DataStruct::GetUInt32(value));
        
        }
        if (typeName == "unsigned short") 
        {
            if (attribute->ElementCount == 1)
                return new CTypedAttributeValue<unsigned short>(name, typeName, DataStruct::GetUShort(value));    
            data_ushort.push_back(DataStruct::GetUShort(value));
        
        }
        if (typeName == "short") 
        {
            if (attribute->ElementCount == 1)
                return new CTypedAttributeValue<short>(name, typeName, DataStruct::GetShort(value));    
            data_short.push_back(DataStruct::GetShort(value));
        
        }
        if (typeName == "float") 
        {
            if (attribute->ElementCount == 1)
                return new CTypedAttributeValue<float>(name, typeName, DataStruct::GetFloat(value));    
            data_float.push_back(DataStruct::GetFloat(value));
        
        }
        if (typeName == "unsigned char") 
        {
            if (attribute->ElementCount == 1)
                return new CTypedAttributeValue<unsigned char>(name, typeName, value[0]);    

            data_uchar.push_back(value[0]);
        
        }
        delete[] value;
    }

    if (typeName == "int") 
        return new CTypedAttribute<int>(name, typeName, data_int);
    if (typeName == "unsigned int") 
        return new CTypedAttribute<unsigned int>(name, typeName, data_uint);
    if (typeName == "unsigned short") 
        return new CTypedAttribute<unsigned short>(name, typeName, data_ushort);
    if (typeName == "short") 
        return new CTypedAttribute<short>(name, typeName, data_short);
    if (typeName == "float") 
        return new CTypedAttribute<float>(name, typeName, data_float);
    if (typeName == "unsigned char") 
        return new CTypedAttribute<unsigned char>(name, typeName, data_uchar);
    return NULL;
}

CString CDataNode::Name() { return NodeName; }
std::vector<ISerialized*> CDataNode::GetAttributes() { return Attributes; }
std::vector<IDataNode*> CDataNode::GetNodes() { return Nodes; }
