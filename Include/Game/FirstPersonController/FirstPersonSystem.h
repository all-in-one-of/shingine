#pragma once
#include "Systems/System.h"
#include "Utility/Data/Serialization.h"
#include "Utility/Typedefs.h"

class TransformComponent;
class CameraComponent;
namespace FirstPersonController
{
    class FirstPersonSystem : public System, public ISerializedClass
    {
    public:
        SERIALIZE_CLASS(FirstPersonSystem)
        ~FirstPersonSystem() {};
        virtual bool Initialize();
        virtual bool Update();
    private:
        void UpdateRotation(glm::vec3 &Front, glm::vec3 &Horizontal, glm::vec3 &cameraFront, 
            glm::vec3 &cameraUp);
        void UpdateMovement(glm::vec3 &position, glm::vec3 &Front, glm::vec3 &Horizontal);
        class FirstPersonComponent* FirstPersonComponent = nullptr;

        TransformComponent* PlayerTransform = nullptr;
        CameraComponent* Camera = nullptr;
    };
};
