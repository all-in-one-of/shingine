#pragma once

#include <map>
#include "IObject.h"
#include "../../Utility/Data/Serialization.h"

class CObject : public IObject, public ISerializedClass
{
public:
    // ATTRIBUTE_DECL_INIT(CObject)
    CObject() {};
    virtual ~CObject() {};
    virtual unsigned int ID();
    virtual IComponent* AddComponent(const CString &componentTypeName);
    virtual void DestroyComponent(const CString &componentTypeName);
    virtual IComponent* GetComponent(const CString &componentTypeName) const;

    REGISTER_SERIALIZED_TYPE(CObject)
    // ISerializedClass
    
    virtual void SetAttribute(ISerialized* &attr);
    virtual void GetAttribute(ISerialized* &attr);
    virtual void GetAllAttributes(std::vector<ISerialized*> &attributes);

protected:
    std::map<CString, IComponent*> Components;
};
