#include "Systems/CRenderingSystem.h"

#include "Modules/Graphics/CGraphics.h"

#include "Modules/Statics/CAssetManager.h"
#include "Modules/Statics/CComponentManager.h"
#include "Modules/Statics/CEntityManager.h"

#include "Engine/Components/CTransformComponent.h"
#include "Engine/Components/CRendererComponent.h"

#include "Engine/AssetTypes/Settings/CRenderSettings.h"

REGISTER_SERIALIZED_NAME(CRenderingSystem, RenderingSystem)
bool CRenderingSystem::Initialize()
{
    // creates a window
    Renderer = CGraphics::GetContext();
    CRenderSettings* renderSettings = CAssetManager::Get()->GetAssetOfType<CRenderSettings>("RenderSettings");
    Renderer->Create(renderSettings->ScreenWidth, renderSettings->ScreenHeight, renderSettings->WindowTitle);
    Active = Renderer->IsWindowCreated();
    return Active;
}

bool CRenderingSystem::Update()
{
    if (Renderer->WindowShouldClose())
        return false;

    Renderer->BeginDrawingLoop();

    CComponentManager::StringMap::iterator rendererIterator;
    CComponentManager::StringMap::iterator transformIterator;

    CComponentManager* componentManager = CComponentManager::Get();
    // iterate over renderer iterator
    bool drawMeshes = true;
    drawMeshes = drawMeshes && componentManager->GetComponentIteratorOfType("Renderer", rendererIterator);
    drawMeshes = drawMeshes && componentManager->GetComponentIteratorOfType("Transform", transformIterator);

    if (drawMeshes)
    {
        typedef std::unordered_map<unsigned int, IComponent*> IComponentMap;

        IComponentMap &rendererComponentMap = rendererIterator->second;
        IComponentMap::iterator entityIterator;
        
        for (entityIterator = rendererComponentMap.begin(); entityIterator != rendererComponentMap.end(); entityIterator++)
        {
            unsigned int entityId = entityIterator->first;
            IComponentMap &transformComponentMap = transformIterator->second;
            CTransformComponent* transform = dynamic_cast<CTransformComponent*>(transformComponentMap.at(entityId));
            CRendererComponent* renderer = dynamic_cast<CRendererComponent*>(entityIterator->second);
            if (!transform || !renderer)
                continue;
            
            Renderer->DrawMesh(transform->WorldTransform, transform->WorldTransformInv, renderer->MeshReference, renderer->MaterialReference);
        }
    }
    
    
    Renderer->EndDrawingLoop();
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
