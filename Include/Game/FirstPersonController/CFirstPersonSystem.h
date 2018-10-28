#pragma once
#include "Systems/CSystem.h"
#include "Utility/Data/Serialization.h"
#include "Utility/Typedefs.h"

class CTransformComponent;
class CCameraComponent;
namespace FirstPersonController
{
    class CFirstPersonSystem : public CSystem, public ISerializedClass
    {
    public:
        SERIALIZE_CLASS(CFirstPersonSystem)
        ~CFirstPersonSystem() {};
        virtual bool Initialize();
        virtual bool Update();
    private:
        void UpdateRotation(glm::vec3 &Front, glm::vec3 &Horizontal, glm::vec3 &cameraFront, 
            glm::vec3 &cameraUp);
        void UpdateMovement(glm::vec3 &position, glm::vec3 &Front, glm::vec3 &Horizontal);
        class CFirstPersonComponent* FirstPersonComponent = NULL;

        CTransformComponent* PlayerTransform = NULL;
        CCameraComponent* Camera = NULL;
    };
};
