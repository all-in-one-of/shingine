#include "Engine/AssetTypes/Texture2D.h"
REGISTER_SERIALIZED_CLASS(Texture2D);

Texture2D::~Texture2D() {}
float *Texture2D::GetPixels() { return Pixels.data(); }
unsigned char *Texture2D::GetPixels32() { return Pixels32.data(); }
void Texture2D::SetPixels(unsigned int width, unsigned int height,
                          unsigned char *newData) {
  Pixels32.clear();
  for (unsigned int x = 0; x < width * height * 4; x++)
    Pixels32.push_back(newData[x]);
  Width = width;
  Height = height;
  Format = Byte_RGBA;
}

void Texture2D::GetResolution(int &width, int &height) {
  width = Width;
  height = Height;
}

void Texture2D::SetPixels(unsigned int width, unsigned int height,
                          float *newData) {
  Pixels.clear();
  for (unsigned int x = 0; x < width * height * 4; x++)
    Pixels.push_back(newData[x]);
  Width = width;
  Height = height;
  Format = Float_RGBA;
}

Texture2D::TextureFormat Texture2D::GetTextureFormat() {
  return (TextureFormat)Format;
}
