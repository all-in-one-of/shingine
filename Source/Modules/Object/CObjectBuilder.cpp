#include <iostream>
#include "../../Utility/Data/IDataNode.h"
#include "../../Utility/Data/IAttributeSerialized.h"
#include "CObjectBuilder.h"

void CObjectBuilder::SetupFromDataNode(IObject* object, IDataNode* node)
{
    std::vector<IDataNode*> childNodes = node->GetNodes();
    for (unsigned char x = 0; x < childNodes.size(); x++)
    {
        IDataNode* childNode = childNodes[x];
        IComponent* component = object->AddComponent(childNode->Name());
        
        if (component == NULL)
            continue;

        std::vector<ITypedAttribute*> attributes = childNode->GetAttributes();

        for (unsigned char x = 0; x < attributes.size(); x++)
        {
            IAttributeSerialized* attributeSerializedObject = 
                dynamic_cast<IAttributeSerialized*>(component);
                
            if (attributeSerializedObject == NULL) 
                continue;

            attributeSerializedObject->SetAttribute(attributes[x]);
        }
    }
}
