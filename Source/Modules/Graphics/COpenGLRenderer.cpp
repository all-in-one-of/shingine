#include "Modules/Graphics/COpenGLRenderer.h"
#include "Utility/Common.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


void key_callback( GLFWwindow* window, 
						  int key, 
						  int scancode, 
						  int action, 
						  int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

COpenGLRender::COpenGLRender()
{
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

COpenGLRender::~COpenGLRender()
{
    glfwDestroyWindow(Window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
