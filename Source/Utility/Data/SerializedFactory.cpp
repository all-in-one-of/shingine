#include "Utility/Data/SerializedFactory.h"
#include "Modules/Statics/Statics.h"

SerializedFactory::TSerializedTypeMap *SerializedFactory::Map = nullptr;
SerializedFactory::TypeNameMap *SerializedFactory::TypeNames = nullptr;

void SerializedFactory::GetDemangledName(String &name) {
  TypeNameMap::iterator it = GetTypeNameMap()->find(name.GetCharArray());
  if (it == GetTypeNameMap()->end())
    name = "";
  name = String(it->second);
}

ISerialized *SerializedFactory::CreateInstance(const std::string &s,
                                               bool setUid) {
  TSerializedTypeMap::iterator it = GetMap()->find(s);
  if (it == GetMap()->end())
    return nullptr;

  ISerialized *createdInstance = it->second();
  IObject *serializedClass =
      dynamic_cast<IObject *>(createdInstance);
  if (!serializedClass)
    return createdInstance;
  // set unique id
  if (setUid) {
    unsigned int uniqueId = Statics::GetUniqueId();
    serializedClass->SetUniqueID(uniqueId);
    Statics::RegisterSerializedObject(serializedClass);

    // initialize classes
    std::vector<ISerialized *> serializedAttributes;
    serializedClass->GetAllAttributes(serializedAttributes);
  }
  // add to the instance manager
  return createdInstance;
}

SerializedFactory::TSerializedTypeMap *SerializedFactory::GetMap() {
  if (!Map)
    Map = new TSerializedTypeMap;
  return Map;
}

SerializedFactory::TypeNameMap *SerializedFactory::GetTypeNameMap() {
  if (!TypeNames)
    TypeNames = new TypeNameMap;
  return TypeNames;
}
