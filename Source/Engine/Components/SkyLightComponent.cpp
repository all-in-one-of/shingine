#include "Engine/Components/SkyLightComponent.h"

REGISTER_COMPONENT(SkyLightComponent);

SkyLightComponent::SkyLightComponent() {
  ATTRIBUTE_REGISTER(SkyLightComponent, Color);
  ATTRIBUTE_REGISTER(SkyLightComponent, Intensity);
  ATTRIBUTE_REGISTER(SkyLightComponent, CubemapEnabled);
  ATTRIBUTE_REGISTER(SkyLightComponent, CubemapId);

  // default values
  Color = {1, 1, 1};
  Intensity = 1.f;

  CubemapId = 0;
  CubemapEnabled = 0;
}

SkyLightComponent::~SkyLightComponent() {}
