#pragma once
#include "Core.h"
class IRenderContext
{
public:
    virtual ~IRenderContext() {};
    virtual void Create(unsigned short width, unsigned short height, const CString& title) = 0;

    virtual void DrawMesh(glm::mat4 &matrix, unsigned int &meshAssetId, unsigned int &materialId) = 0;
    virtual void DrawMesh(glm::mat4 &matrix, glm::mat4 &matrixInv, unsigned int &meshAssetId, unsigned int &materialId) = 0;
    virtual void UpdateFrame() = 0;

    virtual bool WindowShouldClose() = 0;

    virtual void GetWindowFramebufferSize(int &width, int &height) = 0;
    virtual bool IsWindowCreated() = 0;
    
    virtual void BeginDrawingLoop() = 0;
    virtual void EndDrawingLoop() = 0;
};
