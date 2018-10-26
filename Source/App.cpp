#include <iostream>
#include "Core.h"
#include "Modules/Statics/CAssetManager.h"
#include "Modules/Statics/CComponentManager.h"
#include "Modules/Statics/CEntityManager.h"

#include "Modules/Graphics/IShader.h"
#include "Modules/Graphics/CGraphics.h"
#include "Solver/CSolver.h"

#include "Modules/Statics/CActiveCamera.h"

#include "Engine/Components/CTransformComponent.h"

#include "Game/FirstPersonController/CFirstPersonComponent.h"

void Initialize()
{
    // set default shader
    IShader* defaultShader = dynamic_cast<IShader*>(
        CAssetManager::Get()->AddAssetOfType("Shader"));
    CString vertSrc, fragSrc;
    CResourceLoader::LoadText("Assets/Shaders/default.vert", vertSrc);
    CResourceLoader::LoadText("Assets/Shaders/default.frag", fragSrc);
    defaultShader->AddSource(EShaderType::VERTEX, vertSrc);
    defaultShader->AddSource(EShaderType::FRAGMENT, fragSrc);
    CGraphics::SetDefaultShader(defaultShader);

    // set camera
    CTransformComponent* transformComponent = CActiveCamera::Get()->GetTransformComponent();
    transformComponent->SetPosition(0, 1, -6.f);
    // add render settings
    CAssetManager::Get()->AddAssetOfType("RenderSettings");

    // add first person component
    IComponent *a = CComponentManager::Get()->AddComponent("FirstPersonComponent",
        transformComponent->EntityId());
    FirstPersonController::CFirstPersonComponent *comp = dynamic_cast<FirstPersonController::CFirstPersonComponent*>(a);
    comp->MovementSettings->RunMultiplier = 20.f;
    transformComponent->IsDynamic = 1;
}

int main()
{
    Initialize();
    bool didLoad = CResourceLoader::Load("Assets/Scenes/Test.ssd");

    if (!didLoad)
    {
        std::cout << "Couldn't load the scene" << std::endl;
        return 1;
    }
    
    CSolver* solver = new CSolver();
    solver->AddSystem("TransformSystem");
    solver->AddSystem("RenderingSystem");
    solver->AddSystem("FirstPersonSystem");
    solver->InitializeSystems();

    while (solver->Simulate())
    {
         if (!CGraphics::Render())
             break;
    }
    return 0;
}
