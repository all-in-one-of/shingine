#include "CObject.h"
#include "../../Utility/Data/CSerializedFactory.h"

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
    ISerialized* serializedObject = CSerializedFactory::CreateInstance(componentTypeName.GetStdString());
    if (!serializedObject) return NULL;

    IComponent* component = dynamic_cast<IComponent*>(serializedObject);
        
    if (!component)
    {
        delete serializedObject;
        return NULL;
    }
    
    Components[componentTypeName] = component;
    component->SetOwner(this);
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
