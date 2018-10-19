#include "Systems/CRenderingSystem.h"
#include "Engine/AssetTypes/Settings/CRenderSettings.h"
#include "Modules/Statics/CStatics.h"
#include "Modules/Graphics/COpenGLRenderer.h"

void CRenderingSystem::Initialize()
{
    // get render settings
    CRenderSettings* renderSettings = dynamic_cast<CRenderSettings*> (CStatics::AssetManager()->GetAssetOfType("RenderSettings"));
    if (!renderSettings)
    {
        // default render settings;
        renderSettings = new CRenderSettings();
        CStatics::AssetManager()->AddAsset(renderSettings);
    }

    if (!Renderer)
    {
        Renderer = new COpenGLRender();
        Renderer->Create(renderSettings->ScreenWidth, renderSettings->ScreenHeight, renderSettings->WindowTitle);
    }
    // collect objects by rendering x transform object
    IScene* currentScene = CStatics::SceneManager()->CurrentScene();
    

}

void CRenderingSystem::Update()
{
    // takes each object in the list, gets its transform
    // calculates resulting transform 
        // if it did change
        // goes through all parents to check that
    // takes cached one
    // renders (material id, mesh it, matrix)
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
