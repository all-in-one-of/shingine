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
        void UpdateRotation(glm::quat &orientation);
        void UpdateMovement(glm::vec3 &position, glm::quat &orientation);
        class CFirstPersonComponent* FirstPersonComponent = NULL;

        CTransformComponent* PlayerTransform = NULL;
        CCameraComponent* Camera = NULL;
    };
};
