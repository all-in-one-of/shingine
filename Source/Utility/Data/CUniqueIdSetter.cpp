#include "Modules/Statics/CStatics.h"
#include "Utility/Data/IDataNode.h"
#include "Utility/Data/CTypedAttribute.h"
#include "Utility/Data/CUniqueIdSetter.h"

void CUniqueIdSetter::SetIds(const std::vector<IDataNode*> &newNodes)
{
    CUniqueIdSetter a = CUniqueIdSetter(newNodes);
}
CUniqueIdSetter::CUniqueIdSetter(const std::vector<IDataNode*> &newNodes)
{
    Nodes = newNodes;
    // update ids for nodes first, then for attributes
    for (unsigned int x = 0; x < Nodes.size(); x++)
        UpdateUid(Nodes[x]);
    // update attribute uid
    for (unsigned int x = 0; x < Nodes.size(); x++)
        UpdateAttributeUid(Nodes[x]);
}

void CUniqueIdSetter::UpdateUid(IDataNode* node)
{
    unsigned int newId = CStatics::GetUniqueId();
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

        CTypedAttribute<IDataNode*>* attributeNodes = 
            dynamic_cast<CTypedAttribute<IDataNode*>*>(attributes[x]);

        if (!attributeNodes) 
            continue;
            
        std::vector<IDataNode*> attributeNodesVec = attributeNodes->Get();
        for (size_t y = 0; y < attributeNodesVec.size(); y++)
            UpdateUid(attributeNodesVec[y]);
    }
}

void CUniqueIdSetter::UpdateAttributeUid(IDataNode* node)
{
    std::vector<ISerialized*> attributes = node->GetAttributes();
    for (unsigned int x = 0; x < attributes.size(); x++)
    {   
        if (attributes[x]->TypeName() == "uid")
        {
            CAttributeUniqueId* uniqueIdAttribute = dynamic_cast<CAttributeUniqueId*>(attributes[x]);
            if (!uniqueIdAttribute) 
                continue;
            unsigned int currentId = uniqueIdAttribute->Get();
            std::map<unsigned int, unsigned int>::iterator it = LocalToGlobalUid.find(uniqueIdAttribute->Get());
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
    }

    std::vector<IDataNode*> childNodes = node->GetNodes();
    for (unsigned int x = 0; x < childNodes.size(); x++)
        UpdateAttributeUid(childNodes[x]);
}
