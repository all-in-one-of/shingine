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
        std::cout << "Attempting to add component " << childNode->Name().GetStdString() << std::endl;
        IComponent* component = object->AddComponent(childNode->Name());

        std::vector<ITypedAttribute*> attributes = childNode->GetAttributes();

        if (attributes.size() > 0)
            int e = 2;

        for (unsigned char x = 0; x < attributes.size(); x++)
        {
            IAttributeSerialized* attributeSerializedObject = 
                dynamic_cast<IAttributeSerialized*>(component);
                
            if (attributeSerializedObject == NULL) continue;
            std::cout << "Calling set attr" << std::endl;
            attributeSerializedObject->SetAttribute(attributes[x]);
            int z = 4;
        }
    }
}
