#pragma once
#include <unordered_map>
#include "Utility/Data/Serialization.h"

class CEntity : public ISerializedClass
{
public:
    SERIALIZE_CLASS(CEntity)
    CEntity() {};
    virtual ~CEntity(){};
};

class CComponentIdCollection : public ISerializedClass
{
public:
    SERIALIZE_CLASS(CComponentIdCollection)
    CComponentIdCollection() { ATTRIBUTE_REGISTER(CComponentIdCollection, Ids) };
    virtual ~CComponentIdCollection() {};
    ATTRIBUTE_ID_VECTOR(Ids)
};

class CEntityIdCollection : public ISerializedClass
{
public:
    SERIALIZE_CLASS(CEntityIdCollection)
    CEntityIdCollection() 
    { 
        ATTRIBUTE_REGISTER(CEntityIdCollection,Ids) 
        ATTRIBUTE_REGISTER(CEntityIdCollection,Components) 
    };
    virtual ~CEntityIdCollection() 
    {
        for (size_t x = 0; x < Components.size(); x++)
            delete Components[x];
    };
    ATTRIBUTE_ID_VECTOR(Ids)
    ATTRIBUTE_CLASS_VECTOR(CComponentIdCollection, Components)
};

