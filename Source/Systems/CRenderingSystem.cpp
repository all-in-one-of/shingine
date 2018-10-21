#include "Systems/CRenderingSystem.h"
#include "Engine/AssetTypes/Settings/CRenderSettings.h"
#include "Modules/Statics/CStatics.h"
#include "Modules/Graphics/COpenGLRenderer.h"

void CRenderingSystem::Initialize()
{
    // get render settings
    CRenderSettings* renderSettings = dynamic_cast<CRenderSettings*> (CStatics::InstanceManager()->GetAssetOfType("RenderSettings"));
    if (!renderSettings)
    {
        // default render settings;
        renderSettings = new CRenderSettings();
        CStatics::InstanceManager()->AddInstance(renderSettings);
    }

    if (!Renderer)
    {
        Renderer = new COpenGLRender();
        Renderer->Create(renderSettings->ScreenWidth, renderSettings->ScreenHeight, renderSettings->WindowTitle);
    }
}

void CRenderingSystem::Update()
{
    IComponentIterator rendererIterator;
    IComponentIterator transformIterator;

    CInstanceManager* instanceManager = CStatics::InstanceManager();
    // iterate over renderer iterator
    instanceManager->GetComponentIteratorOfType("Renderer", rendererIterator);
    instanceManager->GetComponentIteratorOfType("Transform", transformIterator);
    // get camera component
    IComponent* cameraComponent = instanceManager->GetComponentOfType("Camera");
    if (!cameraComponent)
    {
        unsigned int newId = instanceManager->AddEntity();
        

    }

    std::unordered_map<unsigned int, IComponent*>::iterator entityIterator;
    for (entityIterator = rendererIterator->second.begin(); entityIterator != rendererIterator->second.end(); entityIterator++)
    {
        unsigned int entityId = entityIterator->first;

        IComponent* transform = transformIterator->second.at(entityId);

    }
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
