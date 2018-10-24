#include "Systems/CTransformSystem.h"
#include "Modules/Statics/CComponentManager.h"
#include "Engine/Components/CTransformComponent.h"

#include <algorithm>
#include <glm/gtx/matrix_decompose.hpp>

REGISTER_SERIALIZED_NAME(CTransformSystem, TransformSystem)

bool CTransformSystem::Initialize()
{
    // get reference to the transform component collection
    CComponentManager::Get()->GetComponentIteratorOfType("Transform", TransformCollectionIterator);
    // calculate transform component for dynamic and static objects
    CalculateTransforms(false);
    Active = true;
    return Active;
}

void CTransformSystem::CalculateTransforms(bool ignoreStatic)
{
    // recalculate matrices based on local PSR values
    CalculateLocalTransforms(ignoreStatic);
    // gather parent matrices to get the world transform matrices
    CalculateWorldTransforms(ignoreStatic);
}

void CTransformSystem::CalculateLocalTransforms(bool ignoreStatic)
{
    CComponentManager::IdMap &idMap = TransformCollectionIterator->second;
    CComponentManager::IdMap::iterator transformIterator;
    for (transformIterator = idMap.begin(); transformIterator != idMap.end(); transformIterator++)
    {
        // iterate over transform components
        CTransformComponent* transform = dynamic_cast<CTransformComponent*>(transformIterator->second);
         
        if (ignoreStatic && !transform->IsDynamic) 
            continue;

        glm::mat4 ident(1);
        // TODO ensure the rotation is calculated correctly
        glm::mat4 transformNoScale = glm::translate(ident,
            transform->GetPosition()) * GetRotationMatrix(transform->GetRotation());

        glm::vec3 localScale = transform->GetScale();
        std::vector<float> ls = {localScale.x, localScale.y, localScale.z};
        float maxScaleAxis = *std::max_element(ls.begin(), ls.end());

        transform->LocalTransform = transformNoScale *
            glm::scale(ident, transform->GetScale());

        transform->LocalTransformUniformScale = transformNoScale *
            glm::scale(ident, glm::vec3(maxScaleAxis));
    }    
}

glm::mat4 CTransformSystem::CalculateWorldTransforms(bool ignoreStatic)
{
    CComponentManager::IdMap &idMap = TransformCollectionIterator->second;
    CComponentManager::IdMap::iterator transformIterator;
    for (transformIterator = idMap.begin(); transformIterator != idMap.end(); transformIterator++)
    {
        // iterate over transform components
        CTransformComponent* transform = dynamic_cast<CTransformComponent*>(transformIterator->second);

        if (ignoreStatic && !transform->IsDynamic) 
            continue;

        glm::mat4 parentTransform(1);
        glm::mat4 parentTransformUniformScale(1);
        CTransformComponent* currentTransform = transform;
        CComponentManager::IdMap::iterator parentIterator;

        unsigned int parentId = currentTransform->ParentID;
        unsigned char checkDepth = 255;

        for (unsigned char x = 0; x < checkDepth; x++) 
        {
            if (parentId == 0) 
                break;

            parentIterator = idMap.find(parentId);
            if (parentIterator == idMap.end())
                break;
            currentTransform = dynamic_cast<CTransformComponent*>(parentIterator->second);
            // if static then use world matrix and break

            parentTransform = parentTransform * currentTransform->LocalTransform;
            parentTransformUniformScale = parentTransformUniformScale * currentTransform->LocalTransformUniformScale;
            
            parentId = currentTransform->ParentID;
        }
        transform->WorldTransform = transform->LocalTransform * parentTransform;
        transform->WorldTransformUniformScale = 
            transform->WorldTransformUniformScale * parentTransformUniformScale;

        transform->WorldTransformInv = glm::inverse(glm::transpose(transform->WorldTransform));

        glm::vec3 skew;
        glm::vec4 perspective;
        
        glm::decompose(transform->WorldTransform, transform->WorldScale, transform->WorldRotation, 
            transform->WorldPosition, skew, perspective);

        transform->WorldRotation = glm::conjugate(transform->WorldRotation);
    }
}

bool CTransformSystem::Update()
{
    CalculateTransforms();
    return true;
}

glm::mat4 CTransformSystem::GetRotationMatrix(glm::quat q)
{
    float s = glm::dot(q, q);
	return glm::mat4(
		1-2*s*(q.y*q.y + q.z*q.z), 2*s*(q.x*q.y - q.z*q.w), 2*s*(q.x*q.z + q.y*q.w), 0,
		2*s*(q.x*q.y + q.z*q.w), 1-2*s*(q.x*q.x + q.z*q.z), 2*s*(q.y*q.z - q.x*q.w), 0,
		2*s*(q.x*q.z - q.y*q.w), 2*s*(q.y*q.z + q.x*q.w), 1-2*s*(q.x*q.x + q.y*q.y), 0,
        0, 0, 0, 1  
	);
}
