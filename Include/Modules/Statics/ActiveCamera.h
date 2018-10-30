#pragma once
#include "IActiveCamera.h"
#include "Utility/Data/Serialization.h"

class ActiveCamera : public IActiveCamera, public ISerializedClass
{
public:
    SERIALIZE_CLASS(ActiveCamera)
    ActiveCamera();
    virtual ~ActiveCamera() {};
    virtual CameraComponent* GetCameraComponent();
    virtual TransformComponent* GetTransformComponent();
    virtual glm::mat4 ProjectionMatrix();
    virtual glm::mat4 ViewMatrix();

    void SetComponents();
private: 
    TransformComponent* Transform = nullptr;
    CameraComponent* Camera = nullptr;
    bool AreComponentsSet = false;
};
