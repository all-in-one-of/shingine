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
    return NULL;
}
