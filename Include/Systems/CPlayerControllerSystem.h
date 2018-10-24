#include "CSystem.h"
#include "Utility/Data/Serialization.h"

class CCameraComponent;
class CPlayerControllerSystem : public CSystem, public ISerializedClass
{
public:
    SERIALIZE_CLASS(CPlayerControllerSystem)
    CPlayerControllerSystem(){};
    virtual ~CPlayerControllerSystem();
    virtual bool Initialize();
    virtual bool Update();
private:
    CCameraComponent* CachedCamera = NULL;
};
