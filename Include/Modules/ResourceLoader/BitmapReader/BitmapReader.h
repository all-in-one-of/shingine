#pragma once
#include <fstream>
#include <iostream>

class ISerializedClass;
namespace BitmapReader {
bool ReadBitmapToTexture(const char *fileName, ISerializedClass *texture);
};
