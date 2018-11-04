#include "System.h"
#include "Utility/Data/Serialization.h"
#include <vector>

class IRenderContext;
class ICommandBuffer;
class RenderingSystem : public System, public IObject {
public:
  SERIALIZE_CLASS(RenderingSystem);
  RenderingSystem(){};
  virtual ~RenderingSystem();
  virtual bool Initialize();
  virtual bool Update();

private:
  void FindLights();
  void DrawSkyBox();
  void DrawOpaqueMeshes();

  void SetLightParameters(unsigned int shaderId);
  // private helper methods
  ICommandBuffer *ActiveCommandBuffer = nullptr;
  
#define MAX_LIGHTS 4

  unsigned char LightsFound = 0;
  class LightComponent *LightComponents[MAX_LIGHTS] = {nullptr, nullptr,
                                                       nullptr, nullptr};
  class LightComponent *CachedDirectionalLight = nullptr;
  class SkyLightComponent *CachedSkyLight = nullptr;
};
