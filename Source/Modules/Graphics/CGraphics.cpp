#include "Modules/Graphics/OpenGL/COpenGLRender.h"
#include "Modules/Graphics/CGraphics.h"
#include "Engine/AssetTypes/Settings/CRenderSettings.h"
#include "Modules/Statics/CAssetManager.h"
#include "Modules/Graphics/OpenGL/COglCommandBuffer.h"

CGraphics* CGraphics::Instance = NULL;

bool CGraphics::Render()
{
    // create window if not created
    CGraphics* gr = GetInstance();

    gr->DefaultCommandBuffer->Execute();
    gr->DefaultCommandBuffer->ResetCommandBuffer();
    // Finalize rendering
    gr->RenderContext->Update();
    return !gr->RenderContext->WindowShouldClose();
}

void CGraphics::SetupWindow()
{
    CRenderSettings* renderSettings = CAssetManager::Get()->GetAssetOfType<CRenderSettings>("RenderSettings");
    RenderContext->Create(
        renderSettings->ScreenWidth, renderSettings->ScreenHeight, renderSettings->WindowTitle);
}

CGraphics::CGraphics()
{
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
    RenderContext = new COpenGLRender();
    DefaultCommandBuffer = new COglCommandBuffer();
}

CGraphics* CGraphics::GetInstance()
{
    if (!Instance) 
    {
        Instance = new CGraphics();
        Instance->SetupWindow();
    }
    return Instance;
}

IRenderContext* CGraphics::GetContext()
{
    return GetInstance()->RenderContext;
}

ICommandBuffer* CGraphics::CommandBuffer()
{
    return GetInstance()->DefaultCommandBuffer;
}

void CGraphics::SetDefaultShader(IShader* shader)
{
    GetInstance()->defaultShader = shader;
}

IShader* CGraphics::DefaultShader()
{
    return GetInstance()->defaultShader;
}