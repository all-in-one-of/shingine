#include "Systems/RenderingSystem.h"

#include "Modules/Statics/Graphics.h"

#include "Modules/Statics/IAssetManager.h"
#include "Modules/Statics/IComponentManager.h"
#include "Modules/Statics/IEntityManager.h"


#include "Engine/AssetTypes/Settings/RenderSettings.h"

#include "Modules/Statics/IActiveCamera.h"
#include "Modules/Graphics/ICommandBuffer.h"
#include "Modules/Graphics/GraphicsUtils.h"

#include "Utility/Typedefs.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Components/LightComponent.h"
#include "Engine/Components/SkyLightComponent.h"
#include "Engine/Components/RendererComponent.h"
#include "Engine/Components/CameraComponent.h"

REGISTER_SERIALIZED_NAME(RenderingSystem)

typedef std::unordered_map<unsigned int, IComponent*> IComponentMap;

bool RenderingSystem::Initialize()
{
    Active = true;
    return Active;
}

bool RenderingSystem::Update()
{
    ActiveCommandBuffer = Statics::Get<IGraphics>()->CommandBuffer();

    ActiveCommandBuffer->EnableDepth();
    ActiveCommandBuffer->EnableCullFace();
    ActiveCommandBuffer->Clear();

    // Update projection matrix, then draw meshes
    CameraComponent* camera = Statics::Get<IActiveCamera>()->GetCameraComponent();
    camera->ProjectionMatrix = glm::perspective(camera->FOV,
        Statics::Get<IGraphics>()->GetContext()->GetFrameAspectRatio(), camera->NearPlane, camera->FarPlane);

    // Draw Skybox
    // find lights

    DrawSkyBox();
    DrawOpaqueMeshes();
    
    return true;
}

void RenderingSystem::DrawSkyBox()
{
    IComponentManager* componentManager = Statics::Get<IComponentManager>();
    SkyLightComponent* skyLight = componentManager->GetComponentOfType<SkyLightComponent>();

    // draw sky light here

}

void RenderingSystem::FindLights()
{
    for (unsigned char x = 0; x < MAX_LIGHTS; x++)
        LightComponents[x] = nullptr;
    
    LightsFound = 0;
    CachedDirectionalLight = nullptr;

    IComponentManager* componentManager = Statics::Get<IComponentManager>();
    IComponentManager::StringMap::iterator lightsIterator;
    componentManager->GetComponentIteratorOfType("LightComponent", lightsIterator);
    IComponentMap &lightsComponentMap = lightsIterator->second;
    IComponentMap::iterator entityIterator;

    for (entityIterator = lightsComponentMap.begin(); entityIterator != lightsComponentMap.end(); entityIterator++)
    {
        LightComponent* light = dynamic_cast<LightComponent*>(lightsComponentMap.at(entityIterator->first));
        
        if (light->LightType != DIRECTIONAL_LIGHT_TYPE && LightsFound != MAX_LIGHTS)
            LightComponents[LightsFound] = light;
        else if (light->LightType == DIRECTIONAL_LIGHT_TYPE && !CachedDirectionalLight)
            CachedDirectionalLight = light;
    }
}

void RenderingSystem::DrawOpaqueMeshes()
{
    IComponentManager::StringMap::iterator rendererIterator;
    IComponentManager::StringMap::iterator transformIterator;

    IComponentManager* componentManager = Statics::Get<IComponentManager>();
    // iterate over renderer iterator
    bool drawMeshes = true;
    drawMeshes = drawMeshes && componentManager->GetComponentIteratorOfType("RendererComponent", rendererIterator);
    drawMeshes = drawMeshes && componentManager->GetComponentIteratorOfType("TransformComponent", transformIterator);

    if (!drawMeshes) 
        return;


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
        // discard object if it's not in the view frustrum

        
        unsigned int shaderId;
        GraphicsUtils::SetUniformsFromMaterial(ActiveCommandBuffer, renderer->MaterialReference, shaderId);
        ActiveCommandBuffer->DrawMesh(transform->WorldTransform, transform->WorldTransformInv, renderer->MeshReference, shaderId);
    }
}

RenderingSystem::~RenderingSystem()
{
}
