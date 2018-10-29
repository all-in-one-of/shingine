#include "Modules/Statics/Statics.h"
#include "Utility/Data/IDataNode.h"
#include "Utility/Data/TypedAttribute.h"
#include "Utility/Data/UniqueIdSetter.h"

void UniqueIdSetter::SetIds(const std::vector<IDataNode*> &newNodes)
{
    UniqueIdSetter a = UniqueIdSetter(newNodes);
}
UniqueIdSetter::UniqueIdSetter(const std::vector<IDataNode*> &newNodes)
{
    Nodes = newNodes;
    // update ids for nodes first, then for attributes
    for (unsigned int x = 0; x < Nodes.size(); x++)
        UpdateUid(Nodes[x]);
    // update attribute uid
    for (unsigned int x = 0; x < Nodes.size(); x++)
        UpdateAttributeUid(Nodes[x]);
}

void UniqueIdSetter::UpdateUid(IDataNode* node)
{
    unsigned int newId = Statics::Get()->GetUniqueId();
    LocalToGlobalUid[node->GetUniqueID()] = newId;
    node->SetUniqueID(newId);

    std::vector<IDataNode*> childNodes = node->GetNodes();
    for (unsigned int x = 0; x < childNodes.size(); x++)
        UpdateUid(childNodes[x]);

    std::vector<ISerialized*> attributes = node->GetAttributes();
    for (size_t x = 0; x < attributes.size(); x++)
    {
        if ((attributes[x]->TypeName() == "SerializedClass") == false) 
            continue;

        TypedAttribute<IDataNode*>* attributeNodes = 
            dynamic_cast<TypedAttribute<IDataNode*>*>(attributes[x]);

        if (!attributeNodes) 
            continue;
            
        std::vector<IDataNode*> attributeNodesVec = attributeNodes->Get();
        for (size_t y = 0; y < attributeNodesVec.size(); y++)
            UpdateUid(attributeNodesVec[y]);
    }
}

void UniqueIdSetter::UpdateAttributeUid(IDataNode* node)
{
    std::vector<ISerialized*> attributes = node->GetAttributes();
    for (unsigned int x = 0; x < attributes.size(); x++)
    {   
        if(attributes[x]->TypeName() == "SerializedClass")
        {

            TypedAttribute<IDataNode*>* attributeNodes =
                dynamic_cast<TypedAttribute<IDataNode*>*>(attributes[x]);

            if(!attributeNodes)
                continue;

            std::vector<IDataNode*> attributeNodesVec = attributeNodes->Get();
            for(size_t y = 0; y < attributeNodesVec.size(); y++)
                UpdateAttributeUid(attributeNodesVec[y]);
        }

        if (attributes[x]->TypeName() == "uid")
        {
            // it's a value attribute
            TypedAttributeValue<unsigned int>* uniqueIdAttribute = 
                dynamic_cast<TypedAttributeValue<unsigned int>*>(attributes[x]);
            if (uniqueIdAttribute) 
            {

                unsigned int currentId = uniqueIdAttribute->Get();
                std::map<unsigned int, unsigned int>::iterator it = LocalToGlobalUid.find(currentId);
                if (it == LocalToGlobalUid.end())
                {
                    unsigned int id = 0;
                    uniqueIdAttribute->Set(id);
                    continue;
                }
                else
                {
                    uniqueIdAttribute->Set(it->second);
                }
            }
            // it's a vector attribute
            TypedAttribute<unsigned int>* uniqueIdAttributeVector = dynamic_cast<TypedAttribute<unsigned int>*>(attributes[x]);
            if (uniqueIdAttributeVector) 
            {
                std::vector<unsigned int> ids = uniqueIdAttributeVector->Get();
                for (size_t x = 0; x < ids.size(); x++)
                {
                    unsigned int currentId = ids[x];
                    std::map<unsigned int, unsigned int>::iterator it = LocalToGlobalUid.find(currentId);
                    if (it == LocalToGlobalUid.end())
                    {
                        ids[x] = 0;
                        continue;
                    }
                    else
                    {
                        ids[x] = it->second;
                    }
                }
                uniqueIdAttributeVector->Set(ids);
            }
        }
    }

    std::vector<IDataNode*> childNodes = node->GetNodes();
    for (unsigned int x = 0; x < childNodes.size(); x++)
        UpdateAttributeUid(childNodes[x]);
}
