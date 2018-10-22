#include <iostream>
#include "Core.h"
#include "Modules/Statics/CAssetManager.h"
#include "Modules/Statics/CComponentManager.h"
#include "Modules/Statics/CEntityManager.h"

#include "Modules/Graphics/IShader.h"
#include "Modules/Graphics/CGraphics.h"
#include "Solver/CSolver.h"

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

}

int main()
{

    bool didLoad = CResourceLoader::Load("Assets/Scenes/TestScene.ssd");
    CSolver* solver = new CSolver();
    solver->AddSystem("TransformSystem");
    solver->AddSystem("RenderingSystem");
    solver->InitializeSystems();
    while (solver->Simulate());


    return 0;
}
