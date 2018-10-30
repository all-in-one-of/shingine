#include "Application/Setup.h"

#include "Modules/Statics/AssetManager.h"
#include "Modules/Statics/EntityManager.h"
#include "Modules/Statics/ComponentManager.h"
#include "Modules/Statics/Input.h"
#include "Modules/Statics/SceneManager.h"
#include "Modules/Statics/ActiveCamera.h"
#include "Modules/Statics/Graphics.h"

void SetStaticObjects()
{
   Statics::AddStaticObject<IEntityManager, EntityManager>();
   Statics::AddStaticObject<IAssetManager, AssetManager>();
   Statics::AddStaticObject<IComponentManager, ComponentManager>();
   Statics::AddStaticObject<IInput, Input>();
   Statics::AddStaticObject<ISceneManager, SceneManager>();
   Statics::AddStaticObject<IActiveCamera, ActiveCamera>();
   Statics::AddStaticObject<IGraphics, Graphics>();
}
