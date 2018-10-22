#include "Modules/Graphics/OpenGL/COpenGLRenderer.h"
#include "Modules/Graphics/CGraphics.h"
#include "Engine/AssetTypes/Settings/CRenderSettings.h"
#include "Modules/Statics/CAssetManager.h"

CGraphics* CGraphics::Instance = NULL;

CGraphics::CGraphics(){}

IRenderContext* CGraphics::Context()
{
    if (RenderContext)
        return RenderContext;

    // get render settings
    CRenderSettings* renderSettings = 
        CAssetManager::Get()->GetAssetOfType<CRenderSettings>("RenderSettings");

    if (!renderSettings)
    {
        // default render settings;
        renderSettings = 
            CAssetManager::Get()->AddAssetOfType<CRenderSettings>("RenderSettings");
    }
    // TODO make a factory class for making render context
    if (!RenderContext)
    {
        RenderContext = new COpenGLRender();
        RenderContext->Create(renderSettings->ScreenWidth, renderSettings->ScreenHeight, renderSettings->WindowTitle);
    }
        return RenderContext;
}

void CGraphics::SetDefaultShader(IShader* shader)
{
    Instance->defaultShader = shader;
}

IShader* CGraphics::DefaultShader()
{
    return Instance->defaultShader;
}