#pragma once
#include "IRenderContext.h"
class IShader;
class CRenderSettings;
class CGraphics
{
public:
    static IRenderContext* GetContext()
    {
        if (!Instance) 
            Instance = new CGraphics();
        return Instance->Context();
    }
    IRenderContext* Context();
    static void SetDefaultShader(IShader* shader);
    static IShader* DefaultShader();
    IShader* defaultShader;
private:
    CGraphics();
    static CGraphics* Instance;
    IRenderContext* RenderContext;
};
