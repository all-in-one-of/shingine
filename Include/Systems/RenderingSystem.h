#include "System.h"
#include "Utility/Data/Serialization.h"

class IRenderContext;
class RenderingSystem : public System, public ISerializedClass
{
public:
    SERIALIZE_CLASS(RenderingSystem)
    RenderingSystem(){};
    virtual ~RenderingSystem();
    virtual bool Initialize();
    virtual bool Update();
};
