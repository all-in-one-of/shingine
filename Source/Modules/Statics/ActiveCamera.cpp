#include "Engine/IComponent.h"
#include "Modules/Statics/IGraphics.h"

#include "Modules/Statics/IComponentManager.h"
#include "Modules/Statics/IEntityManager.h"

#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Modules/Statics/ActiveCamera.h"

REGISTER_SERIALIZED_NAME(ActiveCamera)
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
    return Camera->ProjectionMatrix;
}

glm::mat4 ActiveCamera::ViewMatrix()
{
    return Camera->ViewMatrix;
}

ActiveCamera::ActiveCamera()
{
    // find camera component or make a new entity
    IComponentManager* componentManager = Statics::Get<IComponentManager>();
    Camera = componentManager->GetComponentOfType<CameraComponent>();
    if (!Camera)
    {
        unsigned int newId = 
            Statics::Get<IEntityManager>()->CreateEntity(
                {"TransformComponent", "CameraComponent", "ObjectMetadataComponent"});
        
        Camera = componentManager->GetComponentOfType<CameraComponent>(newId);
    }
    Transform =
        componentManager->GetComponentOfType<TransformComponent>(Camera->EntityId());

}
