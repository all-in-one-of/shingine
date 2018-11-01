#include "Modules/Statics/Graphics.h"
#include "Engine/AssetTypes/Settings/RenderSettings.h"
#include "Modules/Graphics/OpenGL/OglCommandBuffer.h"
#include "Modules/Graphics/OpenGL/OpenGLRender.h"
#include "Modules/Statics/IAssetManager.h"

REGISTER_SERIALIZED_CLASS(Graphics)

bool Graphics::Render() {
  // create window if not created
  for (unsigned char x = 0; x < CommandBufferType::COUNT; x++) {
    CommandBuffers[x]->Execute();
    CommandBuffers[x]->ResetCommandBuffer();
  }
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
  for (unsigned int x = 0; x < CommandBufferType::COUNT; x++)
    CommandBuffers[x] = new OglCommandBuffer();
}

IRenderContext *Graphics::GetContext() { return RenderContext; }
ICommandBuffer *Graphics::GetCommandBuffer(IGraphics::CommandBufferType type) {
  return CommandBuffers[type];
}
void Graphics::SetDefaultShader(IShader *shader) { defaultShader = shader; }
IShader *Graphics::DefaultShader() { return defaultShader; }
