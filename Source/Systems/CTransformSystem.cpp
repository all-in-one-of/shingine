#include "Systems/CTransformSystem.h"
#include "Modules/Statics/CComponentManager.h"
#include "Engine/Components/CTransformComponent.h"

#include <algorithm>
#include <glm/gtx/matrix_decompose.hpp>

void CTransformSystem::Initialize()
{
    // calculate transform component cache
    CalculateTransforms(false);
}

void CTransformSystem::CalculateTransforms(bool ignoreStatic)
{
    // cache the iterator
    CComponentManager::StringMap::iterator transformCollectionIterator;
    CComponentManager::Get()->GetComponentIteratorOfType("Transform", transformCollectionIterator);
    auto transformIterator = transformCollectionIterator->second.begin();
    for (transformIterator; transformIterator != transformCollectionIterator->second.end(); transformIterator++)
        CalculateTransform(transformIterator->second, transformCollectionIterator, ignoreStatic);
}

glm::mat4 CTransformSystem::CalculateTransform(IComponent* transformComponent, 
    std::unordered_map<std::string, std::unordered_map<unsigned int, IComponent*>>::iterator &transformCollectionIterator, bool ignoreStatic)
{
    CTransformComponent* transform = dynamic_cast<CTransformComponent*>(transformComponent);
    if (ignoreStatic && !transform->IsDynamic) 
        return transform->LocalTransform;
    
    glm::mat4 ident(1);
    glm::mat4 parentTransform = transform->ParentID == 0 
        ? ident 
        : CalculateTransform(transformCollectionIterator->second.at(transform->ParentID), transformCollectionIterator, ignoreStatic);

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
