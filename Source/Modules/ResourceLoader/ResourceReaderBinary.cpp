#include "Modules/ResourceLoader/ResourceReaderBinary.h"
#include "Utility/Data/SSD.h"
#include "Utility/Data/DataStruct.h"
#include "Utility/Data/DataNode.h"

#include <iostream>

namespace SSD
{
     enum ContainerType { NODE = 0, VALUE = 1 };
     enum DataType { NONE = 0, BYTE = 1, UINT = 2, FLOAT = 3 };
     enum LightType { POINT = 0 };
     enum NodeType { LIGHT = 0, TRANSFORM = 1, MATERIAL = 2, MESH = 3 };
}

ResourceReaderBinary::ResourceReaderBinary(const String &fileName)
{
    FileName = fileName;
}

bool ResourceReaderBinary::Open()
{
    FileStream.open(FileName.GetStdString(), std::ios::binary | std::ios::in);
    bool success = FileStream.is_open();
    if (!success)
    {
        LastError = "Couldn't open the file " + FileName;
        return false;
    }
    return true;
}

void ResourceReaderBinary::ReadNodes(std::vector<IDataNode*> &nodes)
{
    SSD::Header header;
    unsigned short nodeCount = 0;
    ReadHeader(header);
    ReadUShort(nodeCount);
    
    for (unsigned short x = 0; x < nodeCount; x++)
    {
        SSD::Node* node = ReadNode();
        nodes.push_back(new DataNode(node));
        delete node;
    }
}

SSD::Node* ResourceReaderBinary::ReadNode()
{
    unsigned char garbage = 0;
    ReadByte(garbage);
    if (garbage != SSD::NodeBegin)
    {
        LastError = "Error reading node identifier";
        return NULL;
    }
    SSD::Node* node = new SSD::Node();
    ReadUInt32(node->UniqueID);
    ReadByte(node->NameLength);
    node->Name = new char[node->NameLength];
    FileStream.read(node->Name, node->NameLength);
    ReadByte(node->AttributeCount);
    ReadByte(node->NodeCount);

    node->Attributes = new SSD::Attribute*[node->AttributeCount];
    node->Nodes = new SSD::Node*[node->NodeCount];

    for (unsigned char x = 0; x != node->AttributeCount; x++)
        node->Attributes[x] = ReadAttribute();

    for (unsigned char x = 0; x != node->NodeCount; x++)
        node->Nodes[x] = ReadNode();

    ReadByte(garbage);
    if (garbage == SSD::NodeEnd)
        return node;

    LastError = "Couldn't find node end byte";
    delete node;
    return NULL;
}

SSD::Attribute* ResourceReaderBinary::ReadAttribute()
{
    unsigned char garbage = 0;
    ReadByte(garbage);
    if (garbage != SSD::AttributeBegin)
    {
        LastError = "Error reading attr identifier";
        return NULL;
    }
    
    SSD::Attribute* attr = new SSD::Attribute();
    ReadByte(attr->NameLength);

    attr->Name = new char[attr->NameLength];
    FileStream.read(attr->Name, attr->NameLength);

    ReadByte(attr->DataTypeLength);
    attr->DataType = new char[attr->DataTypeLength];
    FileStream.read(attr->DataType, attr->DataTypeLength);

    ReadByte(attr->IsSingleElement);

    ReadUInt32(attr->ByteCount);
    ReadUInt32(attr->ElementCount);

    String dataTypeName = String(attr->DataType);
    
    if (dataTypeName == "SerializedClass")
    {
        // attribute is an array of nodes
        attr->Nodes = new SSD::Node*[attr->ElementCount];
        for (size_t x = 0; x < attr->ElementCount; x++)
        {
            attr->Nodes[x] = ReadNode();
        }
    }
    else
    {
        // nodes
        if(dataTypeName == "uid") dataTypeName = "unsigned int";

        // TODO get stride
        unsigned char stride;   // = dataTypeName == "unsigned char" || dataTypeName == "char" ? 1 : 4;
        DataStruct::GetStride(dataTypeName, stride);

        //unsigned int byteCount = attr->ElementCount * stride;
        unsigned int byteCount = attr->ByteCount;

        attr->Values = new unsigned char[byteCount];
        for (unsigned int x = 0; x < byteCount; x++)
        {
            char v;
            FileStream.read(&v, 1);
            attr->Values[x] = v;
        }
    }
    
    ReadByte(garbage);
    if (garbage == SSD::AttributeEnd)
        return attr;

    delete attr;
    return NULL;
}

void ResourceReaderBinary::ReadHeader(SSD::Header &header)
{
    if (!FileStream.is_open()) return;

    FileStream.read(header.Signature, 3);
    ReadByte(header.Version);
}
void ResourceReaderBinary::ReadUInt32(unsigned int &val)
{
    if (!FileStream.is_open()) return;
    unsigned char* bytes = new unsigned char[4];
    FileStream.read((char*)bytes, 4);
    DataStruct::UnpackUInt32(val, bytes);
    delete[] bytes;
}

void ResourceReaderBinary::ReadUShort(unsigned short &val)
{
    if (!FileStream.is_open()) return;
    unsigned char* bytes = new unsigned char[2];
    FileStream.read((char*)bytes, 2);
    DataStruct::UnpackUShort(val, bytes);
    delete[] bytes;
}

void ResourceReaderBinary::ReadByte(unsigned char &val)
{
    if (!FileStream.is_open()) return;
    FileStream.read((char*)&val, 1);
}

void ResourceReaderBinary::Close()
{
    if (FileStream.is_open()) FileStream.close();
}

ResourceReaderBinary::~ResourceReaderBinary()
{
    Close();
}
