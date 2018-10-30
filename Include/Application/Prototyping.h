#pragma once

#include "Modules/Statics/IActiveCamera.h"
#include "Engine/Components/TransformComponent.h"
#include "Game/FirstPersonController/FirstPersonComponent.h"
#include "Engine/Components/RendererComponent.h"
#include "Modules/Statics/IComponentManager.h"

#include "Engine/AssetTypes/Material.h"
#include "Modules/Statics/IAssetManager.h"
#include "Modules/Graphics/GraphicsUtils.h"
#include "Modules/Graphics/GraphicsUtils.h"

void AddFirstPersonController()
{
    TransformComponent *transformComponent = 
        Statics::Get<IActiveCamera>()->GetTransformComponent();
    // Add first person component
    FirstPersonController::FirstPersonComponent *comp = 
        Statics::Get<IComponentManager>()->AddComponent<FirstPersonController::FirstPersonComponent>(
            transformComponent->EntityId());

    comp->PlayerMovementSettings->RunMultiplier = 5.f;
    comp->PlayerMovementSettings->ForwardSpeed = 13.f;
    // Won't recalculate transform matrices without this set to 1
    transformComponent->IsDynamic = 1;
}

void TestStuff()
{
    // load texture
    ISerializedClass *tex;
    ResourceLoader::LoadBitmap("Assets/Textures/uv_checker.bmp", tex);

    IShader *shader = GraphicsUtils::CreateVertexFragmentShader(
        "Assets/Shaders/AlbedoDrawer.vert", "Assets/Shaders/AlbedoDrawer.frag");
    Material *mat = Statics::Get<IAssetManager>()->AddAssetOfType<Material>();
    mat->Name = "TexturePreviewMaterial";
    mat->ShaderId = shader->AssetId();
    mat->SetTexture("MainTex", tex->UniqueID());

    IComponentManager::StringMap::iterator iterator;
    Statics::Get<IComponentManager>()->GetComponentIteratorOfType("RendererComponent", iterator);
    IComponentManager::IdMap &map = iterator->second;
    IComponentManager::IdMap::iterator it = map.begin();

    for(it = map.begin(); it != map.end(); it++)
    {
        RendererComponent* r = dynamic_cast<RendererComponent*>(it->second);
        r->MaterialReference = mat->UniqueID();
    }

}
