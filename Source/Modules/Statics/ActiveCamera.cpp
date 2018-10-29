#include "Engine/IComponent.h"
#include "Modules/Graphics/Graphics.h"

#include "Modules/Statics/ComponentManager.h"
#include "Modules/Statics/EntityManager.h"

#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Modules/Statics/ActiveCamera.h"


ActiveCamera* ActiveCamera::Instance = NULL;

CameraComponent* ActiveCamera::GetCameraComponent()
{
    return Camera;
}

TransformComponent* ActiveCamera::GetTransformComponent()
{
    return Transform;
}

glm::mat4 ActiveCamera::ProjectionMatrix()
{
    return Instance->Camera->ProjectionMatrix;
}

glm::mat4 ActiveCamera::ViewMatrix()
{
    return Instance->Camera->ViewMatrix;
}

ActiveCamera::ActiveCamera()
{
    // find camera component or make a new entity
    ComponentManager* componentManager = ComponentManager::Get();
    Camera = componentManager->GetComponentOfType<CameraComponent>();
    if (!Camera)
    {
        unsigned int newId = 
            EntityManager::Get()->CreateEntity(
                {"TransformComponent", "CameraComponent", "ObjectMetadataComponent"});
        
        Camera = componentManager->GetComponentOfType<CameraComponent>(newId);
    }
    Transform =
        componentManager->GetComponentOfType<TransformComponent>(Camera->EntityId());

}
