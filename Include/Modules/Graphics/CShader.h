#pragma once
#include "Core.h"
#include "Utility/Data/Serialization.h"

enum EShaderType
{
    UNKNOWN = 0,
    VERTEX = 1, 
    FRAGMENT = 2,
    GEOMETRY = 3,
    COMPUTE = 4
};

class CShaderSource : public ISerializedClass
{
public:
    SERIALIZE_CLASS(CShaderSource)
    CShaderSource()
    {
        ATTRIBUTE_REGISTER(CShaderSource, Type)
        ATTRIBUTE_REGISTER(CShaderSource, Source)
    }
    ATTRIBUTE_VALUE(unsigned short, Type)
    ATTRIBUTE_VALUE(CString, Source)
};

class CShader : public ISerializedClass
{
public:
    SERIALIZE_CLASS(CShader)
    CShader()
    {
        ATTRIBUTE_REGISTER(CShader, Language)
        ATTRIBUTE_REGISTER(CShader, Source)
    }
    ATTRIBUTE_VALUE(CString, Language)
    ATTRIBUTE_CLASS_VECTOR(CShaderSource, Source)

    CString LastCompilationError;
    bool Valid = false;
};
