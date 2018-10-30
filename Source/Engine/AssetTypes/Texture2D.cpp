#include "Engine/AssetTypes/Texture2D.h"
REGISTER_SERIALIZED_NAME(Texture2D)

Texture2D::~Texture2D() 
{
    if (Data != nullptr)
        delete[] Data;
}

Pixel32* Texture2D::GetPixels()
{
    if (Data) return Data;

    if ((Red.size() > 0) && (Red.size() == Green.size() == Blue.size() == Alpha.size()))
    {
        Data = new Pixel32[Red.size()];
        for (unsigned int x = 0; x < Red.size(); x++)
        {
            Data[x].r = Red[x];
            Data[x].g = Green[x];
            Data[x].b = Blue[x];
            Data[x].a = Alpha[x];
        }
    }
    return Data;
}

void Texture2D::GetResolution(int &width, int& height)
{
    width = Width;
    height = Height;
}

void Texture2D::SetPixels(unsigned int width, unsigned int height, Pixel32 *newData)
{
    Red.clear(); Green.clear(); Blue.clear(); Alpha.clear();
    if (Data)
        delete[] Data;
        
    Data = newData;

    for (unsigned int x = 0; x < width * height; x++)
    {
        Red.push_back(newData[x].r);
        Green.push_back(newData[x].g);
        Blue.push_back(newData[x].b);
        Alpha.push_back(newData[x].a);
    }    
    Width = width;
    Height = height;

}

