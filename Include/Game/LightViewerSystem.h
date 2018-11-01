#pragma once
#include "Systems/System.h"
#include "Utility/Data/Serialization.h"
#include "Utility/Typedefs.h"

class LightComponent;
template <class T> class ComponentMap;

class LightViewerSystem : public System, public ISerializedClass {
public:
  SERIALIZE_CLASS(LightViewerSystem);
  ~LightViewerSystem(){};
  virtual bool Initialize();
  virtual bool Update();
private:
  unsigned int CubeMeshAssetId = 0;
  unsigned int UnlitShaderId = 0;
  ComponentMap<LightComponent> *LightComponentMap;
};
