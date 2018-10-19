#include "ISystem.h"

class IRenderContext;
class CRenderingSystem : public ISystem
{
public:
    virtual ~CRenderingSystem();
    virtual void Initialize();
    virtual void Update();

private:
    IRenderContext* Renderer;
};
