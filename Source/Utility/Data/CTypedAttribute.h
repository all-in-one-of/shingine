#pragma once
#include <vector>
#include "../Common.h"
#include "ISerialized.h"

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

    virtual ~CTypedAttribute();
    virtual CString SerializedName();
    virtual CString TypeName();

    std::vector<T> Get();
    void Set(std::vector<T> &data);

private:
    std::vector<T> Data;
    CString AttributeName;
    CString AttributeType;
};

template <class T>
CTypedAttribute<T>::~CTypedAttribute()
{
}

template <class T>
CString CTypedAttribute<T>::SerializedName()
{
    return AttributeName;
}

template <class T>
CString CTypedAttribute<T>::TypeName()
{
    return AttributeType;
}

template<class T>
std::vector<T> CTypedAttribute<T>::Get()
{
    return Data;
}

template<class T>
void CTypedAttribute<T>::Set(std::vector<T> &data)
{
    Data = data;
}
