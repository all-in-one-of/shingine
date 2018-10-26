#pragma once
#include "Modules/Graphics/IRenderContext.h"
#include <string>
#include <unordered_map>

class CVaoMeshManager;
class COglShaderManager;
class ISerializedClass;

class COpenGLRender : public IRenderContext
{
public:
    COpenGLRender();
    virtual ~COpenGLRender();
    virtual void Create(unsigned short width, unsigned short height, const CString& title);

    virtual bool WindowShouldClose();
    virtual void GetWindowFramebufferSize(int &width, int &height);
    virtual float GetFrameAspectRatio();
    virtual void SetFramebufferSize(int &width, int &height);
    virtual bool IsWindowCreated();
    virtual void Update();

    // // pre-defined uniform names 
    static std::string ModelMatrixName;
    static std::string ModelMatrixInverseName;
    static std::string ViewMatrixName; 
    static std::string ProjectionMatrixName; 

    static std::string PositionAttributeName; 
    static std::string NormalAttributeName; 
    static std::string TexCoordAttributeName;

    CVaoMeshManager* GetMeshManager();
    COglShaderManager* GetShaderManager();
private:
    struct GLFWwindow* Window;

    int FrameWidth = -1;
    int FrameHeight = -1;
    float FrameAspectRatio = 1.f;

    CVaoMeshManager* MeshManager = NULL;
    COglShaderManager* ShaderManager = NULL;


        
};
