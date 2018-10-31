#pragma once
#include "Core.h"

enum EShaderType {
  UNKNOWN = 0,
  VERTEX = 1,
  FRAGMENT = 2,
  GEOMETRY = 3,
  COMPUTE = 4
};

class IShader {
public:
  virtual ~IShader(){};
  virtual unsigned int AssetId() = 0;
  virtual void AddSource(EShaderType type, const String &source) = 0;
  virtual bool GetSource(EShaderType type, String &source) = 0;
};
