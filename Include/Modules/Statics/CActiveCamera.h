#pragma once
#include <glm/glm.hpp>

class CCameraComponent;
class CTransformComponent;
class CActiveCamera
{
public:
    static CActiveCamera* Get() 
    {
        if (!Instance) Instance = new CActiveCamera();
        return Instance;
    }

    CCameraComponent* GetCameraComponent();
    CTransformComponent* GetTransformComponent();
    static glm::mat4 ProjectionMatrix();
    static glm::mat4 ViewMatrix();
private: 
    CActiveCamera();
    static CActiveCamera* Instance;
    CTransformComponent* TransformComponent;
    CCameraComponent* CameraComponent;
};
