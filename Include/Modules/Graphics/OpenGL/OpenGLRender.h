#pragma once
#include "Modules/Graphics/IRenderContext.h"
#include <string>
#include <unordered_map>

class VaoMeshManager;
class OglShaderManager;
class OglTextureManager;
class IObject;

class OpenGLRender : public IRenderContext {
public:
  OpenGLRender();
  virtual ~OpenGLRender();
  virtual void Create(unsigned short width, unsigned short height,
                      const String &title);

  virtual bool WindowShouldClose();
  virtual void GetWindowFramebufferSize(int &width, int &height);
  virtual float GetFrameAspectRatio();
  virtual void SetFramebufferSize(int &width, int &height);
  virtual bool IsWindowCreated();
  virtual void Update();
  virtual void Cleanup();

  VaoMeshManager *GetMeshManager();
  OglShaderManager *GetShaderManager();
  OglTextureManager *GetTextureManager();

private:
  struct GLFWwindow *Window;

  int FrameWidth = -1;
  int FrameHeight = -1;
  float FrameAspectRatio = 1.f;

  VaoMeshManager *MeshManager = nullptr;
  OglShaderManager *ShaderManager = nullptr;
  OglTextureManager *TextureManager = nullptr;

};
