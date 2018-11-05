#pragma once
#include "ComponentSetup.h"
#include "Core.h"

class SkyLightComponent : public Component {
public:
  COMPONENT_CLASS(SkyLightComponent);

  SkyLightComponent();
  virtual ~SkyLightComponent();

  ATTRIBUTE_GLM_VEC3(Color);
  ATTRIBUTE_VALUE(float, Intensity);

  ATTRIBUTE_VALUE(unsigned char, CubemapEnabled);
  ATTRIBUTE_ID(CubemapId);
};
