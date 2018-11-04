#pragma once
#include <fstream>
#include <iostream>

class IObject;
namespace BitmapReader {
bool ReadBitmapToTexture(const char *fileName, IObject *texture);
};
