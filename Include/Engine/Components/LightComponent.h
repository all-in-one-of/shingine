#pragma once
#include "ComponentSetup.h"
#include <glm/glm.hpp>

#define POINT_LIGHT_TYPE 0
#define SPOT_LIGHT_TYPE 1
#define DIRECTIONAL_LIGHT_TYPE 2

class LightComponent : public Component {
public:
  COMPONENT_CLASS(LightComponent);

  LightComponent();
  virtual ~LightComponent();


  glm::vec4 GetColor();

  ATTRIBUTE_GLM_VEC3(Color);
  ATTRIBUTE_VALUE(float, Exposure);
  ATTRIBUTE_VALUE(float, Intensity);

  // 0 point 1 spot 2 directional
  ATTRIBUTE_VALUE(unsigned char, LightType);
  // attenuation
  ATTRIBUTE_VALUE(float, Constant);
  ATTRIBUTE_VALUE(float, Linear);
  ATTRIBUTE_VALUE(float, Quadratic);

  ATTRIBUTE_VALUE(float, InnerAngle);
  ATTRIBUTE_VALUE(float, OuterAngle);

  ATTRIBUTE_VALUE(float, CutOff);

  ATTRIBUTE_VALUE(unsigned char, ShadowEnabled);

};
