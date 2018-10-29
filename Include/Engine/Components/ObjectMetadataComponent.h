#pragma once
#include "ComponentSetup.h"

class ObjectMetadataComponent : public Component
{
public:
    SERIALIZE_CLASS(ObjectMetadataComponent)
    ObjectMetadataComponent()
    {
        ATTRIBUTE_REGISTER(ObjectMetadataComponent, Name)
        ATTRIBUTE_REGISTER(ObjectMetadataComponent, Tag)
        ATTRIBUTE_REGISTER(ObjectMetadataComponent, Layer)
    }
    virtual ~ObjectMetadataComponent() {};
private:
    ATTRIBUTE_VALUE(String, Name)
    ATTRIBUTE_VALUE(String, Tag)
    ATTRIBUTE_VALUE(String, Layer)
};
