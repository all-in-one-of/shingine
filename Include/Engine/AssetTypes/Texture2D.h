#pragma once
#include "Utility/Data/Serialization.h"

struct Pixel32 { float r, g, b, a; };

class Texture2D : public ISerializedClass
{
public:
    SERIALIZE_CLASS(Texture2D)
    Texture2D()
    {
        ATTRIBUTE_REGISTER(Texture2D, Name)

        ATTRIBUTE_REGISTER(Texture2D, Width)
        ATTRIBUTE_REGISTER(Texture2D, Height)

        ATTRIBUTE_REGISTER(Texture2D, Red)
        ATTRIBUTE_REGISTER(Texture2D, Green)
        ATTRIBUTE_REGISTER(Texture2D, Blue)
        ATTRIBUTE_REGISTER(Texture2D, Alpha)
    };
    virtual ~Texture2D();
    
    ATTRIBUTE_VALUE(String, Name)

    Pixel32* GetPixels();
    void SetPixels(unsigned int width, unsigned int height, Pixel32 *newData);


private:
    ATTRIBUTE_VALUE(unsigned int, Width)
    ATTRIBUTE_VALUE(unsigned int, Height)

    ATTRIBUTE_VECTOR(float, Red)
    ATTRIBUTE_VECTOR(float, Green)
    ATTRIBUTE_VECTOR(float, Blue)
    ATTRIBUTE_VECTOR(float, Alpha)

    Pixel32* Data = nullptr;
};
