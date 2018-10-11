#include "CSceneReaderBinary.h"

CSceneReaderBinary::CSceneReaderBinary(const CString &fileName)
{
    FileName = fileName;
}

bool CSceneReaderBinary::Open()
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

SSD::SNode* CSceneReaderBinary::ReadNode()
{
    unsigned char garbage;
    ReadByte(garbage);
    if (garbage != SSD::NodeBegin)
    {
        LastError = "Error reading node identifier";
        return NULL;
    }
    SSD::SNode* node = new SSD::SNode();
    ReadUShort(node->ID);
    ReadUShort(node->ParentID);
    ReadByte(node->Type);
    ReadByte(node->NameLength);
    FileStream.read(node->Name, node->NameLength);
    ReadByte(node->AttributeCount);
    ReadByte(node->NodeCount);

    node->Attributes = new SSD::SAttribute*[node->AttributeCount];

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

SSD::SAttribute* CSceneReaderBinary::ReadAttribute()
{
    unsigned char garbage;
    ReadByte(garbage);
    if (garbage != SSD::AttributeBegin)
    {
        LastError = "Error reading attr identifier";
        return NULL;
    }
    
    SSD::SAttribute* attr = new SSD::SAttribute();
    ReadByte(attr->NameLength);
    FileStream.read(attr->Name, attr->NameLength);
    ReadByte(attr->DataType);
    ReadUInt32(attr->ElementCount);
    attr->Values = new unsigned char[attr->ElementCount];
    FileStream.read((char *)attr->Values, attr->ElementCount);
    
    ReadByte(garbage);
    if (garbage == SSD::NodeEnd)
        return attr;

    delete attr;
    return NULL;
}

void CSceneReaderBinary::ReadHeader(SSD::SHeader &header)
{
    if (!FileStream.is_open()) return;

    FileStream.read(header.Signature, 3);
    ReadByte(header.Version);
    // FileStream.read((char*)&header.Version, 1);
}
void CSceneReaderBinary::ReadUInt32(unsigned int &val)
{
    if (!FileStream.is_open()) return;
    unsigned char bytes[4];
    FileStream.read((char*)&bytes, 4);
    val =   (0xff000000 & (bytes[0] << 24)) |
            (0xff0000 & (bytes[1] << 16)) |
            (0xff00 & (bytes[2] << 8)) |
            (0xff & bytes[3]);
}

void CSceneReaderBinary::ReadUShort(unsigned short &nodeCount)
{
    if (!FileStream.is_open()) return;
    unsigned char bytes[2];
    FileStream.read((char*)&bytes, 2);
    nodeCount = (0xff00 & (bytes[0] << 8)) |
                (0xff & bytes[1]);
}

void CSceneReaderBinary::ReadByte(unsigned char &val)
{
    if (!FileStream.is_open()) return;
    FileStream.read((char*)&val, 1);
}

void CSceneReaderBinary::Close()
{
    if (FileStream.is_open()) FileStream.close();
}

CSceneReaderBinary::~CSceneReaderBinary()
{
    Close();
}
