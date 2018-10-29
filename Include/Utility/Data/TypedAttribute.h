#pragma once
#include <vector>
#include "../Common.h"
#include "ISerialized.h"

// for ATTRIBUTE_VECTOR
template <class T>
class TypedAttribute : public ISerialized
{
public:
    TypedAttribute(String name, String type, std::vector<T> &data)
    {
        AttributeName = name; 
        AttributeType = type;
        Data = data;
    };

    virtual ~TypedAttribute() {};
    virtual String SerializedName() { return AttributeName; }; 
    virtual String TypeName() { return AttributeType; }; 

    std::vector<T> Get() { return Data; }
    void Set(std::vector<T> &data) { Data = data; }

private:
    std::vector<T> Data;
    String AttributeName;
    String AttributeType;
};

// for ATTRIBUTE_VALUE
template <class T>
class TypedAttributeValue : public ISerialized
{
public:
    TypedAttributeValue(String name, String type, T data)
    {
        AttributeName = name; 
        AttributeType = type;
        Data = data;
    };

    virtual ~TypedAttributeValue() {};
    virtual String SerializedName() { return AttributeName; }; 
    virtual String TypeName() { return AttributeType; }; 

    T Get() { return Data; }
    void Set(T &data) { Data = data; }

private:
    T Data;
    String AttributeName;
    String AttributeType;
};

// for ATTRIBUTE_CLASS_VECTOR
class CAttributeClassVector : public ISerialized
{
public:
    CAttributeClassVector(String name, std::vector<ISerialized*> &data)
    {
        AttributeName = name; 
        Data = data;
    };

    virtual ~CAttributeClassVector() {};
    virtual String SerializedName() { return AttributeName; }; 
    virtual String TypeName() { return "SerializedClass"; }; 

    std::vector<ISerialized*> Get() { return Data; }
    void Set(std::vector<ISerialized*> &data) { Data = data; }

private:
    std::vector<ISerialized*> Data;
    String AttributeName;
};
