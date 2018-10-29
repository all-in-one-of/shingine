#include <iostream>
#include "Core.h"
#include "Modules/Statics/AssetManager.h"
#include "Modules/Statics/ComponentManager.h"
#include "Modules/Statics/EntityManager.h"

#include "Modules/Graphics/IShader.h"
#include "Modules/Graphics/Graphics.h"
#include "Solver/Solver.h"

#include "Modules/Statics/ActiveCamera.h"

#include "Engine/Components/TransformComponent.h"

#include "Game/FirstPersonController/FirstPersonComponent.h"

void Initialize()
{
    AssetManager::Get()->AddAssetOfType("Material");
    // set default shader
    IShader* defaultShader = dynamic_cast<IShader*>(
        AssetManager::Get()->AddAssetOfType("Shader"));
    String vertSrc, fragSrc;
    ResourceLoader::LoadText("Assets/Shaders/default.vert", vertSrc);
    ResourceLoader::LoadText("Assets/Shaders/default.frag", fragSrc);
    defaultShader->AddSource(EShaderType::VERTEX, vertSrc);
    defaultShader->AddSource(EShaderType::FRAGMENT, fragSrc);
    Graphics::SetDefaultShader(defaultShader);

    // set camera
    TransformComponent* transformComponent = ActiveCamera::Get()->GetTransformComponent();
    transformComponent->SetPosition(0, 1, -6.f);
    // add render settings
    AssetManager::Get()->AddAssetOfType("RenderSettings");

    // add first person component
    IComponent *a = ComponentManager::Get()->AddComponent<FirstPersonController::FirstPersonComponent>(
          transformComponent->EntityId());
    FirstPersonController::FirstPersonComponent *comp = dynamic_cast<FirstPersonController::FirstPersonComponent*>(a);
    comp->PlayerMovementSettings->RunMultiplier = 5.f;
    comp->PlayerMovementSettings->ForwardSpeed = 13.f;
    transformComponent->IsDynamic = 1;
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
         if (!Graphics::Render())
             break;
    }
    return 0;
}
