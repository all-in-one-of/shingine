#include "CObject.h"
#include <iostream>
REGISTER_SERIALIZED_NAME(CObject,Object)

unsigned int CObject::ID()
{
    return UniqueID();
}

IComponent* CObject::AddComponent(const CString &componentTypeName)
{
    ISerialized* serializedObject = CSerializedFactory::CreateInstance(componentTypeName.GetStdString());
    if (!serializedObject) 
        return NULL;

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

// ISerializedClass
void CObject::SetAttribute(ISerialized* &attr)
{
    // if (attr->SerializedName() == "Id")
    //     Id = ((CTypedAttributeValue<unsigned int> *)attr)->Get();

    IComponent* component = dynamic_cast<IComponent*>(attr);
    if (!component) 
        return;
    
    Components[attr->SerializedName()] = component;
    component->SetOwner(this);
}

void CObject::GetAttribute(ISerialized* &attr)
{
    // if (attr->SerializedName() == "Id")
    //     attr = new CTypedAttributeValue<unsigned int>("Id", "unsigned int", Id);

    if (Components.find(attr->SerializedName()) != Components.end())
        attr = dynamic_cast<ISerialized*>(Components.at(attr->SerializedName()));
}

void CObject::GetAllAttributes(std::vector<ISerialized*> &attributes)
{
    // attributes.push_back(
    //     new CTypedAttributeValue<unsigned int>("Id", "unsigned int", Id));

    std::map<CString,IComponent*>::iterator it;
    for (it = Components.begin(); it != Components.end(); it++)
    {
        attributes.push_back(
            dynamic_cast<ISerialized*>(it->second));
    }
}
