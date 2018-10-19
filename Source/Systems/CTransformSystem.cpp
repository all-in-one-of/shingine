#include "Systems/CTransformSystem.h"
#include "Modules/Statics/CStatics.h"
#include "Engine/Components/CTransformComponent.h"

void CTransformSystem::Initialize()
{
    // gather map of transforms
    UpdateComponentMap();
    // calculate transform component cache
    CalculateTransforms(false);

}

void CTransformSystem::CalculateTransforms(bool ignoreStatic)
{
    // recalculate transform on dynamic objects
    for (std::map<unsigned int, CTransformComponent*>::iterator it = Transforms.begin(); it != Transforms.end(); it++)
    {
    }
}

void CTransformSystem::CalculateTransform(CTransformComponent* transform)
{

}

void CTransformSystem::Update()
{
    // recalculate transform on dynamic objects
    CalculateTransforms();
}

void CTransformSystem::UpdateComponentMap()
{
    Transforms.clear();
    std::vector<IComponent*> genericComponents;
    CStatics::SceneManager()->CurrentScene()->GetComponents("Transform", genericComponents);
    for (size_t x = 0; x < genericComponents.size(); x++)
    {
        CTransformComponent* transformComponent = dynamic_cast<CTransformComponent*>(genericComponents[x]);
        Transforms[transformComponent->Owner()->ID()] = transformComponent;
    }
}
