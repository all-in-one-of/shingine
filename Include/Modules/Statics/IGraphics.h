#pragma once
#include "Statics.h"
class IShader;
class CRenderSettings;
class IRenderContext;
class ICommandBuffer;

class IGraphics
{
public:
    virtual ~IGraphics() {}
    virtual bool Render() = 0;
    virtual IRenderContext* GetContext() = 0;
    virtual ICommandBuffer* CommandBuffer() = 0;
    virtual void SetDefaultShader(IShader* shader) = 0;
    virtual IShader* DefaultShader() = 0;
    virtual void SetupWindow() = 0;
};
