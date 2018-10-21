#pragma once
#include <map>
class ISerialized;
class CEntityIdCollection;
class CEntityComponentIdSetter
{
public:
    static void UpdateIds(ISerialized* idCollections);
private:
    CEntityComponentIdSetter() {};
    CEntityComponentIdSetter(CEntityIdCollection* collection);
};
