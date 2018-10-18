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

    struct SAttribute
    {
        unsigned char NameLength;
        char* Name;
        unsigned char DataTypeLength;
        char* DataType;
        unsigned int ElementCount;
        unsigned char* Values;

        ~SAttribute()
        {
            delete[] Name;
            delete[] DataType;
            delete[] Values;
        }
    };

    struct SNode
    {
        unsigned char NameLength;
        char* Name;
        unsigned char AttributeCount;
        unsigned char NodeCount;
        SAttribute** Attributes;
        SNode** Nodes;

        ~SNode()
        {
            delete[] Name;
            for (unsigned char x = 0; x < AttributeCount; x++)
                delete Attributes[x];
            delete[] Attributes;

            for (unsigned char x = 0; x < NodeCount; x++)
                delete Nodes[x];
            delete[] Nodes;
        }
    };
};
