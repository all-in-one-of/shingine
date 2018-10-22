#include "Systems/CRenderingSystem.h"

#include "Modules/Graphics/CGraphics.h"

#include "Modules/Statics/CAssetManager.h"
#include "Modules/Statics/CComponentManager.h"
#include "Modules/Statics/CEntityManager.h"

#include "Engine/Components/CTransformComponent.h"
#include "Engine/Components/CRendererComponent.h"

REGISTER_SERIALIZED_NAME(CRenderingSystem, RenderingSystem)
bool CRenderingSystem::Initialize()
{
    Renderer = CGraphics::GetContext();
    return true;
}

bool CRenderingSystem::Update()
{
    CComponentManager::StringMap::iterator rendererIterator;
    CComponentManager::StringMap::iterator transformIterator;

    CComponentManager* componentManager = CComponentManager::Get();
    // iterate over renderer iterator
    componentManager->GetComponentIteratorOfType("Renderer", rendererIterator);
    componentManager->GetComponentIteratorOfType("Transform", transformIterator);
    // get camera component
    IComponent* cameraComponent = componentManager->GetComponentOfType("Camera");
    if (!cameraComponent)
    {
        unsigned int newId = 
            CEntityManager::Get()->CreateEntity({"Transform", "Camera", "ObjectMetadata"});
        cameraComponent = componentManager->GetComponentOfType("Camera", newId);
    }

    std::unordered_map<unsigned int, IComponent*>::iterator entityIterator;
    for (entityIterator = rendererIterator->second.begin(); entityIterator != rendererIterator->second.end(); entityIterator++)
    {
        unsigned int entityId = entityIterator->first;
        CTransformComponent* transform = dynamic_cast<CTransformComponent*>(transformIterator->second.at(entityId));
        CRendererComponent* renderer = dynamic_cast<CRendererComponent*>(entityIterator->second);
        if (!transform || !renderer) 
            continue;
            
        Renderer->DrawMesh(transform->WorldTransform, renderer->MeshReference, renderer->MaterialReference);
        //
    }
    return true;
}

CRenderingSystem::~CRenderingSystem()
{
    delete Renderer;
}

// the rendering backend
// checks shader first
// compiles if not backs to default
// find mesh bind to this material id
// renders it with the matrix and shader uniforms collected from material
