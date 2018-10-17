#include "CComponent.h"

CComponent::~CComponent()
{
}

IObject* CComponent::Owner() { return ObjectOwner; }
void CComponent::SetOwner(IObject* owner) { ObjectOwner = owner; }
