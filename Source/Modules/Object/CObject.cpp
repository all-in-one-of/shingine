#include "CObject.h"
#include "../../Components/SComponentFactory.h"

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
    IComponent* component = 
        SComponentFactory::CreateInstance(componentTypeName.GetStdString());
    Components[componentTypeName] = component;
    return component;
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
