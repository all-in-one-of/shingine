#include "Game/LightViewerSystem.h"
#include "Modules/Statics/IAssetManager.h"
#include "Modules/Statics/IComponentManager.h"
#include "Modules/Statics/IGraphics.h"

#include "Engine/Components/LightComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Modules/Utility/GraphicsUtils.h"
#include "Modules/Graphics/ICommandBuffer.h"
#include "Modules/Graphics/IShader.h"
#include "Utility/Typedefs.h"
#include <iostream>

REGISTER_SERIALIZED_CLASS(LightViewerSystem)

bool LightViewerSystem::Initialize() {
  const String cubeObjectPath = "Assets/Scenes/SceneAssets/Meshes/box1.ssd";
  const String vertexShaderPath = "Assets/Shaders/Unlit.vert";
  const String fragmentShaderPath = "Assets/Shaders/Unlit.frag";

  LightComponentMap =
      Statics::Get<IComponentManager>()->GetComponentMap<LightComponent>();
  // load cube mesh
  IObject *cube =
      Statics::Get<IAssetManager>()->GetAssetByFileName(cubeObjectPath);
  CubeMeshAssetId = cube->UniqueID();
  // add shader
  IShader *unlitShader = GraphicsUtils::CreateVertexFragmentShader(
      vertexShaderPath, fragmentShaderPath);
  UnlitShaderId = unlitShader->AssetId();

  return true;
}

const float cubeScale = .15f;
const std::string UnlitShaderName = "_UnlitColor";

bool LightViewerSystem::Update() {
  IComponentManager *componentManager = Statics::Get<IComponentManager>();
  ICommandBuffer *commandBuffer = Statics::Get<IGraphics>()->GetCommandBuffer(
      IGraphics::CommandBufferType::Main);

  glm::mat4 ident(1);
  glm::mat4 scaleMatrix =
      glm::scale(ident, glm::vec3(cubeScale, cubeScale, cubeScale));

  for (unsigned int x = 0; x < LightComponentMap->Count(); x++) {
    LightComponent *light = LightComponentMap->AtIndex(x);
    if (light->LightType == DIRECTIONAL_LIGHT_TYPE)
      continue;
    commandBuffer->SetVector(UnlitShaderName, UnlitShaderId, light->GetColor());
    TransformComponent *transform =
        componentManager->GetComponentOfType<TransformComponent>(
            light->EntityId());
    glm::mat4 xform =
        glm::translate(ident, transform->WorldPosition) * scaleMatrix;
    commandBuffer->DrawMesh(xform, ident, CubeMeshAssetId, UnlitShaderId);
  }
  return true;
}
