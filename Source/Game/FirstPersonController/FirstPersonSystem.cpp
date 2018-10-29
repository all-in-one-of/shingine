#include "Game/FirstPersonController/FirstPersonSystem.h"
#include "Modules/Statics/ComponentManager.h"
#include "Game/FirstPersonController/FirstPersonComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Components/CameraComponent.h"

#include "Modules/Statics/Input.h"

#include "Utility/Typedefs.h"

#include <iostream>

#define DEG2RAD 0.017453292522222223f
#define RAD2DEG 57.29577950560105f

// TODO optimize
namespace FirstPersonController
{
    REGISTER_SERIALIZED_NAME(FirstPersonSystem)

    bool FirstPersonSystem::Initialize()
    {
        ComponentManager* componentManager = ComponentManager::Get();
        // find first person component
        FirstPersonComponent =
            componentManager->GetComponentOfType<FirstPersonController::FirstPersonComponent>();
        
        if (!FirstPersonComponent)
        {
            Active = false;
            return true;
        }
        PlayerTransform = 
            componentManager->GetComponentOfType<TransformComponent>(FirstPersonComponent->EntityId());
        Camera = 
            componentManager->GetComponentOfType<CameraComponent>(FirstPersonComponent->EntityId());
                
        Active = PlayerTransform != NULL && Camera != NULL;

        FirstPersonComponent->Yaw = 90.f;

       
        return true;
    }

    void FirstPersonSystem::UpdateRotation(glm::vec3 &Front, glm::vec3 &Horizontal, glm::vec3 &cameraFront, glm::vec3 &cameraUp)
    {
        float offsetX = Input::Get()->GetAxis(Input::AxisType::MouseX);
        float offsetY = -Input::Get()->GetAxis(Input::AxisType::MouseY);
        bool isMouseMoving = abs(offsetX) > .0001f || abs(offsetY) > .0001f;
        if (!isMouseMoving) return;

        float &xoffset = offsetX;
        float &yoffset = offsetY;
        float &yaw = FirstPersonComponent->Yaw;
        float &pitch = FirstPersonComponent->Pitch;

        float sensitivity = 0.05;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        Front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        Front.y = sin(glm::radians(pitch));
        Front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(Front);
        Horizontal = glm::normalize(glm::cross(Front, cameraUp));
    }

    void FirstPersonSystem::UpdateMovement(glm::vec3 &position, glm::vec3 &Front, glm::vec3 &Horizontal)
    {
        float deltaTime = 1 / 60.f;
        
        float factor = deltaTime * 
            (Input::Get()->GetMousePressed(S_INPUT_MOUSE_LEFT) 
            ? FirstPersonComponent->PlayerMovementSettings->RunMultiplier
            : 1.f);
        
        factor *= FirstPersonComponent->PlayerMovementSettings->ForwardSpeed;
        position = PlayerTransform->GetPosition();
        glm::vec3 translation(0);

        if (Input::Get()->GetKeyPressed(S_INPUT_KEY_W))
        {
            translation.x += factor * Front.x;
            translation.z += factor * Front.z;
        }
        
        if (Input::Get()->GetKeyPressed(S_INPUT_KEY_S))
        {
            translation.x -= factor * Front.x;
            translation.z -= factor * Front.z;
        }

        if (Input::Get()->GetKeyPressed(S_INPUT_KEY_A))
            translation -= Horizontal * factor;

        if (Input::Get()->GetKeyPressed(S_INPUT_KEY_D))
            translation += Horizontal * factor;

        if (Input::Get()->GetKeyPressed(S_INPUT_KEY_Q))
            translation.y += factor * 1.f;

        if (Input::Get()->GetKeyPressed(S_INPUT_KEY_E))
            translation.y += factor * -1.f;

        position += translation;
    }

    bool FirstPersonSystem::Update()
    {
        if (!Active)
            return true;

        glm::vec3 &Front = FirstPersonComponent->Front;
        glm::vec3 &Horizontal = FirstPersonComponent->Horizontal;
        glm::vec3 &cameraFront = FirstPersonComponent->CameraFront;
        glm::vec3 &cameraUp = FirstPersonComponent->CameraUp;

        glm::vec3 position = PlayerTransform->GetPosition();
        UpdateRotation(Front, Horizontal, cameraFront, cameraUp);
        UpdateMovement(position, Front, Horizontal);

        Camera->ViewMatrix = glm::lookAt(position, position + cameraFront, cameraUp);

        PlayerTransform->SetPosition(position);
        
        return true;
    }
};
