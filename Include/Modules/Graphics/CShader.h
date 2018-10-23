#pragma once
#include "Utility/Data/Serialization.h"
#include "IShader.h"

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

class CShader : public IShader, public ISerializedClass
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

    virtual unsigned int AssetId();
    virtual void AddSource(EShaderType type, const CString &source);
    virtual bool GetSource(EShaderType type, CString &source);

    CString LastCompilationError;
    bool Valid = false;
};
