#pragma once
#include "IObject.h"

class IDataNode;

class CObjectBuilder
{
public:
    void SetupFromDataNode(IObject* object, IDataNode* node);
};
