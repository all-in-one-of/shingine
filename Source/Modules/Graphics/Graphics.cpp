#include "Modules/Graphics/OpenGL/OpenGLRender.h"
#include "Modules/Graphics/Graphics.h"
#include "Engine/AssetTypes/Settings/RenderSettings.h"
#include "Modules/Statics/AssetManager.h"
#include "Modules/Graphics/OpenGL/OglCommandBuffer.h"

Graphics* Graphics::Instance = NULL;

bool Graphics::Render()
{
    // create window if not created
    Graphics* gr = GetInstance();

    gr->DefaultCommandBuffer->Execute();
    gr->DefaultCommandBuffer->ResetCommandBuffer();
    // Finalize rendering
    gr->RenderContext->Update();
    return !gr->RenderContext->WindowShouldClose();
}

void Graphics::SetupWindow()
{
    RenderSettings* renderSettings = AssetManager::Get()->GetAssetOfType<RenderSettings>();
    RenderContext->Create(
        renderSettings->ScreenWidth, renderSettings->ScreenHeight, renderSettings->WindowTitle);
}

Graphics::Graphics()
{
    // get render settings
    RenderSettings* renderSettings = 
        AssetManager::Get()->GetAssetOfType<RenderSettings>();

    if (!renderSettings)
        AssetManager::Get()->AddAssetOfType<RenderSettings>();
    
    // TODO make a factory class for making render context
    RenderContext = new OpenGLRender();
    DefaultCommandBuffer = new OglCommandBuffer();
}

Graphics* Graphics::GetInstance()
{
    if (!Instance) 
    {
        Instance = new Graphics();
        Instance->SetupWindow();
    }
    return Instance;
}

IRenderContext* Graphics::GetContext()
{
    return GetInstance()->RenderContext;
}

ICommandBuffer* Graphics::CommandBuffer()
{
    return GetInstance()->DefaultCommandBuffer;
}

void Graphics::SetDefaultShader(IShader* shader)
{
    GetInstance()->defaultShader = shader;
}

IShader* Graphics::DefaultShader()
{
    return GetInstance()->defaultShader;
}
