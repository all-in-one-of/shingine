#include "Modules/ResourceLoader/BitmapReader/BitmapReader.h"
#include <fstream>
#include <iostream>

#include "Utility/Data/SerializedFactory.h"
#include "Engine/AssetTypes/Texture2D.h"

namespace BitmapReader
{
    bool ReadBitmapToTexture(const char* fileName, ISerializedClass* texture)
    {
        unsigned char headerInfo[54] = {0};
        unsigned int width = 0;
        unsigned int height = 0;
        unsigned int size = 0;
        unsigned short bitCount = 0;
        unsigned char* pixelsRaw = nullptr;
        Pixel32* pixels = nullptr;

        Texture2D* tex = dynamic_cast<Texture2D*>(texture);
        if(!tex) 
            return;

        FILE *file = fopen(fileName, "rb");
        if (!file)
            return false;

        fread(headerInfo, sizeof(unsigned char), 54, file);
        width = *(unsigned int *) &headerInfo[18];
        height = *(unsigned int *) &headerInfo[22];
        size = width * height * 3;
        bitCount = *(unsigned short *) &headerInfo[28];
        
        if (bitCount != 24)
        {
            fclose(file);
            return false;
        }
        pixelsRaw = new unsigned char[size];
        fread(pixelsRaw, sizeof(unsigned char), size, file);
        fclose(file);

        pixels = new Pixel32[width * height]; 
        for (unsigned int x = 0; x < size; x+=3)
        {
            Pixel32 pixel;
            pixel.r = pixelsRaw[x + 2] / 255.f;
            pixel.g = pixelsRaw[x + 1] / 255.f;
            pixel.b = pixelsRaw[x] / 255.f;
            pixel.a = 1.f;

            pixels[x / 3] = pixel; 
        }

        tex->Name = fileName;
        tex->SetPixels(width, height, pixels);

        delete[] pixelsRaw;
        fclose(file);
        return true;
    }
};
