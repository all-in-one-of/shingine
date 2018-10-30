#pragma once
#include "Utility/Typedefs.h"
#include <string>
class ICommandBuffer
{
public:
    enum EDrawPolygonMode { Fill, Point, Line };

    virtual ~ICommandBuffer() {};
    virtual void ResetCommandBuffer() = 0;

    virtual void EnableDepth() = 0;
    virtual void EnableCullFace() = 0;
    virtual void Clear() = 0;

    virtual void SetPolygonMode(EDrawPolygonMode mode) = 0;
    
    virtual void DrawMesh(glm::mat4 &matrix, unsigned int &meshAssetId, unsigned int &shaderId) = 0;
    virtual void DrawMesh(glm::mat4 &matrix, glm::mat4 &matrixInv, unsigned int &meshAssetId, unsigned int &shaderId) = 0;

    virtual void Execute() = 0;

    virtual void SetMatrix(const std::string &name, unsigned int shaderId, glm::mat4 matrix) = 0;
    virtual void SetFloat(const std::string &name, unsigned int shaderId, float value) = 0;
    virtual void SetVector(const std::string &name, unsigned int shaderId, glm::vec4 &vector) = 0;
    virtual void SetTexture(const std::string &name, unsigned int shaderId, unsigned int textureId) = 0;

};
