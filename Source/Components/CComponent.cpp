#include "CComponent.h"
#include "SComponentFactory.h"

SComponentFactory::TComponentTypeMap * SComponentFactory::Map = NULL;
CComponent::~CComponent()
{
}

IObject* CComponent::Owner()
{
    return ObjectOwner;
}
void CComponent::SetOwner(IObject* owner)
{
    ObjectOwner = owner;
}
