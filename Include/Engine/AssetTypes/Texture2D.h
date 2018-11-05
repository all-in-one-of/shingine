#pragma once
#include "Utility/Data/Serialization.h"
#include "Asset.h"

class Texture2D : public Asset, public IObject {
public:
  SERIALIZE_CLASS(Texture2D);
  Texture2D() {
    ATTRIBUTE_REGISTER(Texture2D, Name);

    ATTRIBUTE_REGISTER(Texture2D, Width);
    ATTRIBUTE_REGISTER(Texture2D, Height);

    ATTRIBUTE_REGISTER(Texture2D, Pixels);
  };
  virtual ~Texture2D();

  ATTRIBUTE_VALUE(String, Name);

  float *GetPixels();
  void SetPixels(unsigned int width, unsigned int height, float *newData);
  void GetResolution(int &width, int &height);

private:
  ATTRIBUTE_VALUE(unsigned int, Width);
  ATTRIBUTE_VALUE(unsigned int, Height);

  ATTRIBUTE_VECTOR(float, Pixels);
};
