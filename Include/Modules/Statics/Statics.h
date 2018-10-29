#pragma once
#include <vector>

class ISerializedClass;
class Statics
{
public:
    static Statics* Get() 
    {
        if (!Instance) Instance = new Statics();
        return Instance;
    }
    void AddSerializedObject(ISerializedClass* object);
    void Destroy(ISerializedClass* object);
    unsigned int GetUniqueId();
private:
    Statics();
    static Statics* Instance;
    std::vector<unsigned int> IdPool;
    unsigned int NextId = 1000;
};
