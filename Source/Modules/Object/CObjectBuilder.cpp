#include "CObjectBuilder.h"
#include "../../Utility/SSD.h"

#include "../../Components/CComponentBuilder.h"

#include <iostream>

void CObjectBuilder::SetupObjectFromDescriptionNode(IObject* object, SSD::SNode* node)
{
    CComponentBuilder* componentBuilder = new CComponentBuilder();
    for (unsigned char x = 0; x < node->AttributeCount; x++)
    {
        SSD::SAttribute* attr = node->Attributes[x];
        std::cout << "Attempting to add component " << attr->Name << std::endl;
        IComponent* component = object->AddComponent(attr->Name);
        
    }
    delete componentBuilder;
}