#include "Systems/RenderingSystem.h"

#include "Modules/Graphics/Graphics.h"

#include "Modules/Statics/AssetManager.h"
#include "Modules/Statics/ComponentManager.h"
#include "Modules/Statics/EntityManager.h"

#include "Engine/Components/TransformComponent.h"
#include "Engine/Components/RendererComponent.h"
#include "Engine/Components/CameraComponent.h"

#include "Engine/AssetTypes/Settings/RenderSettings.h"

#include "Modules/Statics/ActiveCamera.h"
#include "Modules/Graphics/ICommandBuffer.h"

#include "Utility/Typedefs.h"

REGISTER_SERIALIZED_NAME(RenderingSystem)
bool RenderingSystem::Initialize()
{
    Active = true;
    return Active;
}

bool RenderingSystem::Update()
{
    ICommandBuffer* buf = Graphics::CommandBuffer();

    buf->EnableDepth();
    buf->EnableCullFace();
    buf->Clear();

    // Update projection matrix, then draw meshes
    CameraComponent* camera = ActiveCamera::Get()->GetCameraComponent();
    camera->ProjectionMatrix = glm::perspective(camera->FOV,
        Graphics::GetContext()->GetFrameAspectRatio(), camera->NearPlane, camera->FarPlane);

    ComponentManager::StringMap::iterator rendererIterator;
    ComponentManager::StringMap::iterator transformIterator;

    ComponentManager* componentManager = ComponentManager::Get();
    // iterate over renderer iterator
    bool drawMeshes = true;
    drawMeshes = drawMeshes && componentManager->GetComponentIteratorOfType("RendererComponent", rendererIterator);
    drawMeshes = drawMeshes && componentManager->GetComponentIteratorOfType("TransformComponent", transformIterator);

    if (drawMeshes)
    {
        typedef std::unordered_map<unsigned int, IComponent*> IComponentMap;

        IComponentMap &rendererComponentMap = rendererIterator->second;
        IComponentMap::iterator entityIterator;
        
        for (entityIterator = rendererComponentMap.begin(); entityIterator != rendererComponentMap.end(); entityIterator++)
        {
            unsigned int entityId = entityIterator->first;
            IComponentMap &transformComponentMap = transformIterator->second;
            TransformComponent* transform = dynamic_cast<TransformComponent*>(transformComponentMap.at(entityId));
            RendererComponent* renderer = dynamic_cast<RendererComponent*>(entityIterator->second);
            if (!transform || !renderer)
                continue;
            
            buf->DrawMesh(transform->WorldTransform, transform->WorldTransformInv, renderer->MeshReference, renderer->MaterialReference);
        }
    }
    return true;
}

RenderingSystem::~RenderingSystem()
{
}
