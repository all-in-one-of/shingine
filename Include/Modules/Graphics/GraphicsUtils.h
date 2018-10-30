#pragma once
#include "IShader.h"
class ICommandBuffer;
class String;

namespace GraphicsUtils
{
    void SetUniformsFromMaterial(ICommandBuffer* buffer, unsigned int materialId,  unsigned int &shaderId);
    IShader* CreateVertexFragmentShader(const String &vertexFileName, const String &fragmentFileName);
}
