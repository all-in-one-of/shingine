#include "Modules/Statics/Graphics.h"
#include "Engine/AssetTypes/Settings/RenderSettings.h"
#include "Modules/Graphics/OpenGL/OglCommandBuffer.h"
#include "Modules/Graphics/OpenGL/OpenGLRender.h"
#include "Modules/Statics/IAssetManager.h"

REGISTER_SERIALIZED_CLASS(Graphics)

bool Graphics::Render() {
  // create window if not created

  DefaultCommandBuffer->Execute();
  DefaultCommandBuffer->ResetCommandBuffer();
  // Finalize rendering
  RenderContext->Update();
  return !RenderContext->WindowShouldClose();
}

void Graphics::SetupWindow() {
  RenderSettings *renderSettings =
      Statics::Get<IAssetManager>()->GetAssetOfType<RenderSettings>();
  RenderContext->Create(renderSettings->ScreenWidth,
                        renderSettings->ScreenHeight,
                        renderSettings->WindowTitle);
}

Graphics::Graphics() {
  // get render settings
  RenderSettings *renderSettings =
      Statics::Get<IAssetManager>()->GetAssetOfType<RenderSettings>();

  if (!renderSettings)
    Statics::Get<IAssetManager>()->AddAssetOfType<RenderSettings>();

  // TODO make a factory class for making render context
  RenderContext = new OpenGLRender();
  DefaultCommandBuffer = new OglCommandBuffer();
}

IRenderContext *Graphics::GetContext() { return RenderContext; }

ICommandBuffer *Graphics::CommandBuffer() { return DefaultCommandBuffer; }

void Graphics::SetDefaultShader(IShader *shader) { defaultShader = shader; }

IShader *Graphics::DefaultShader() { return defaultShader; }
