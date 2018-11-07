#pragma once
#include "Utility/Data/Serialization.h"
#include "Asset.h"

class Texture2D : public Asset, public IObject {
public:
  enum TextureFormat { Float_RGBA, Byte_RGBA };
  SERIALIZE_CLASS(Texture2D);
  Texture2D() {
    ATTRIBUTE_REGISTER(Texture2D, Name);
    ATTRIBUTE_REGISTER(Texture2D, Format);

    ATTRIBUTE_REGISTER(Texture2D, Width);
    ATTRIBUTE_REGISTER(Texture2D, Height);

    ATTRIBUTE_REGISTER(Texture2D, Pixels);
    ATTRIBUTE_REGISTER(Texture2D, Pixels32);
  };
  virtual ~Texture2D();

  ATTRIBUTE_VALUE(String, Name);

  float *GetPixels();
  unsigned char *GetPixels32();
  void SetPixels(unsigned int width, unsigned int height, float *newData);
  void SetPixels(unsigned int width, unsigned int height, unsigned char *newData);
  void GetResolution(int &width, int &height);
  TextureFormat GetTextureFormat();
private:
  ATTRIBUTE_VALUE(unsigned int, Format);
  ATTRIBUTE_VALUE(unsigned int, Width);
  ATTRIBUTE_VALUE(unsigned int, Height);

  ATTRIBUTE_VECTOR(float, Pixels);
  ATTRIBUTE_VECTOR(unsigned char, Pixels32);
};
