#pragma once

class CameraComponent;
class TransformComponent;
namespace SceneUtils {
CameraComponent *GetActiveCamera();
void GetActiveCamera(CameraComponent *&camera, TransformComponent *&transform);
CameraComponent *CreateDefaultCamera();
}; // namespace SceneUtils