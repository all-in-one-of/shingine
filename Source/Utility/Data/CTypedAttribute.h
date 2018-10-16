#pragma once
#include <vector>
#include "ITypedAttribute.h"

template <class T>
class CTypedAttribute : public ITypedAttribute
{
public:
    CTypedAttribute(CString name, CString type, std::vector<T> &data)
    {
        AttributeName = name; 
        AttributeType = type;
        Data = data;
    };

    
    virtual ~CTypedAttribute();
    virtual CString Name();
    virtual CString TypeName();

    std::vector<T> Get();
    void Set(std::vector<T> &data);

    // void SetAt(unsigned int index, T &value);
    // void AddValue(T &value);

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
CString CTypedAttribute<T>::Name()
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
