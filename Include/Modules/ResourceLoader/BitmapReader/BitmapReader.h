#pragma once
#include <fstream>
#include <iostream>

struct ISerializedClass;
namespace BitmapReader
{
    bool ReadBitmapToTexture(const char* fileName, ISerializedClass* texture);
};
