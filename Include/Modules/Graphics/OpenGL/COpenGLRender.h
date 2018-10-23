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
    virtual void DrawMesh(glm::mat4 &matrix, unsigned int &meshAssetId, unsigned int &materialId);
    virtual void DrawMesh(glm::mat4 &matrix, glm::mat4 &matrixInv, unsigned int &meshAssetId, unsigned int &materialId);

    virtual void UpdateFrame();
    virtual bool WindowShouldClose();
    virtual void GetWindowFramebufferSize(int &width, int &height);
    virtual bool IsWindowCreated();
    virtual void BeginDrawingLoop();
    virtual void EndDrawingLoop();

    // pre-defined uniform names 
    static std::string ModelMatrixName;
    static std::string ModelMatrixInverseName;
    static std::string ViewMatrixName; 
    static std::string ProjectionMatrixName; 

    static std::string PositionAttributeName; 
    static std::string NormalAttributeName; 
    static std::string TexCoordAttributeName;
private:
    struct GLFWwindow* Window;
    CVaoMeshManager* MeshManager;
    COglShaderManager* ShaderManager;
    std::unordered_map<std::string, 
        std::unordered_map<unsigned int, ISerializedClass*>>::iterator MaterialIterator;
};
