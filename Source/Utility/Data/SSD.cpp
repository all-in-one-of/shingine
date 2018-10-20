#include "Utility/Data/SSD.h"

namespace SSD
{
    SNode::~SNode()
    {
        delete[] Name;
        for (unsigned char x = 0; x < AttributeCount; x++)
            delete Attributes[x];
        delete[] Attributes;

        for (unsigned char x = 0; x < NodeCount; x++)
            delete Nodes[x];
        delete[] Nodes;
    }

    SAttribute::~SAttribute()
    {
        delete[] Name;
        delete[] DataType;
        delete[] Values;
        if(!Nodes) return;
        for(size_t x = 0; x < ElementCount; x++)
        {
            if(!Nodes[x]) continue;
            delete Nodes[x];
        }
        delete[] Nodes;
    }
};
