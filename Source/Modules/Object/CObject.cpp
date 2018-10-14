#include "CObject.h"

unsigned int CObject::ID() { return Id; }

CObject::CObject(unsigned int id)
{
    this->Id = id;
}

CObject::~CObject()
{
}

IComponent* CObject::AddComponent(const CString &componentTypeName)
{
    return NULL;
}

IComponent* CObject::GetComponent(const CString &componentTypeName) const
{
    if (Components.find(componentTypeName) != Components.end())
        return Components.at(componentTypeName);
    return NULL;
}

void CObject::DestroyComponent(const CString &componentTypeName)
{
    if (Components.find(componentTypeName) != Components.end())
    {
        // TODO
    }
}
