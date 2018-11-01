#include "Systems/RenderingSystem.h"

#include "Modules/Statics/Graphics.h"

#include "Modules/Statics/IAssetManager.h"
#include "Modules/Statics/IComponentManager.h"
#include "Modules/Statics/IEntityManager.h"

#include "Engine/AssetTypes/Settings/RenderSettings.h"

#include "Modules/Graphics/GraphicsUtils.h"
#include "Modules/Graphics/ICommandBuffer.h"
#include "Modules/Statics/IActiveCamera.h"

#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/LightComponent.h"
#include "Engine/Components/RendererComponent.h"
#include "Engine/Components/SkyLightComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Utility/Typedefs.h"

REGISTER_SERIALIZED_CLASS(RenderingSystem)

typedef std::unordered_map<unsigned int, IComponent *> IComponentMapType;

bool RenderingSystem::Initialize() {
  Active = true;
  return Active;
}

bool RenderingSystem::Update() {
  IGraphics *graphics = Statics::Get<IGraphics>();
  ActiveCommandBuffer =
      graphics->GetCommandBuffer(IGraphics::CommandBufferType::Main);
  ICommandBuffer *initBuffer =
      graphics->GetCommandBuffer(IGraphics::CommandBufferType::FrameInit);

  initBuffer->EnableDepth();
  initBuffer->EnableCullFace();
  initBuffer->Clear();

  // Update projection matrix, then draw meshes
  CameraComponent *camera = Statics::Get<IActiveCamera>()->GetCameraComponent();
  camera->ProjectionMatrix = glm::perspective(
      camera->FOV,
      Statics::Get<IGraphics>()->GetContext()->GetFrameAspectRatio(),
      camera->NearPlane, camera->FarPlane);

  // TODO Draw Skybox
  FindLights();

  DrawSkyBox();
  DrawOpaqueMeshes();

  return true;
}

void RenderingSystem::DrawSkyBox() {
  IComponentManager *componentManager = Statics::Get<IComponentManager>();
  SkyLightComponent *skyLight =
      componentManager->GetComponentOfType<SkyLightComponent>();

  // draw sky light here
}

void RenderingSystem::FindLights() {
  for (unsigned char x = 0; x < MAX_LIGHTS; x++)
    LightComponents[x] = nullptr;

  LightsFound = 0;
  CachedDirectionalLight = nullptr;

  IComponentManager *componentManager = Statics::Get<IComponentManager>();

  ComponentMap<LightComponent> *lightComponentMap =
      componentManager->GetComponentMap<LightComponent>();

  for (unsigned int x = 0; x < lightComponentMap->Count(); x++) {
    LightComponent *light = lightComponentMap->AtIndex(x);
    if (light->LightType != DIRECTIONAL_LIGHT_TYPE && LightsFound != MAX_LIGHTS)
      LightComponents[LightsFound++] = light;
    else if (light->LightType == DIRECTIONAL_LIGHT_TYPE &&
             !CachedDirectionalLight)
      CachedDirectionalLight = light;
  }
}

