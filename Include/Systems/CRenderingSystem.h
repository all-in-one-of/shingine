#include "CSystem.h"
#include "Utility/Data/Serialization.h"

class IRenderContext;
class CRenderingSystem : public CSystem, public ISerializedClass
{
public:
    SERIALIZE_CLASS(CRenderingSystem)
    CRenderingSystem(){};
    virtual ~CRenderingSystem();
    virtual bool Initialize();
    virtual bool Update();

private:
    IRenderContext* Renderer;
};
