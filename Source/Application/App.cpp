#include <iostream>
#include "Core.h"
#include "Application/Setup.h"

#include "Modules/Graphics/IShader.h"
#include "Solver/Solver.h"

#include "Modules/Statics/ActiveCamera.h"

#include "Engine/Components/TransformComponent.h"

#include "Game/FirstPersonController/FirstPersonComponent.h"

#include "Modules/Statics/IGraphics.h"
#include "Modules/Statics/IAssetManager.h"
#include "Modules/Statics/IComponentManager.h"


void Initialize()
{
    SetStaticObjects();

    Statics::Get<IAssetManager>()->AddAssetOfType("Material");
    // set default shader
    IShader *defaultShader = dynamic_cast<IShader *>(
        Statics::Get<IAssetManager>()->AddAssetOfType("Shader"));

    String vertSrc, fragSrc;
    ResourceLoader::LoadText("Assets/Shaders/default.vert", vertSrc);
    ResourceLoader::LoadText("Assets/Shaders/default.frag", fragSrc);
    defaultShader->AddSource(EShaderType::VERTEX, vertSrc);
    defaultShader->AddSource(EShaderType::FRAGMENT, fragSrc);
    Statics::Get<IGraphics>()->SetDefaultShader(defaultShader);

    // set camera
    TransformComponent *transformComponent = Statics::Get<IActiveCamera>()->GetTransformComponent();
    transformComponent->SetPosition(0, 1, -6.f);
    // add render settings
    Statics::Get<IAssetManager>()->AddAssetOfType("RenderSettings");

    // add first person component
    IComponent *a = Statics::Get<IComponentManager>()->AddComponent<FirstPersonController::FirstPersonComponent>(
        transformComponent->EntityId());
    FirstPersonController::FirstPersonComponent *comp = dynamic_cast<FirstPersonController::FirstPersonComponent *>(a);
    comp->PlayerMovementSettings->RunMultiplier = 5.f;
    comp->PlayerMovementSettings->ForwardSpeed = 13.f;
    transformComponent->IsDynamic = 1;


    ISerializedClass *obj;
    ResourceLoader::LoadBitmap("Assets/Textures/uv_checker.bmp", obj);

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

    Solver *solver = new Solver();
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
