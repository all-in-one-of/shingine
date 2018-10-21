#pragma once
#include <vector>

class ISerializedClass;
class CStatics
{
public:
    static CStatics* Get() 
    {
        if (!Instance) Instance = new CStatics();
        return Instance;
    }
    void AddSerializedObject(ISerializedClass* object);
    void Destroy(ISerializedClass* object);
    unsigned int GetUniqueId();
private:
    CStatics();
    static CStatics* Instance;
    std::vector<unsigned int> IdPool;
    unsigned int NextId = 1000;

};
