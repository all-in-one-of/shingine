#pragma once
#include "IComponent.h"
#include <map>
#include <string>
#include <iomanip>

template<typename T> IComponent* createT() { return new T; }

struct SComponentFactory
{
    typedef std::map<std::string, IComponent*(*)()> TComponentTypeMap;

    static IComponent* CreateInstance(const std::string &s)
    {
        TComponentTypeMap::iterator it = GetMap()->find(s);
        if (it == GetMap()->end()) return NULL;
        return it->second();
    }

protected:
    static TComponentTypeMap *GetMap()
    {
        if (!Map) Map = new TComponentTypeMap;
        return Map;
    }
private:
    static TComponentTypeMap *Map;

};

template<typename T>
struct SComponentRegistry : SComponentFactory
{
    SComponentRegistry(const CString &s)
    {
        GetMap()->insert(std::make_pair(s.GetStdString(), &createT<T>));
    }
};

#define REGISTER_COMPONENT_TYPE(TYPENAME) \
    virtual CString GetTypeName(); \
    static const CString TypeName; \
    static SComponentRegistry<TYPENAME> reg

#define REGISTER_COMPONENT_NAME(TYPENAME,NAME) \
    const CString TYPENAME::TypeName = #NAME; \
    CString TYPENAME::GetTypeName() { return TYPENAME::TypeName; } \
    SComponentRegistry<TYPENAME> TYPENAME::reg(TYPENAME::TypeName); \

