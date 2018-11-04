#include "Modules/Utility/SceneUtils.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Modules/Statics/IComponentManager.h"
#include "Modules/Statics/IEntityManager.h"

namespace SceneUtils {
void GetActiveCamera(CameraComponent *&camera, TransformComponent *&transform) {
  camera = GetActiveCamera();
  transform =
      Statics::Get<IComponentManager>()->GetComponentOfType<TransformComponent>(
          camera->EntityId());
}

CameraComponent *GetActiveCamera() {
  IComponentManager *componentManager = Statics::Get<IComponentManager>();
  ComponentMap<CameraComponent> *cameraComponentMap =
      componentManager->GetComponentMap<CameraComponent>();
  CameraComponent *camera = nullptr;
  for (unsigned int x = 0; x < cameraComponentMap->Count(); x++) {
    CameraComponent *camera = cameraComponentMap->AtIndex(x);
    if (camera->IsCurrent())
      return camera;
  }
  if (!camera) {
    camera = CreateDefaultCamera();
    return camera;
  }
  camera->SetCurrent(true);
  return camera;
}

CameraComponent *CreateDefaultCamera() {
  IComponentManager *componentManager = Statics::Get<IComponentManager>();
  CameraComponent *camera = nullptr;
  unsigned int newId = Statics::Get<IEntityManager>()->CreateEntity(
      {"TransformComponent", "CameraComponent", "ObjectMetadataComponent"});

  camera = componentManager->GetComponentOfType<CameraComponent>(newId);
  camera->SetCurrent(true);
  return camera;
}

}; // namespace SceneUtils
