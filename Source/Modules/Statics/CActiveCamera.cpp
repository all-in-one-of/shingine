#include "Modules/Statics/CActiveCamera.h"
#include "Engine/IComponent.h"
#include "Modules/Graphics/CGraphics.h"

#include "Engine/Components/CCameraComponent.h"
#include "Engine/Components/CTransformComponent.h"

#include "Modules/Statics/CComponentManager.h"
#include "Modules/Statics/CEntityManager.h"

CActiveCamera* CActiveCamera::Instance = NULL;
CCameraComponent* CActiveCamera::GetCameraComponent()
{
    return CameraComponent;
}

CTransformComponent* CActiveCamera::GetTransformComponent()
{
    return TransformComponent;
}

glm::mat4 CActiveCamera::ProjectionMatrix()
{
    CCameraComponent* cameraComponent = CActiveCamera::Get()->GetCameraComponent();
    int width, height;
    CGraphics::GetContext()->GetWindowFramebufferSize(width, height);
    return glm::perspective(cameraComponent->FOV, width / (float) height,
        cameraComponent->NearPlane, cameraComponent->FarPlane);
}

glm::mat4 CActiveCamera::ViewMatrix()
{
    CTransformComponent* transformComponent = CActiveCamera::Get()->GetTransformComponent();
    return transformComponent->WorldTransform;
}

CActiveCamera::CActiveCamera()
{
    // find camera component or make a new entity
    CComponentManager* componentManager = CComponentManager::Get();
    CameraComponent = componentManager->GetComponentOfType<CCameraComponent>("Camera");
    if (!CameraComponent)
    {
        unsigned int newId = 
            CEntityManager::Get()->CreateEntity({"Transform", "Camera", "ObjectMetadata"});
        CameraComponent = componentManager->GetComponentOfType<CCameraComponent>("Camera", newId);
    }
    TransformComponent = 
        componentManager->GetComponentOfType<CTransformComponent>("Transform", CameraComponent->EntityId());

}