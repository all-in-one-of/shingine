#pragma once
#include "../Common.h"
#include <vector>
class ITypedAttribute
{
public:
    virtual ~ITypedAttribute() {};
    virtual CString Name() = 0;
    virtual CString TypeName() = 0;
};
