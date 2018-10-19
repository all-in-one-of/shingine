#pragma once
#include <map>
#include <string>
#include <iomanip>

#include "ISerialized.h"

template<typename T> ISerialized* createT() { return new T; }

class CSerializedFactory
{
public:
    typedef std::map<std::string, ISerialized*(*)()> TSerializedTypeMap;

    static ISerialized* CreateInstance(const std::string &s)
    {
        TSerializedTypeMap::iterator it = GetMap()->find(s);
        if (it == GetMap()->end()) return NULL;
        return it->second();
    }

protected:
    static TSerializedTypeMap *GetMap()
    {
        if (!Map) Map = new TSerializedTypeMap;
        return Map;
    }
private:
    static TSerializedTypeMap *Map;
};

template<typename T>
class CSerializedRegistry : CSerializedFactory
{
public:
    CSerializedRegistry(const CString &s)
    {
        GetMap()->insert(std::make_pair(s.GetStdString(), &createT<T>));
    }
};

#define REGISTER_SERIALIZED_TYPE(TYPENAME) \
    virtual CString SerializedName(); \
    static const CString SerializedNameVar; \
    static CSerializedRegistry<TYPENAME> reg; \
    \
    unsigned int SerializedUniqueID; \
    bool SerializedIDSet = false; \
    virtual unsigned int UniqueID() { return SerializedUniqueID; } \
    virtual void SetUniqueID(unsigned int newId) \
    { \
        if (!SerializedIDSet) \
        { \
            SerializedUniqueID = newId; \
            SerializedIDSet = true; \
        } \
        else \
            throw 1; \
    }
    

#define REGISTER_SERIALIZED_NAME(TYPENAME,NAME) \
    const CString TYPENAME::SerializedNameVar = #NAME; \
    CString TYPENAME::SerializedName() { return TYPENAME::SerializedNameVar; } \
    CSerializedRegistry<TYPENAME> TYPENAME::reg(TYPENAME::SerializedNameVar); \
