#pragma once
#include "ComponentSetup.h"

class CObjectMetadataComponent : public CComponent, public ISerializedClass
{
public:
    ATTRIBUTE_DECL_INIT(CObjectMetadataComponent)
    CObjectMetadataComponent()
    {
        ATTRIBUTE_REGISTER(CObjectMetadataComponent, Name)
        ATTRIBUTE_REGISTER(CObjectMetadataComponent, Tag)
        ATTRIBUTE_REGISTER(CObjectMetadataComponent, Layer)
    }
    virtual ~CObjectMetadataComponent() {};
    REGISTER_SERIALIZED_TYPE(CObjectMetadataComponent)
private:
    ATTRIBUTE_VALUE_INFO(CString, Name)
    ATTRIBUTE_VALUE_INFO(CString, Tag)
    ATTRIBUTE_VALUE_INFO(CString, Layer)
};
