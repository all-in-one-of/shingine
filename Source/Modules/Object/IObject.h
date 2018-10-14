#pragma once
#include "../../Components/IComponent.h"
#include "Common.h"

class IObject
{
public:
    virtual ~IObject() {};
    virtual unsigned int ID() = 0;
    virtual IComponent* AddComponent(const CString &componentTypeName) = 0;
    virtual void DestroyComponent(const CString &componentTypeName) = 0;
    virtual IComponent* GetComponent(const CString &componentTypeName) const = 0;
};
