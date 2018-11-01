#include "Modules/Statics/Statics.h"
#include "Utility/Data/ISerialized.h"

#include "Engine/IComponent.h"
#include "Modules/Statics/IAssetManager.h"
#include "Modules/Statics/IComponentManager.h"

Statics *Statics::Instance = nullptr;

Statics::Statics() {}

unsigned int Statics::GetUniqueId() {
  Statics *instance = GetInstance();
  if ((instance->IdPool).size() == 0) {
    return instance->NextId++;
  }
  unsigned int id = instance->IdPool[instance->IdPool.size() - 1];
  (instance->IdPool).pop_back();
  return id;
}

void Statics::Destroy(ISerializedClass *object) {
  Statics *instance = GetInstance();
  (instance->IdPool).push_back(object->UniqueID());
  delete object;
}

void Statics::AddSerializedObject(ISerializedClass *object) {
  Statics *instance = GetInstance();
  IComponent *component = dynamic_cast<IComponent *>(object);
  if (component)
    instance->Get<IComponentManager>()->AddGenericComponent(component);
  else
    instance->Get<IAssetManager>()->AddInstance(object);
}
