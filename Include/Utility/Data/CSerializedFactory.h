#pragma once
#include <map>
#include <string>
#include <iomanip>

#include "ISerialized.h"

template<typename T> ISerialized* createT() 
{ 
    return new T; 
}

class CSerializedFactory
{
public:
    typedef std::map<std::string, ISerialized*(*)()> TSerializedTypeMap;
    static ISerialized* CreateInstance(const std::string &s, bool setUid=true);
protected:
    static TSerializedTypeMap* GetMap();
private:
    static TSerializedTypeMap *Map;
};

template<typename T>
class CSerializedRegistry : CSerializedFactory
{
public:
    CSerializedRegistry(const CString &friendlyName, const CString &typeName)
    {
        GetMap()->insert(std::make_pair("Class:" + typeName.GetStdString(), &createT<T>));
        GetMap()->insert(std::make_pair(friendlyName.GetStdString(), &createT<T>));
    }
};
