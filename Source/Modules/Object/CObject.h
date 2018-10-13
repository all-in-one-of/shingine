#pragma once

#include "IObject.h"

class CObject : public IObject
{
public:
    CObject(unsigned int id);
    virtual ~CObject();
    virtual unsigned int ID();
    virtual IComponent* AddComponent(const CString &componentTypeName);
    virtual IComponent* GetComponent(const CString &componentTypeName) const;
protected:
    unsigned int Id;
};
