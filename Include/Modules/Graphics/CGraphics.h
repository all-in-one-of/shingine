#pragma once
#include "IRenderContext.h"
#include "ICommandBuffer.h"
class IShader;
class CRenderSettings;

class CGraphics
{
public:
    static bool Render();
    static IRenderContext* GetContext();
    static ICommandBuffer* CommandBuffer();
    static void SetDefaultShader(IShader* shader);
    static IShader* DefaultShader();
    IShader* defaultShader;
private:
    CGraphics();
    void SetupWindow();
    static CGraphics* GetInstance();
    static CGraphics* Instance;
    IRenderContext* RenderContext = NULL;
    ICommandBuffer* DefaultCommandBuffer = NULL;
};
