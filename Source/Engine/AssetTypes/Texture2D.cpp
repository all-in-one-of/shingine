#include "Engine/AssetTypes/Texture2D.h"
REGISTER_SERIALIZED_CLASS(Texture2D);

Texture2D::~Texture2D() {
  // if (Data != nullptr)
  //   delete[] Data;
}

float *Texture2D::GetPixels() {
  return Pixels.data();
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
}
