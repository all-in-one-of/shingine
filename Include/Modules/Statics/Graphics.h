#pragma once
#include "IGraphics.h"
#include "Modules/Graphics/ICommandBuffer.h"
#include "Modules/Graphics/IRenderContext.h"
#include "Utility/Data/Serialization.h"

class IShader;
class RenderSettings;

class Graphics : public IGraphics, public ISerializedClass {
public:
  SERIALIZE_CLASS(Graphics);
  Graphics();
  virtual ~Graphics() {}
  virtual bool Render();
  virtual IRenderContext *GetContext();
  virtual ICommandBuffer *GetCommandBuffer(CommandBufferType type);
  virtual void SetDefaultShader(IShader *shader);
  virtual IShader *DefaultShader();
  virtual void SetupWindow();

private:
  IShader *defaultShader;
  IRenderContext *RenderContext = nullptr;
  ICommandBuffer *CommandBuffers[CommandBufferType::COUNT];
};
