#pragma once
#include "Modules/Graphics/IRenderContext.h"

class COpenGLRender : public IRenderContext
{
public:
    COpenGLRender();
    virtual ~COpenGLRender();
    virtual void Create(unsigned short width, unsigned short height, const CString& title);
    virtual void DrawMesh(glm::mat4 matrix, unsigned int meshAssetId, unsigned int materialId);
    virtual void UpdateFrame();
private:
    struct GLFWwindow* Window;
};
