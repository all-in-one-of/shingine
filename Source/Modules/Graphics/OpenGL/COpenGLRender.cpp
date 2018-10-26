#include "Modules/Graphics/OpenGL/COpenGLRender.h"
#include "Modules/Graphics/OpenGL/COglShaderManager.h"
#include "Modules/Graphics/OpenGL/CVaoMeshManager.h"
#include "Core.h"
#include "Utility/Graphics.h"

#include "Modules/Statics/CAssetManager.h"
#include "Engine/AssetTypes/CMaterial.h"
#include "Modules/Statics/CActiveCamera.h"
#include "Modules/Graphics/CGraphics.h"

#include "Modules/Statics/CInput.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>

std::string COpenGLRender::PositionAttributeName = "_PositionAttribute"; 
std::string COpenGLRender::NormalAttributeName = "_NormalAttribute"; 
std::string COpenGLRender::TexCoordAttributeName = "_TexCoordAttribute"; 

std::string COpenGLRender::ModelMatrixName = "_ModelMatrix";
std::string COpenGLRender::ModelMatrixInverseName = "_ModelMatrixInverseTransposed";
std::string COpenGLRender::ViewMatrixName = "_ViewMatrix"; 
std::string COpenGLRender::ProjectionMatrixName = "_ProjectionMatrix"; 

namespace COpenGLRenderStatics
{
    static void ErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

    void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        CInput::Get()->SetKeyEvent(key, scanCode, action, mods);
    }

    void MouseCallback(GLFWwindow* window, int key, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        CInput::Get()->SetMouseEvent(key, action, mods);
    }

    void MouseMove(GLFWwindow *window, double mouseX, double mouseY)
    {
        CInput::Get()->SetMousePosition(mouseX, mouseY);
    }

    void ResizeWindow(GLFWwindow *window, GLsizei width, GLsizei height)
    {
        CGraphics::GetContext()->SetFramebufferSize(width, height);
    }
};

COpenGLRender::COpenGLRender()
{
    ShaderManager = new COglShaderManager();
    MeshManager = new CVaoMeshManager();
}

COpenGLRender::~COpenGLRender()
{
    delete ShaderManager;
    delete MeshManager;
    
    glfwDestroyWindow(Window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

bool COpenGLRender::WindowShouldClose()
{
    return glfwWindowShouldClose(Window);
}

void COpenGLRender::Create(unsigned short width, unsigned short height, const CString& title)
{
    glfwSetErrorCallback(COpenGLRenderStatics::ErrorCallback);

    if (!glfwInit()) 
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    FrameWidth = width;
    FrameHeight = height;

    Window = glfwCreateWindow(FrameWidth, FrameHeight, title.GetCharArray(), NULL, NULL);
    if (!Window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    // UI callbacks
    glfwSetWindowSizeCallback(Window, COpenGLRenderStatics::ResizeWindow);
    glfwSetKeyCallback(Window, COpenGLRenderStatics::KeyCallback);
    glfwSetCursorPosCallback(Window, COpenGLRenderStatics::MouseMove); 
    glfwSetMouseButtonCallback(Window, COpenGLRenderStatics::MouseCallback);

    glfwMakeContextCurrent(Window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
    
    // glViewport(0, 0, width, height);
}

void COpenGLRender::GetWindowFramebufferSize(int &width, int &height)
{
    // glfwGetFramebufferSize(Window, &width, &height);
    width = FrameWidth;
    height = FrameHeight;
}

float COpenGLRender::GetFrameAspectRatio()
{
    return FrameAspectRatio;
}

void COpenGLRender::SetFramebufferSize(int &width, int &height)
{
    FrameWidth = width;
    FrameHeight = height;
    FrameAspectRatio = width / (float) height;
}

CVaoMeshManager* COpenGLRender::GetMeshManager()
{
    return MeshManager;
}

COglShaderManager* COpenGLRender::GetShaderManager()
{
    return ShaderManager;
}

bool COpenGLRender::IsWindowCreated()
{
    return Window != NULL;    
}

void COpenGLRender::Update()
{
    CInput::Get()->Update();
    glfwSwapBuffers(Window);
    glfwPollEvents();
}
