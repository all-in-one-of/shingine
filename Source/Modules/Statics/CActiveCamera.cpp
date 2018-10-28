#include "Engine/IComponent.h"
#include "Modules/Statics/CActiveCamera.h"
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
    return Instance->CameraComponent->ProjectionMatrix;
}

glm::mat4 CActiveCamera::ViewMatrix()
{
    return Instance->CameraComponent->ViewMatrix;
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