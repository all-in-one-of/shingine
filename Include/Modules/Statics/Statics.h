#pragma once
#include <vector>
#include "Utility/Data/SerializedFactory.h"

class ISerializedClass;
class String;
class Statics
{
public:
    static void AddStaticObject(const String &interfaceName, const String &typeName);

    static void AddSerializedObject(ISerializedClass* object);
    static void Destroy(ISerializedClass* object);
    static unsigned int GetUniqueId();

    template <typename InterfaceTypeName>
    static InterfaceTypeName* Get()
    {
        Statics* instance = GetInstance();
        String typeName = typeid(InterfaceTypeName).name();
        StaticObjectMap::iterator it = instance->StaticObjects.find(typeName.GetStdString());

        if (it == instance->StaticObjects.end())
        {
            printf("Couldn't find static object of type : %s", typeName.GetCharArray());
            return nullptr;
        }
        return dynamic_cast<InterfaceTypeName*>(it->second);
    }

    template <typename InterfaceTypeName, typename TypeName>
    static void AddStaticObject()
    {
        Statics* instance = GetInstance();

        String interfaceName = typeid(InterfaceTypeName).name();
        String typeName = typeid(TypeName).name();
        SerializedFactory::GetDemangledName(typeName);
        ISerializedClass* newObject = dynamic_cast<ISerializedClass*>(SerializedFactory::CreateInstance(typeName.GetStdString()));
        instance->StaticObjects[interfaceName.GetStdString()] = newObject;
    }
    
private:
    Statics();
    static Statics* GetInstance() 
    {
        if (!Instance) Instance = new Statics();
        return Instance;
    }

    static Statics* Instance;
    std::vector<unsigned int> IdPool;
    unsigned int NextId = 1000;

    typedef std::unordered_map<std::string, ISerializedClass*> StaticObjectMap;
    StaticObjectMap StaticObjects;
};
