#pragma once
#include <vector>
#include "../Common.h"
#include "ISerialized.h"

// for ATTRIBUTE_VECTOR
template <class T>
class CTypedAttribute : public ISerialized
{
public:
    CTypedAttribute(CString name, CString type, std::vector<T> &data)
    {
        AttributeName = name; 
        AttributeType = type;
        Data = data;
    };

    virtual ~CTypedAttribute() {};
    virtual CString SerializedName() { return AttributeName; }; 
    virtual CString TypeName() { return AttributeType; }; 

    std::vector<T> Get() { return Data; }
    void Set(std::vector<T> &data) { Data = data; }

private:
    std::vector<T> Data;
    CString AttributeName;
    CString AttributeType;
};

// for ATTRIBUTE_VALUE
template <class T>
class CTypedAttributeValue : public ISerialized
{
public:
    CTypedAttributeValue(CString name, CString type, T data)
    {
        AttributeName = name; 
        AttributeType = type;
        Data = data;
    };

    virtual ~CTypedAttributeValue() {};
    virtual CString SerializedName() { return AttributeName; }; 
    virtual CString TypeName() { return AttributeType; }; 

    T Get() { return Data; }
    void Set(T &data) { Data = data; }

private:
    T Data;
    CString AttributeName;
    CString AttributeType;
};

// for ATTRIBUTE_CLASS_VECTOR
class CAttributeClassVector : public ISerialized
{
public:
    CAttributeClassVector(CString name, std::vector<ISerialized*> &data)
    {
        AttributeName = name; 
        Data = data;
    };

    virtual ~CAttributeClassVector() {};
    virtual CString SerializedName() { return AttributeName; }; 
    virtual CString TypeName() { return "SerializedClass"; }; 

    std::vector<ISerialized*> Get() { return Data; }
    void Set(std::vector<ISerialized*> &data) { Data = data; }

private:
    std::vector<ISerialized*> Data;
    CString AttributeName;
};

// for ATTRIBUTE_ID
class CAttributeUniqueId : public ISerialized
{
public:
    CAttributeUniqueId(CString name, unsigned int data)
    {
        AttributeName = name; 
        Data = data;
    };

    virtual CString TypeName() 
    {
        return "uid"; 
    }; 

    virtual ~CAttributeUniqueId() {};
    virtual CString SerializedName()
    {
        return AttributeName;
    };
    unsigned int Get() { return Data; }
    void Set(unsigned int &data) { Data = data; }
private:
    unsigned int Data;
    CString AttributeName;
};
