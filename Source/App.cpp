#include <iostream>
#include "Core.h"

#include "Modules/Graphics/IShader.h"
#include "Modules/Statics/Graphics.h"
#include "Solver/Solver.h"

#include "Modules/Statics/ActiveCamera.h"

#include "Engine/Components/TransformComponent.h"

#include "Game/FirstPersonController/FirstPersonComponent.h"


#include "Modules/Statics/Statics.h"
#include "Modules/Statics/AssetManager.h"
#include "Modules/Statics/ComponentManager.h"
#include "Modules/Statics/EntityManager.h"
#include "Modules/Statics/Input.h"
#include "Modules/Statics/SceneManager.h"
#include "Modules/Statics/ActiveCamera.h"
#include "Modules/Statics/Graphics.h"

void Initialize()
{
    // Add global objects
    Statics::AddStaticObject<IEntityManager, EntityManager>();
    Statics::AddStaticObject<IAssetManager, AssetManager>();
    Statics::AddStaticObject<IComponentManager, ComponentManager>();
    Statics::AddStaticObject<IInput, Input>();
    Statics::AddStaticObject<ISceneManager, SceneManager>();
    Statics::AddStaticObject<IActiveCamera, ActiveCamera>();
    Statics::AddStaticObject<IGraphics, Graphics>();
    
    Statics::Get<IAssetManager>()->AddAssetOfType("Material");
    // set default shader
    IShader* defaultShader = dynamic_cast<IShader*>(
        Statics::Get<IAssetManager>()->AddAssetOfType("Shader"));

    String vertSrc, fragSrc;
    ResourceLoader::LoadText("Assets/Shaders/default.vert", vertSrc);
    ResourceLoader::LoadText("Assets/Shaders/default.frag", fragSrc);
    defaultShader->AddSource(EShaderType::VERTEX, vertSrc);
    defaultShader->AddSource(EShaderType::FRAGMENT, fragSrc);
    Statics::Get<IGraphics>()->SetDefaultShader(defaultShader);

    // set camera
    TransformComponent* transformComponent = Statics::Get<IActiveCamera>()->GetTransformComponent();
    transformComponent->SetPosition(0, 1, -6.f);
    // add render settings
    Statics::Get<IAssetManager>()->AddAssetOfType("RenderSettings");

    // add first person component
    IComponent *a = Statics::Get<IComponentManager>()->AddComponent<FirstPersonController::FirstPersonComponent>(
          transformComponent->EntityId());
    FirstPersonController::FirstPersonComponent *comp = dynamic_cast<FirstPersonController::FirstPersonComponent*>(a);
    comp->PlayerMovementSettings->RunMultiplier = 5.f;
    comp->PlayerMovementSettings->ForwardSpeed = 13.f;
    transformComponent->IsDynamic = 1;

    Statics::Get<IGraphics>()->SetupWindow();
}

int main()
{
    Initialize();

    bool didLoad = ResourceLoader::Load("Assets/Scenes/Test.ssd");


    if (!didLoad)
    {
        std::cout << "Couldn't load the scene" << std::endl;
        return 1;
    }

    Solver* solver = new Solver();
    solver->AddSystem("TransformSystem");
    solver->AddSystem("RenderingSystem");
    solver->AddSystem("FirstPersonSystem");
    solver->InitializeSystems();

    while (solver->Simulate())
    {
         if (!Statics::Get<IGraphics>()->Render())
             break;
    }
    return 0;
}
