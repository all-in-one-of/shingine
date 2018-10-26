#include "Game/FirstPersonController/CFirstPersonSystem.h"
#include "Modules/Statics/CComponentManager.h"
#include "Game/FirstPersonController/CFirstPersonComponent.h"
#include "Engine/Components/CTransformComponent.h"
#include "Engine/Components/CCameraComponent.h"

#include "Modules/Statics/CInput.h"

#include "Utility/Typedefs.h"

#include <iostream>

#define DEG2RAD 0.017453292522222223f
#define RAD2DEG 57.29577950560105f

// TODO optimize
namespace FirstPersonController
{
    REGISTER_SERIALIZED_NAME(CFirstPersonSystem, FirstPersonSystem)

    bool CFirstPersonSystem::Initialize()
    {
        CComponentManager* componentManager = CComponentManager::Get();
        // find first person component
        FirstPersonComponent = 
            componentManager->GetComponentOfType<CFirstPersonComponent>("FirstPersonComponent");
        
        if (FirstPersonComponent)
        {
            PlayerTransform = 
                componentManager->GetComponentOfType<CTransformComponent>(
                    "Transform", FirstPersonComponent->EntityId());
            Camera = 
                componentManager->GetComponentOfType<CCameraComponent>(
                    "Camera", FirstPersonComponent->EntityId());
                    
            Active = PlayerTransform != NULL && Camera != NULL;
        }
        else
            Active = false;

        return true;
    }

    void CFirstPersonSystem::UpdateRotation(glm::quat &orientation)
    {
        orientation = PlayerTransform->GetRotation();

        float yRot = CInput::Get()->GetAxis(CInput::AxisType::MouseX) *
            FirstPersonComponent->ViewSettings->SensitivityX;
        float xRot = CInput::Get()->GetAxis(CInput::AxisType::MouseY) *
            FirstPersonComponent->ViewSettings->SensitivityY;
        
        float speed = 1.f;
         orientation *= glm::angleAxis(yRot * speed, glm::vec3(0,1,0) * orientation);
         orientation *= glm::angleAxis(-xRot * speed, glm::vec3(1,0,0) * orientation);
    }

    void CFirstPersonSystem::UpdateMovement(glm::vec3 &position, glm::quat &orientation)
    {
        float deltaTime = 1 / 60.f;
        
        float factor = deltaTime * 
            (CInput::Get()->GetMousePressed(S_INPUT_MOUSE_LEFT) 
            ? FirstPersonComponent->MovementSettings->RunMultiplier
            : 1.f);

        position = PlayerTransform->GetPosition();

        glm::vec3 translation(0);

        if (CInput::Get()->GetKeyPressed(S_INPUT_KEY_W))
            translation.z += factor * 1.f;
        
        if (CInput::Get()->GetKeyPressed(S_INPUT_KEY_S))
            translation.z += factor * -1.f;

        if (CInput::Get()->GetKeyPressed(S_INPUT_KEY_A))
            translation.x += factor * 1.f;

        if (CInput::Get()->GetKeyPressed(S_INPUT_KEY_D))
            translation.x += factor * -1.f;

        if (CInput::Get()->GetKeyPressed(S_INPUT_KEY_Q))
            translation.y += factor * -1.f;

        if (CInput::Get()->GetKeyPressed(S_INPUT_KEY_E))
            translation.y += factor * 1.f;

        position += translation * orientation;
    }

    bool CFirstPersonSystem::Update()
    {
        if (!Active)
            return true;

        glm::vec3 position;
        glm::quat orientation;

        UpdateRotation(orientation);
        UpdateMovement(position, orientation);

        glm::vec3 at = glm::vec3(0,0,1) * orientation;
        glm::vec3 up = glm::vec3(0,1,0) * orientation;

        Camera->ViewMatrix = glm::lookAt(position, position + at, up);

        PlayerTransform->SetPosition(position);
        PlayerTransform->SetRotation(orientation);
        
        return true;
    }
};
