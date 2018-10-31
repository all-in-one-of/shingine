#pragma once
#include "ComponentSetup.h"

class SkyLightComponent : public Component {
public:
  COMPONENT_CLASS(SkyLightComponent);

  SkyLightComponent();
  virtual ~SkyLightComponent();

  ATTRIBUTE_VECTOR(float, Color);
  ATTRIBUTE_VALUE(float, Intensity);

  ATTRIBUTE_VALUE(unsigned char, CubemapEnabled);
  ATTRIBUTE_ID(CubemapId);
};
