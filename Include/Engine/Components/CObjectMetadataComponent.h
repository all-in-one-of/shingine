#pragma once
#include "ComponentSetup.h"

class CObjectMetadataComponent : public CComponent
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
    ATTRIBUTE_VALUE(CString, Name)
    ATTRIBUTE_VALUE(CString, Tag)
    ATTRIBUTE_VALUE(CString, Layer)
};
