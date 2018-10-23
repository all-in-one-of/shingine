#include "Modules/Graphics/OpenGL/COpenGLRender.h"
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
        RenderContext = new COpenGLRender();
        
    return RenderContext;
}

void CGraphics::SetDefaultShader(IShader* shader)
{
    if(Instance == NULL) Instance = new CGraphics();
    Instance->defaultShader = shader;
}

IShader* CGraphics::DefaultShader()
{
    if(Instance == NULL) Instance = new CGraphics();
    return Instance->defaultShader;
}