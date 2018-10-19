#include "Systems/CTransformSystem.h"
#include "Modules/Statics/CStatics.h"
#include "Engine/Components/CTransformComponent.h"

#include <algorithm>
#include <glm/gtx/matrix_decompose.hpp>

void CTransformSystem::Initialize()
{
    // gather map of transforms
    UpdateComponentMap();
    // calculate transform component cache
    CalculateTransforms(false);

}

void CTransformSystem::CalculateTransforms(bool ignoreStatic)
{
    for (std::map<unsigned int, CTransformComponent*>::iterator it = Transforms.begin(); it != Transforms.end(); it++)
    {
        CalculateTransform(it->second, ignoreStatic);
    }
}

glm::mat4 CTransformSystem::CalculateTransform(CTransformComponent* transform, bool ignoreStatic)
{
    if (ignoreStatic && !transform->IsDynamic) return transform->LocalTransform;
    glm::mat4 ident(1);
    glm::mat4 parentTransform = transform->ParentID == 0 
        ? ident 
        : CalculateTransform(Transforms.at(transform->ParentID));

    glm::mat4 transformNoScale = glm::translate(ident, 
        transform->GetLocalPosition()) * glm::toMat4(transform->GetLocalRotation());

    transform->LocalTransform = transformNoScale *
        glm::scale(ident, transform->GetLocalScale());

    transform->WorldTransform = transform->LocalTransform * parentTransform;

    float maxScaleAxis = *std::max_element(
            transform->LocalScale.begin(), transform->LocalScale.end());

    transform->WorldTransformUniformScale = transformNoScale *
        glm::scale(ident, glm::vec3(maxScaleAxis)) * parentTransform;

    glm::vec3 skew;
    glm::vec4 perspective;
    
    glm::decompose(transform->WorldTransform, transform->Scale, transform->Rotation, 
        transform->Position, skew, perspective);

    transform->Rotation = glm::conjugate(transform->Rotation);

    return transform->LocalTransform;
}

void CTransformSystem::Update()
{
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
