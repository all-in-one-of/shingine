#include "Modules/Statics/ComponentManager.h"
#include "Engine/IComponent.h"
#include "Utility/Data/ISerialized.h"
#include "Utility/Data/SerializedFactory.h"

#include "Utility/Data/IComponentMap.h"

REGISTER_SERIALIZED_CLASS(ComponentManager)

ComponentManager::ComponentManager() {}

void ComponentManager::AddGenericComponent(IComponent *component) {
  unsigned int entityId = component->EntityId();
  if (entityId == 0)
    entityId = component->Id();

  std::string typeName = dynamic_cast<ISerializedClass *>(component)
                             ->SerializedName()
                             .GetStdString();
  StringCompMap::iterator stringMapIterator = Components.find(typeName);
  IComponentMap *componentMap = nullptr;
  // first find out if there is  a map for this type
  if (stringMapIterator == Components.end()) {
    componentMap = AddComponentMap(typeName);
  } else
    componentMap = stringMapIterator->second;
  // replace the old component map with the new one
  componentMap->Set(entityId, component);
  // Components[typeName][entityId] = component;
}

IComponent *ComponentManager::AddComponent(String type, unsigned int entityId) {
  ISerialized *serializedObject =
      SerializedFactory::CreateInstance(type.GetStdString());
  if (!serializedObject)
    return nullptr;
  IComponent *component = dynamic_cast<IComponent *>(serializedObject);

  if (entityId != 0)
    component->SetEntityId(entityId);
  AddGenericComponent(component);
  return component;
}

IComponentMap *ComponentManager::GetComponentMap(const String &typeName) {
  std::string typeNameStdString = typeName.GetStdString();
  StringCompMap::iterator stringMapIterator =
      Components.find(typeNameStdString);
  if (stringMapIterator == Components.end())
    return AddComponentMap(typeNameStdString);
  return stringMapIterator->second;
}

IComponentMap *ComponentManager::AddComponentMap(const std::string &typeName) {
  ISerialized *newMap = SerializedFactory::CreateInstance(
      std::string("ComponentMap<") + typeName + ">");
  IComponentMap *componentMap = dynamic_cast<IComponentMap *>(newMap);
  Components[typeName] = componentMap;
  return componentMap;
}

void ComponentManager::UpdateComponentEntityId(IComponent *component) {
  // get component name
  ISerializedClass *serializedComponent =
      dynamic_cast<ISerializedClass *>(component);
  unsigned int uniqueId = serializedComponent->UniqueID();

  std::string serializedName =
      serializedComponent->SerializedName().GetStdString();

  IComponentMap *componentMap = Components[serializedName];
  componentMap->Erase(uniqueId);
  componentMap->Set(component->EntityId(), component);
}

void ComponentManager::GetAllComponents(std::vector<IComponent *> &components) {
  for (StringCompMap::iterator it = Components.begin(); it != Components.end();
       it++) {
    IComponentMap *componentMap = it->second;
    componentMap->GetAllComponents(components);
  }
}
