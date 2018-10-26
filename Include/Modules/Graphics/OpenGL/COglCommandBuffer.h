#pragma once
#include "Modules/Graphics/ICommandBuffer.h"
#include <glm/glm.hpp>
#include <vector>

class COglCommandBuffer : public ICommandBuffer
{
public:
    COglCommandBuffer();
    virtual ~COglCommandBuffer();
    virtual void ResetCommandBuffer();
    virtual void EnableDepth();
    virtual void EnableCullFace();
    virtual void Clear();

    virtual void SetPolygonMode(ICommandBuffer::EDrawPolygonMode mode);

    virtual void DrawMesh(glm::mat4 &matrix, unsigned int &meshAssetId, unsigned int &materialId);
    virtual void DrawMesh(glm::mat4 &matrix, glm::mat4 &matrixInv, unsigned int &meshAssetId, unsigned int &materialId);

    virtual void Execute();

    virtual void SetMatrix(const std::string &name, int programId, glm::mat4 matrix);
    virtual void SetFloat(const std::string &name, int programId, float value);
    virtual void SetVector(const std::string &name, int programId, glm::vec4 &vector);

    virtual void SetMatrix(int uniformLocation, glm::mat4 &matrix);
    virtual void SetFloat(int uniformLocation, float value);
    virtual void SetVector(int uniformLocation, glm::vec4 &vector);

    void UseProgram(int programId);
private:
    void ReadValue(unsigned char &cmd);
    void AddCommand(const unsigned char &cmd);
    std::vector<unsigned char> Commands;
    unsigned int CommandCount = 0;
    unsigned int CurrentByte = 0;
};
