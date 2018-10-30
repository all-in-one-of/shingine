#include "Modules/Graphics/OpenGL/OpenGLRender.h"
#include "Modules/Graphics/OpenGL/OglShaderManager.h"
#include "Modules/Graphics/OpenGL/OglTextureManager.h"
#include "Modules/Graphics/OpenGL/VaoMeshManager.h"
#include "Core.h"
#include "Utility/Graphics.h"

#include "Modules/Statics/AssetManager.h"
#include "Engine/AssetTypes/Material.h"
#include "Modules/Statics/IActiveCamera.h"
#include "Modules/Statics/IGraphics.h"

#include "Modules/Statics/IInput.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>

std::string OpenGLRender::PositionAttributeName = "_PositionAttribute";
std::string OpenGLRender::NormalAttributeName = "_NormalAttribute";
std::string OpenGLRender::TexCoordAttributeName = "_TexCoordAttribute";

std::string OpenGLRender::ModelMatrixName = "_ModelMatrix";
std::string OpenGLRender::ModelMatrixInverseName = "_ModelMatrixInverseTransposed";
std::string OpenGLRender::ViewMatrixName = "_ViewMatrix";
std::string OpenGLRender::ProjectionMatrixName = "_ProjectionMatrix";

namespace OpenGLRenderStatics
{
static void ErrorCallback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void KeyCallback(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    Statics::Get<IInput>()->SetKeyEvent(key, scanCode, action, mods);
}

void MouseCallback(GLFWwindow *window, int key, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    Statics::Get<IInput>()->SetMouseEvent(key, action, mods);
}

void MouseMove(GLFWwindow *window, double mouseX, double mouseY)
{
}

void ResizeWindow(GLFWwindow *window, GLsizei width, GLsizei height)
{
    Statics::Get<IInput>()->SetScreenReferenceSize(width, height);
    Statics::Get<IGraphics>()->GetContext()->SetFramebufferSize(width, height);
}
}; 

OpenGLRender::OpenGLRender()
{
    ShaderManager = new OglShaderManager();
    MeshManager = new VaoMeshManager();
    TextureManager = new OglTextureManager();
}

OpenGLRender::~OpenGLRender()
{
    delete ShaderManager;
    delete MeshManager;
    delete TextureManager;

    glfwDestroyWindow(Window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

bool OpenGLRender::WindowShouldClose()
{
    return glfwWindowShouldClose(Window);
}

void OpenGLRender::Create(unsigned short width, unsigned short height, const String &title)
{
    glfwSetErrorCallback(OpenGLRenderStatics::ErrorCallback);

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

    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // set screen width explicitly
    OpenGLRenderStatics::ResizeWindow(Window, FrameWidth, FrameHeight);
    // UI callbacks
    glfwSetWindowSizeCallback(Window, OpenGLRenderStatics::ResizeWindow);
    glfwSetKeyCallback(Window, OpenGLRenderStatics::KeyCallback);
    glfwSetCursorPosCallback(Window, OpenGLRenderStatics::MouseMove);
    glfwSetMouseButtonCallback(Window, OpenGLRenderStatics::MouseCallback);

    glfwMakeContextCurrent(Window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
}

void OpenGLRender::GetWindowFramebufferSize(int &width, int &height)
{
    // glfwGetFramebufferSize(Window, &width, &height);
    width = FrameWidth;
    height = FrameHeight;
}

float OpenGLRender::GetFrameAspectRatio()
{
    return FrameAspectRatio;
}

void OpenGLRender::SetFramebufferSize(int &width, int &height)
{
    FrameWidth = width;
    FrameHeight = height;
    FrameAspectRatio = width / (float)height;
}

VaoMeshManager* OpenGLRender::GetMeshManager()
{
    return MeshManager;
}

OglShaderManager* OpenGLRender::GetShaderManager()
{
    return ShaderManager;
}

OglTextureManager* OpenGLRender::GetTextureManager()
{
    return TextureManager;
}

bool OpenGLRender::IsWindowCreated()
{
    return Window != nullptr;
}

void OpenGLRender::Update()
{

    Statics::Get<IInput>()->Update();
    glfwSwapBuffers(Window);
    glfwPollEvents();
    
    double x, y;
    glfwGetCursorPos(Window, &x, &y);
    Statics::Get<IInput>()->SetMousePosition(x, y);
}
