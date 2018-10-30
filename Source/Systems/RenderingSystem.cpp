#include "Systems/RenderingSystem.h"

#include "Modules/Statics/Graphics.h"

#include "Modules/Statics/IAssetManager.h"
#include "Modules/Statics/IComponentManager.h"
#include "Modules/Statics/IEntityManager.h"

#include "Engine/Components/TransformComponent.h"
#include "Engine/Components/RendererComponent.h"
#include "Engine/Components/CameraComponent.h"

#include "Engine/AssetTypes/Settings/RenderSettings.h"

#include "Modules/Statics/IActiveCamera.h"
#include "Modules/Graphics/ICommandBuffer.h"
#include "Modules/Graphics/GraphicsUtils.h"

#include "Utility/Typedefs.h"

REGISTER_SERIALIZED_NAME(RenderingSystem)
bool RenderingSystem::Initialize()
{
    Active = true;
    return Active;
}

bool RenderingSystem::Update()
{
    ICommandBuffer* buf = Statics::Get<IGraphics>()->CommandBuffer();

    buf->EnableDepth();
    buf->EnableCullFace();
    buf->Clear();

    // Update projection matrix, then draw meshes
    CameraComponent* camera = Statics::Get<IActiveCamera>()->GetCameraComponent();
    camera->ProjectionMatrix = glm::perspective(camera->FOV,
        Statics::Get<IGraphics>()->GetContext()->GetFrameAspectRatio(), camera->NearPlane, camera->FarPlane);

    IComponentManager::StringMap::iterator rendererIterator;
    IComponentManager::StringMap::iterator transformIterator;

    IComponentManager* componentManager = Statics::Get<IComponentManager>();
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
            
            // TODO cache material if it repeats
            
            unsigned int shaderId;
            GraphicsUtils::SetUniformsFromMaterial(buf, renderer->MaterialReference, shaderId);
            buf->DrawMesh(transform->WorldTransform, transform->WorldTransformInv, renderer->MeshReference, shaderId);
        }
    }
    return true;
}

RenderingSystem::~RenderingSystem()
{
}
