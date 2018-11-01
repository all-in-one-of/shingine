#pragma once
#include "Statics.h"
class IShader;
class CRenderSettings;
class IRenderContext;
class ICommandBuffer;

class IGraphics {
public:
  enum CommandBufferType { FrameInit = 0, Main = 1, COUNT };
  virtual ~IGraphics() {}
  virtual bool Render() = 0;
  virtual IRenderContext *GetContext() = 0;
  virtual ICommandBuffer *GetCommandBuffer(CommandBufferType type) = 0;
  virtual void SetDefaultShader(IShader *shader) = 0;
  virtual IShader *DefaultShader() = 0;
  virtual void SetupWindow() = 0;
};
