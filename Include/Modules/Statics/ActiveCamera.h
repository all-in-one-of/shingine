#pragma once
#include <glm/glm.hpp>

class CameraComponent;
class TransformComponent;

class ActiveCamera
{
public:
    static ActiveCamera* Get() 
    {
        if (!Instance) Instance = new ActiveCamera();
        return Instance;
    }

    CameraComponent* GetCameraComponent();
    TransformComponent* GetTransformComponent();
    static glm::mat4 ProjectionMatrix();
    static glm::mat4 ViewMatrix();
private: 
    ActiveCamera();
    static ActiveCamera* Instance;
    TransformComponent* Transform = NULL;
    CameraComponent* Camera = NULL;
};
