#pragma once
#include "ComponentSetup.h"

class CObjectMetadataComponent : public CComponent, public ISerializedClass
{
public:
    SERIALIZE_CLASS(CObjectMetadataComponent)
    CObjectMetadataComponent()
    {
        ATTRIBUTE_REGISTER(CObjectMetadataComponent, Name)
        ATTRIBUTE_REGISTER(CObjectMetadataComponent, Tag)
        ATTRIBUTE_REGISTER(CObjectMetadataComponent, Layer)
    }
    virtual ~CObjectMetadataComponent() {};
private:
    ATTRIBUTE_VALUE_INFO(CString, Name)
    ATTRIBUTE_VALUE_INFO(CString, Tag)
    ATTRIBUTE_VALUE_INFO(CString, Layer)
};
