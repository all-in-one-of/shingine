#include "Modules/ResourceLoader/BitmapReader/BitmapReader.h"
#include <fstream>
#include <iostream>

#include "Engine/AssetTypes/Texture2D.h"
#include "Utility/Data/SerializedFactory.h"

namespace BitmapReader {
bool ReadBitmapToTexture(const char *fileName, IObject *texture) {
  unsigned char headerInfo[54] = {0};
  unsigned int width = 0;
  unsigned int height = 0;
  unsigned int size = 0;
  unsigned short bitCount = 0;
  unsigned char *pixelsRaw = nullptr;
  unsigned char *pixels32 = nullptr;

  Texture2D *tex = dynamic_cast<Texture2D *>(texture);
  if (!tex)
    return false;

  FILE *file = fopen(fileName, "rb");
  if (!file)
    return false;

  fread(headerInfo, sizeof(unsigned char), 54, file);
  width = *(unsigned int *)&headerInfo[18];
  height = *(unsigned int *)&headerInfo[22];
  size = width * height * 3;
  bitCount = *(unsigned short *)&headerInfo[28];

  if (bitCount != 24) {
    fclose(file);
    return false;
  }
  pixelsRaw = new unsigned char[size];
  fread(pixelsRaw, sizeof(unsigned char), size, file);
  fclose(file);

  pixels32 = new unsigned char[width * height * 4];
  unsigned int pixelCount = width * height;
  for (unsigned int x = 0; x < pixelCount; x++) {
    unsigned int index = x * 4;
    pixels32[index + 0] = pixelsRaw[x * 3 + 2];
    pixels32[index + 1] = pixelsRaw[x * 3 + 1];
    pixels32[index + 2] = pixelsRaw[x * 3 + 0];
    pixels32[index + 3] = 255;
  }
  tex->Name = fileName;
  tex->SetPixels(width, height, pixels32);

  delete[] pixelsRaw;
  delete[] pixels32;
  fclose(file);
  return true;
}
}; // namespace BitmapReader
