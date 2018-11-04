#pragma once
#include "Modules/Graphics/ICommandBuffer.h"
#include <glm/glm.hpp>
#include <vector>
class OpenGLRender;
class OglCommandBuffer : public ICommandBuffer {
public:
  OglCommandBuffer();
  virtual ~OglCommandBuffer();
  virtual void ResetCommandBuffer();
  virtual void EnableDepth();
  virtual void EnableCullFace();
  virtual void Clear();

  virtual void SetPolygonMode(ICommandBuffer::EDrawPolygonMode mode);

  virtual void DrawMesh(glm::mat4 &matrix, unsigned int &meshAssetId,
                        unsigned int &shaderId);
  virtual void DrawMesh(glm::mat4 &matrix, glm::mat4 &matrixInv,
                        unsigned int &meshAssetId, unsigned int &shaderId);

  virtual void Execute();

  virtual void SetMatrix(const std::string &name, unsigned int shaderId,
                         glm::mat4 matrix);
  virtual void SetFloat(const std::string &name, unsigned int shaderId,
                        float value);
  virtual void SetVector(const std::string &name, unsigned int shaderId,
                         const glm::vec4 &vector);

  virtual void SetTexture(const std::string &name, unsigned int shaderId,
                          unsigned int textureId);

private:
  void ReadValue(unsigned char &cmd);
  void AddCommand(const unsigned char &cmd);
  std::vector<unsigned char> Commands;
  unsigned int CommandCount = 0;
  unsigned int CurrentByte = 0;

  virtual void SetMatrixOgl(const std::string &name, int programId,
                            glm::mat4 matrix);
  virtual void SetFloatOgl(const std::string &name, int programId, float value);
  virtual void SetVectorOgl(const std::string &name, int programId,
                            const glm::vec4 &vector);

  virtual void SetMatrixOgl(int uniformLocation, glm::mat4 &matrix);
  virtual void SetFloatOgl(int uniformLocation, float value);
  virtual void SetVectorOgl(int uniformLocation, const glm::vec4 &vector);

  void UseProgram(int programId);

  OpenGLRender *GetContext();

  class CameraComponent *ActiveCamera;
  class TransformComponent *ActiveCameraTransform;
  void UpdateCamera();

  OpenGLRender *CachedRenderContext = nullptr;
  int CurrentShaderProgram = 0;
};
