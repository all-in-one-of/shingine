#include <iostream>
#include "../../Utility/Data/IDataNode.h"
#include "../../Utility/Data/ISerialized.h"
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

        ISerializedClass* attributeSerializedObject = 
            dynamic_cast<ISerializedClass*>(component);

        std::vector<ISerialized*> attributes = childNode->GetAttributes();
        std::vector<IDataNode*> childDataNodes = childNode->GetNodes();

        for (unsigned char x = 0; x < attributes.size(); x++)
            attributeSerializedObject->SetAttribute(attributes[x]);

        for (unsigned char x = 0; x < childDataNodes.size(); x++)
        {
            int a = 1;
            ISerialized* deserializedDataNode = childDataNodes[x]->Deserialize();
            attributeSerializedObject->SetAttribute(deserializedDataNode);
        }
    }
}
