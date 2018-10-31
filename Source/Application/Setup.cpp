#include "Application/Setup.h"

#include "Modules/Statics/ActiveCamera.h"
#include "Modules/Statics/AssetManager.h"
#include "Modules/Statics/ComponentManager.h"
#include "Modules/Statics/EntityManager.h"
#include "Modules/Statics/Graphics.h"
#include "Modules/Statics/Input.h"
#include "Modules/Statics/SceneManager.h"

#include "Modules/Graphics/IShader.h"

#include "Engine/Components/TransformComponent.h"

#include "Modules/Graphics/GraphicsUtils.h"

void SetStaticObjects() {
  Statics::AddStaticObject<IEntityManager, EntityManager>();
  Statics::AddStaticObject<IAssetManager, AssetManager>();
  Statics::AddStaticObject<IComponentManager, ComponentManager>();
  Statics::AddStaticObject<IInput, Input>();
  Statics::AddStaticObject<ISceneManager, SceneManager>();
  Statics::AddStaticObject<IActiveCamera, ActiveCamera>();
  Statics::AddStaticObject<IGraphics, Graphics>();
}

void SetupDefaults() {
  // adds a default material
  Statics::Get<IAssetManager>()->AddAssetOfType("Material");
  IShader *defaultShader = GraphicsUtils::CreateVertexFragmentShader(
      "Assets/Shaders/default.vert", "Assets/Shaders/default.frag");
  Statics::Get<IGraphics>()->SetDefaultShader(defaultShader);
}

void AddDefaultCamera() {
  // set camera
  TransformComponent *transformComponent =
      Statics::Get<IActiveCamera>()->GetTransformComponent();
  transformComponent->SetPosition(0, 1, -6.f);
  // add render settings
  Statics::Get<IAssetManager>()->AddAssetOfType("RenderSettings");
}

void InitializeEngine() {
  SetStaticObjects();
  SetupDefaults();
  AddDefaultCamera();
}