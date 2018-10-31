#pragma once
#include "Core.h"
class IRenderContext {
public:
  virtual ~IRenderContext(){};
  virtual void Create(unsigned short width, unsigned short height,
                      const String &title) = 0;

  virtual bool WindowShouldClose() = 0;

  virtual void GetWindowFramebufferSize(int &width, int &height) = 0;
  virtual float GetFrameAspectRatio() = 0;
  virtual void SetFramebufferSize(int &width, int &height) = 0;
  virtual bool IsWindowCreated() = 0;

  virtual void Update() = 0;
};
