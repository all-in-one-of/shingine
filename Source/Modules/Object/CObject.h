#pragma once

#include "IObject.h"
#include <map>

class CObject : public IObject
{
public:
    CObject(unsigned int id);
    virtual ~CObject();
    virtual unsigned int ID();
    virtual IComponent* AddComponent(const CString &componentTypeName);
    virtual void DestroyComponent(const CString &componentTypeName);
    virtual IComponent* GetComponent(const CString &componentTypeName) const;
protected:
    std::map<CString, IComponent*> Components;
    unsigned int Id;
};
