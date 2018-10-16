#pragma once
#include "CTypedAttribute.h"
#include <string>
#include <map>
#include <iostream>

class IAttributeSerialized
{
public:
    virtual ~IAttributeSerialized() {};
    virtual void SetAttribute(ITypedAttribute* &attr) = 0;
    virtual void GetAttribute(ITypedAttribute* &attr) = 0;
    virtual void GetAllAttributes(std::vector<ITypedAttribute*> &attr) = 0;
};

#define ATTRIBUTE_DECL_INIT(CLASSNAME) \
    typedef void (CLASSNAME::*MFP)(ITypedAttribute* &attr); \
    std::map<std::string, MFP> AttributeFunctionMap; \
    virtual void SetAttribute(ITypedAttribute* &attr) \
    { \
        std::string mapName = std::string("Set_") + attr->Name().GetStdString(); \
        std::cout << "SetAttribute" << mapName << std::endl; \
        if (AttributeFunctionMap.find(mapName) == AttributeFunctionMap.end()) return; \
        std::cout << "FOUND " << mapName << std::endl; \
        MFP fp = AttributeFunctionMap[mapName]; \
        (this->*fp)(attr); \
    }; \
    virtual void GetAttribute(ITypedAttribute* &attr) \
    { \
        std::string mapName = std::string("Get_") + attr->Name().GetStdString(); \
        if (AttributeFunctionMap.find(mapName) == AttributeFunctionMap.end()) return; \
        MFP fp = AttributeFunctionMap[mapName]; \
        (this->*fp)(attr); \
    }; \
    virtual void GetAllAttributes(std::vector<ITypedAttribute*> &attr) \
    { \
    }; \

#define ATTRIBUTE_VALUE_INFO(TYPE_NAME,NAME) \
    TYPE_NAME NAME; \
    void Attrib_Set_##NAME(ITypedAttribute* &attr) \
    { \
        std::cout << "CALLING SET" << #NAME << std::endl; \
        std::vector<TYPE_NAME> arr = ((CTypedAttribute<TYPE_NAME> *)attr)->Get(); \
        NAME = arr[0]; \
    } \
    void Attrib_Get_##NAME(ITypedAttribute* &attr) \
    { \
        std::vector<TYPE_NAME> data; \
        data.push_back(NAME); \
        attr = new CTypedAttribute<TYPE_NAME>(#NAME, #TYPE_NAME, data); \
    }

#define ATTRIBUTE_VECTOR_INFO(TYPE_NAME,NAME) \
    std::vector<TYPE_NAME> NAME; \
    void Attrib_Set_##NAME(ITypedAttribute* &attr) \
    { \
        CTypedAttribute<TYPE_NAME> * typedAttr = (CTypedAttribute<TYPE_NAME> *)attr; \
        if (typedAttr == NULL) return; \
        NAME = typedAttr->Get(); \
    } \
    void Attrib_Get_##NAME(ITypedAttribute* &attr) \
    { \
        attr = new CTypedAttribute<TYPE_NAME>(#NAME, #TYPE_NAME, NAME); \
    } 

#define ATTRIBUTE_REGISTER(CLASSNAME,NAME) \
    AttributeFunctionMap.insert( std::make_pair( std::string("Get_") + #NAME , &CLASSNAME::Attrib_Get_##NAME )); \
    AttributeFunctionMap.insert( std::make_pair( std::string("Set_") + #NAME , &CLASSNAME::Attrib_Set_##NAME )); 
