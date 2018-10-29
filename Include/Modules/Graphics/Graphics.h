#pragma once
#include "IRenderContext.h"
#include "ICommandBuffer.h"
class IShader;
class RenderSettings;

class Graphics
{
public:
    static bool Render();
    static IRenderContext* GetContext();
    static ICommandBuffer* CommandBuffer();
    static void SetDefaultShader(IShader* shader);
    static IShader* DefaultShader();
    IShader* defaultShader;
private:
    Graphics();
    void SetupWindow();
    static Graphics* GetInstance();
    static Graphics* Instance;
    IRenderContext* RenderContext = NULL;
    ICommandBuffer* DefaultCommandBuffer = NULL;
};
