#include "Modules/Graphics/OpenGL/COpenGLRender.h"
#include "Modules/Graphics/OpenGL/COglShaderManager.h"
#include "Modules/Graphics/OpenGL/CVaoMeshManager.h"
#include "Core.h"
#include "Utility/Graphics.h"

#include "Modules/Statics/CAssetManager.h"
#include "Engine/AssetTypes/CMaterial.h"
#include "Modules/Statics/CActiveCamera.h"
#include "Modules/Graphics/CGraphics.h"

#include <stdlib.h>
#include <stdio.h>

std::string COpenGLRender::PositionAttributeName = "_PositionAttribute"; 
std::string COpenGLRender::NormalAttributeName = "_NormalAttribute"; 
std::string COpenGLRender::TexCoordAttributeName = "_TexCoordAttribute"; 

std::string COpenGLRender::ModelMatrixName = "_ModelMatrix";
std::string COpenGLRender::ModelMatrixInverseName = "_ModelMatrixInverseTransposed";
std::string COpenGLRender::ViewMatrixName = "_ViewMatrix"; 
std::string COpenGLRender::ProjectionMatrixName = "_ProjectionMatrix"; 

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

COpenGLRender::COpenGLRender()
{
    MeshManager = new CVaoMeshManager();
    ShaderManager = new COglShaderManager();
    CAssetManager::Get()->GetAssetIteratorOfType("Material", MaterialIterator);
}

COpenGLRender::~COpenGLRender()
{
    delete MeshManager;
    delete ShaderManager;

    glfwDestroyWindow(Window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

bool COpenGLRender::WindowShouldClose()
{
    return glfwWindowShouldClose(Window);
}

void COpenGLRender::DrawMesh(glm::mat4 &matrix, unsigned int &meshAssetId, unsigned int &materialId)
{
    glm::mat4 matrixInv = glm::inverse(glm::transpose(matrix));
    DrawMesh(matrix, matrixInv, meshAssetId, materialId);
}

void COpenGLRender::DrawMesh(glm::mat4 &matrix, glm::mat4 &matrixInv, unsigned int &meshAssetId, unsigned int &materialId)
{
    CMaterial* material;
    if (materialId == 0)
        materialId = MaterialIterator->second.begin()->first;

    material = dynamic_cast<CMaterial*>(MaterialIterator->second.at(materialId));

    // set material uniforms
    int programId = ShaderManager->GetShaderProgramId(material->ShaderId);
    // define static default uniform names +
    glUseProgram(programId);
    // TODO cache camera matrix 
    ShaderManager->SetMatrix(ModelMatrixName, programId, matrix);
    ShaderManager->SetMatrix(ModelMatrixInverseName, programId, matrixInv);
    ShaderManager->SetMatrix(ProjectionMatrixName, programId, CActiveCamera::ProjectionMatrix());
    ShaderManager->SetMatrix(ViewMatrixName, programId, CActiveCamera::ViewMatrix());
    // set material uniforms
    ShaderManager->SetMaterialUniforms(material, programId);
    // handle mesh vao management
    unsigned int vaoId, indexCount;
    MeshManager->GetVAOForMeshId(programId, meshAssetId, vaoId, indexCount);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glEnable(GL_CULL_FACE);
    // bind/unbind vao
    glBindVertexArray(vaoId); 
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); 
}

void COpenGLRender::Create(unsigned short width, unsigned short height, const CString& title)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
	{
        exit(EXIT_FAILURE);
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    Window = glfwCreateWindow(width, height, title.GetCharArray(), NULL, NULL);
    if (!Window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(Window, key_callback);
    glfwMakeContextCurrent(Window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
}

void COpenGLRender::UpdateFrame()
{
    glfwSwapBuffers(Window);		// Shows what we drew
    glfwPollEvents();
}


void COpenGLRender::GetWindowFramebufferSize(int &width, int &height)
{
    glfwGetFramebufferSize(Window, &width, &height);
}

bool COpenGLRender::IsWindowCreated()
{
    return Window != NULL;    
}

void COpenGLRender::BeginDrawingLoop()
{
    glEnable(GL_DEPTH);	
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void COpenGLRender::EndDrawingLoop()
{
    glfwSwapBuffers(Window);
    glfwPollEvents();
}