void RenderingSystem::SetLightParameters(unsigned int shaderId) {
  // set camera position

  // set for directional light
  glm::vec4 directionalColor = glm::vec4(1, 1, 1, 0);
  glm::vec4 directionalDirection = glm::vec4(0, -1, 0, 0);

  IComponentManager *componentManager = Statics::Get<IComponentManager>();

  if (CachedDirectionalLight) {
    TransformComponent *xform =
        componentManager->GetComponentOfType<TransformComponent>(
            CachedDirectionalLight->EntityId());
    directionalColor.x = CachedDirectionalLight->Color[0];
    directionalColor.y = CachedDirectionalLight->Color[1];
    directionalColor.z = CachedDirectionalLight->Color[2];
    directionalColor.w = CachedDirectionalLight->Intensity;

    glm::quat q = xform->GetRotation();
    glm::vec3 dir = glm::vec3(0, 0, 1) * q;
    directionalDirection =
        glm::vec4(dir.x, dir.y, dir.x, CachedDirectionalLight->ShadowEnabled);
  }

  ActiveCommandBuffer->SetVector("DirectionalLight.Color", shaderId,
                                 directionalColor);
  ActiveCommandBuffer->SetVector("DirectionalLight.Direction", shaderId,
                                 directionalDirection);
  //
  for (unsigned char x = 0; x < MAX_LIGHTS; x++) {
    char lightId[64];
    glm::vec4 lightColor = glm::vec4(0, 0, 0, 0);
    sprintf(lightId, "Lights[%i]", x);
    if (x >= LightsFound) {
      std::string col = std::string(lightId) + ".Color";
      ActiveCommandBuffer->SetVector(col, shaderId, lightColor);
      continue;
    }

    LightComponent *light = LightComponents[x];
    TransformComponent *xform =
        componentManager->GetComponentOfType<TransformComponent>(
            light->EntityId());

    glm::vec3 pos = xform->GetPosition();
    glm::vec3 dir(0, 1, 0);

    if (light->LightType == SPOT_LIGHT_TYPE)
      dir = glm::vec3(0, 0, 1) * xform->GetRotation();

    glm::vec4 pos4 = glm::vec4(pos.x, pos.y, pos.z, 1.0);
    glm::vec4 dir4 = glm::vec4(dir.x, dir.y, dir.z, 1.0);

    lightColor = glm::vec4(light->Color[0], light->Color[1], lightColor[2],
                           light->Intensity);

    glm::vec4 params0(light->LightType, light->InnerAngle, light->OuterAngle,
                      light->ShadowEnabled);

    glm::vec4 params1(light->Constant, light->Linear, light->Quadratic,
                      light->CutOff);

    ActiveCommandBuffer->SetVector(std::string(lightId) + ".Position", shaderId,
                                   pos4);
    ActiveCommandBuffer->SetVector(std::string(lightId) + ".Direction",
                                   shaderId, dir4);
    ActiveCommandBuffer->SetVector(std::string(lightId) + ".Color", shaderId,
                                   lightColor);
    ActiveCommandBuffer->SetVector(std::string(lightId) + ".Params0", shaderId,
                                   params0);
    ActiveCommandBuffer->SetVector(std::string(lightId) + ".Params1", shaderId,
                                   params1);
  }
}

void RenderingSystem::DrawOpaqueMeshes() {
  IComponentManager *componentManager = Statics::Get<IComponentManager>();
  // iterate over renderer iterator

  ComponentMap<TransformComponent> *transformComponents =
      componentManager->GetComponentMap<TransformComponent>();
  ComponentMap<RendererComponent> *rendererComponents =
      componentManager->GetComponentMap<RendererComponent>();

  bool drawMeshes = true;
  drawMeshes = drawMeshes && transformComponents;
  drawMeshes = drawMeshes && rendererComponents;

  if (!drawMeshes)
    return;

  for (unsigned int x = 0; x < rendererComponents->Count(); x++) {
    RendererComponent *renderer = rendererComponents->AtIndex(x);
    unsigned int entityId = renderer->EntityId();
    TransformComponent *transform = transformComponents->At(entityId);

    if (!transform || !renderer)
      continue;

    // TODO cache material if it repeats
    // discard object if it's not in the view frustrum

    unsigned int shaderId;
    GraphicsUtils::SetUniformsFromMaterial(
        ActiveCommandBuffer, renderer->MaterialReference, shaderId);
    // TODO track which shader had lighting uniforms already set
    SetLightParameters(shaderId);
    ActiveCommandBuffer->DrawMesh(transform->WorldTransform,
                                  transform->WorldTransformInv,
                                  renderer->MeshReference, shaderId);
  }
}

RenderingSystem::~RenderingSystem() {}
