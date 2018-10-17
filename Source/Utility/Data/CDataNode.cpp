#include "CDataNode.h"
#include "SSD.h"
#include "Struct.h"
#include "CTypedAttribute.h"
#include "CSerializedFactory.h"

CDataNode::CDataNode(SSD::SNode* node)
{
    NodeID = node->ID;
    NodeName = CString(node->Name);
    
    for (unsigned int x = 0; x < node->NodeCount; x++)
        Nodes.push_back(new CDataNode(node->Nodes[x]));

    for (unsigned int x = 0; x < node->AttributeCount; x++)
    {
        SSD::SAttribute* attribute = node->Attributes[x];
        CString typeName = CString(attribute->DataType);
        Attributes.push_back(MakeTypedAttribute(attribute));
    }
}

CDataNode::~CDataNode() {}

ISerialized* CDataNode::Deserialize()
{
    ISerializedClass* serializedClass = dynamic_cast<ISerializedClass*>(
        CSerializedFactory::CreateInstance(Name().GetStdString()));
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
        std::vector<CString> data_char;
        char* temp = new char[attribute->ElementCount + 1];
        for (unsigned x = 0; x < attribute->ElementCount; x++)
            temp[x] = attribute->Values[x];
        temp[attribute->ElementCount] = '\0';
        data_char.push_back(CString(temp));
        delete[] temp;
        return new CTypedAttribute<CString>(name, typeName, data_char);
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
            data_int.push_back(DataStruct::GetInt32(value));
        if (typeName == "unsigned int") 
            data_uint.push_back(DataStruct::GetUInt32(value));
        if (typeName == "unsigned short") 
            data_ushort.push_back(DataStruct::GetUShort(value));
        if (typeName == "short") 
            data_short.push_back(DataStruct::GetShort(value));
        if (typeName == "float") 
            data_float.push_back(DataStruct::GetFloat(value));
        if (typeName == "unsigned char") 
            data_uchar.push_back(value[0]);
        // if (typeName == "char") 
        //     data_char.push_back(value[0]);
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
    // if (typeName == "char") 
    //     return new CTypedAttribute<char>(name, typeName, data_char);
    return NULL;
}

unsigned int CDataNode::ID() { return NodeID; }
CString CDataNode::Name() { return NodeName; }
std::vector<ISerialized*> CDataNode::GetAttributes() { return Attributes; }
std::vector<IDataNode*> CDataNode::GetNodes() { return Nodes; }
