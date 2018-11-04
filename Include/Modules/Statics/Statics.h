#pragma once
#include "Utility/Data/SerializedFactory.h"
#include <vector>

class IObject;
class String;

class Statics {
public:
  static void AddStaticObject(const String &interfaceName,
                              const String &typeName);

  // this is called once a new instance of ISerializedClass is created
  // which obtained a unique id
  static void RegisterSerializedObject(IObject *object);
  static IObject *FindSerializedObject(unsigned int objectId);

  // Checks wheter it's a component or not
  static void AddSerializedObject(IObject *object);
  static void Destroy(IObject *object);
  static unsigned int GetUniqueId();
  static void ReturnUniqueId(unsigned int uid);

  template <typename InterfaceTypeName> static InterfaceTypeName *Get() {
    Statics *instance = GetInstance();
    String typeName = typeid(InterfaceTypeName).name();
    StaticObjectMap::iterator it = instance->StaticObjects.find(typeName);

    if (it == instance->StaticObjects.end()) {
      printf("Couldn't find static object of type : %s",
             typeName.GetCharArray());
      return nullptr;
    }
    return dynamic_cast<InterfaceTypeName *>(it->second);
  }

  template <typename InterfaceTypeName, typename TypeName>
  static void AddStaticObject() {
    Statics *instance = GetInstance();

    String interfaceName = typeid(InterfaceTypeName).name();
    String typeName = typeid(TypeName).name();
    SerializedFactory::GetDemangledName(typeName);
    IObject *newObject = dynamic_cast<IObject *>(
        SerializedFactory::CreateInstance(typeName));
    instance->StaticObjects[interfaceName] = newObject;
  }

private:
  Statics();
  static Statics *GetInstance() {
    if (!Instance)
      Instance = new Statics();
    return Instance;
  }

  static Statics *Instance;
  std::vector<unsigned int> IdPool;
  unsigned int NextId = 1000;

  typedef std::unordered_map<unsigned int, IObject *>
      GlobalRegistryMap;
  typedef std::unordered_map<std::string, IObject *> StaticObjectMap;
  StaticObjectMap StaticObjects;
  GlobalRegistryMap SerializedObjects;
};
