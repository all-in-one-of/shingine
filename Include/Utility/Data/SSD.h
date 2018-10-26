#pragma once

namespace SSD
{
    const unsigned char NodeBegin = 0xaa;
    const unsigned char NodeEnd = 0xab;
    const unsigned char AttributeBegin = 0xba;
    const unsigned char AttributeEnd = 0xbb;

    struct SHeader
    {
        char Signature[3];
        unsigned char Version;
    };

    struct SNode;

    struct SAttribute
    {
        unsigned char NameLength;
        char* Name;
        unsigned char DataTypeLength;
        char* DataType;
        unsigned char IsSingleElement;
        unsigned int ByteCount;
        unsigned int ElementCount;
        unsigned char* Values;
        SNode** Nodes;
        ~SAttribute();

    };

    struct SNode
    {
        unsigned char NameLength;
        char* Name;
        unsigned int UniqueID;
        unsigned char AttributeCount;
        unsigned char NodeCount;
        SAttribute** Attributes;
        SNode** Nodes;
        ~SNode();
    };
};
