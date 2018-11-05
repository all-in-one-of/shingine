#include "Engine/Components/LightComponent.h"

REGISTER_COMPONENT(LightComponent);

LightComponent::LightComponent() {
  ATTRIBUTE_REGISTER(LightComponent, Color);
  ATTRIBUTE_REGISTER(LightComponent, Exposure);
  ATTRIBUTE_REGISTER(LightComponent, Intensity);
  
  ATTRIBUTE_REGISTER(LightComponent, LightType);
  
  ATTRIBUTE_REGISTER(LightComponent, Constant);
  ATTRIBUTE_REGISTER(LightComponent, Linear);
  ATTRIBUTE_REGISTER(LightComponent, Quadratic);
  
  ATTRIBUTE_REGISTER(LightComponent, InnerAngle);
  ATTRIBUTE_REGISTER(LightComponent, OuterAngle);
  ATTRIBUTE_REGISTER(LightComponent, CutOff);
  ATTRIBUTE_REGISTER(LightComponent, ShadowEnabled);
  
  Color = {1, 1, 1};
  Exposure = 1.f;
  Intensity = 1.f;

  LightType = POINT_LIGHT_TYPE;

  Constant = 1.0f;
  Linear = 0.09f;
  Quadratic = 0.032f;

  CutOff = 0.9762959977f;
  ShadowEnabled = 0;

  InnerAngle = 15.f;
  OuterAngle = 45.f;
};

LightComponent::~LightComponent() {}

glm::vec4 LightComponent::GetColor() {
  return glm::vec4(Color[0], Color[1], Color[2], 1.f);
}
