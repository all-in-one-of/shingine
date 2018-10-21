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
    static ISerialized* CreateInstance(const std::string &s);
protected:
    static TSerializedTypeMap* GetMap();
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
