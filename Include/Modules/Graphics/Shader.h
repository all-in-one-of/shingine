#pragma once
#include "Utility/Data/Serialization.h"
#include "IShader.h"

class ShaderSource : public ISerializedClass
{
public:
    SERIALIZE_CLASS(ShaderSource)
    ShaderSource()
    {
        ATTRIBUTE_REGISTER(ShaderSource, Type)
        ATTRIBUTE_REGISTER(ShaderSource, Source)
    }
    ATTRIBUTE_VALUE(unsigned short, Type)
    ATTRIBUTE_VALUE(String, Source)
};

class Shader : public IShader, public ISerializedClass
{
public:
    SERIALIZE_CLASS(Shader)
    Shader()
    {
        ATTRIBUTE_REGISTER(Shader, Language)
        ATTRIBUTE_REGISTER(Shader, Source)
    }
    ATTRIBUTE_VALUE(String, Language)
    ATTRIBUTE_CLASS_VECTOR(ShaderSource, Source)

    virtual unsigned int AssetId();
    virtual void AddSource(EShaderType type, const String &source);
    virtual bool GetSource(EShaderType type, String &source);

    String LastCompilationError;
    bool Valid = false;
};
