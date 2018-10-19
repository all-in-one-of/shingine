#pragma once
#include "IRenderContext.h"

class COpenGLRender : public IRenderContext
{
public:
    COpenGLRender();
    virtual ~COpenGLRender();
    virtual void Create(unsigned short width, unsigned short height, const CString& title);
    virtual void UpdateFrame();
private:
    struct GLFWwindow* Window;
};
