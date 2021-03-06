#include "Utility/Data/UniqueIdSetter.h"
#include "Modules/Statics/Statics.h"
#include "Utility/Data/IDataNode.h"
#include "Utility/Data/TypedAttribute.h"

void UniqueIdSetter::SetIds(const std::vector<IDataNode *> &newNodes) {
  (void)UniqueIdSetter(newNodes);
}

void ReplaceUid(IDataNode *node, unsigned int oldId, unsigned int newId) {
  if (node->GetUniqueID() == oldId)
    node->SetUniqueID(newId);
  std::vector<IDataNode *> nodes = node->GetNodes();
  for (unsigned int x = 0; x < nodes.size(); x++)
    ReplaceUid(nodes[x], oldId, newId);

  std::vector<ISerialized *> attributes = node->GetAttributes();
  for (unsigned int x = 0; x < attributes.size(); x++) {
    if ((attributes[x]->TypeName() == "uid") == false)
      continue;
    TypedAttributeValue<unsigned int> *uniqueIdAttribute =
        dynamic_cast<TypedAttributeValue<unsigned int> *>(attributes[x]);
    TypedAttribute<unsigned int> *uniqueIdAttributeVector =
        dynamic_cast<TypedAttribute<unsigned int> *>(attributes[x]);
    if (uniqueIdAttribute) {
      if (uniqueIdAttribute->Get() == oldId)
        uniqueIdAttribute->Set(newId);
    } else if (uniqueIdAttributeVector) {
      std::vector<unsigned int> ids = uniqueIdAttributeVector->Get();
      for (unsigned int y = 0; y < ids.size(); y++) {
        if (ids[y] == oldId)
          ids[y] = newId;
      }
      uniqueIdAttributeVector->Set(ids);
    }
  }
}

void UniqueIdSetter::ReplaceIds(
    std::vector<IDataNode *> &nodes,
    std::unordered_map<unsigned int, unsigned int> &oldToNewIdPairs) {
  std::unordered_map<unsigned int, unsigned int>::iterator pairIterator;
  for (pairIterator = oldToNewIdPairs.begin();
       pairIterator != oldToNewIdPairs.end(); pairIterator++) {
    unsigned int oldId = pairIterator->first;
    unsigned int newId = pairIterator->second;
    for (unsigned int x = 0; x < nodes.size(); x++) {
      ReplaceUid(nodes[x], oldId, newId);
    }
  }
}

UniqueIdSetter::UniqueIdSetter(const std::vector<IDataNode *> &newNodes) {
  Nodes = newNodes;
  // update ids for nodes first, then for attributes
  for (unsigned int x = 0; x < Nodes.size(); x++)
    UpdateUid(Nodes[x]);
  // update attribute uid
  for (unsigned int x = 0; x < Nodes.size(); x++)
    UpdateAttributeUid(Nodes[x]);
}

void UniqueIdSetter::UpdateUid(IDataNode *node) {
  unsigned int newId = Statics::GetUniqueId();
  LocalToGlobalUid[node->GetUniqueID()] = newId;
  node->SetUniqueID(newId);

  std::vector<IDataNode *> childNodes = node->GetNodes();
  for (unsigned int x = 0; x < childNodes.size(); x++)
    UpdateUid(childNodes[x]);

  std::vector<ISerialized *> attributes = node->GetAttributes();
  for (size_t x = 0; x < attributes.size(); x++) {
    if ((attributes[x]->TypeName() == "SerializedClass") == false)
      continue;

    TypedAttribute<IDataNode *> *attributeNodes =
        dynamic_cast<TypedAttribute<IDataNode *> *>(attributes[x]);

    if (!attributeNodes)
      continue;

    std::vector<IDataNode *> attributeNodesVec = attributeNodes->Get();
    for (size_t y = 0; y < attributeNodesVec.size(); y++)
      UpdateUid(attributeNodesVec[y]);
  }
}

void UniqueIdSetter::UpdateAttributeUid(IDataNode *node) {
  std::vector<ISerialized *> attributes = node->GetAttributes();
  for (unsigned int x = 0; x < attributes.size(); x++) {
    if (attributes[x]->TypeName() == "SerializedClass") {

      TypedAttribute<IDataNode *> *attributeNodes =
          dynamic_cast<TypedAttribute<IDataNode *> *>(attributes[x]);

      if (!attributeNodes)
        continue;

      std::vector<IDataNode *> attributeNodesVec = attributeNodes->Get();
      for (size_t y = 0; y < attributeNodesVec.size(); y++)
        UpdateAttributeUid(attributeNodesVec[y]);
    }

    if (attributes[x]->TypeName() == "uid") {
      // it's a value attribute
      TypedAttributeValue<unsigned int> *uniqueIdAttribute =
          dynamic_cast<TypedAttributeValue<unsigned int> *>(attributes[x]);
      if (uniqueIdAttribute) {
        unsigned int currentId = uniqueIdAttribute->Get();
        std::map<unsigned int, unsigned int>::iterator it =
            LocalToGlobalUid.find(currentId);
        if (it == LocalToGlobalUid.end()) {
          unsigned int id = 0;
          uniqueIdAttribute->Set(id);
          continue;
        } else {
          uniqueIdAttribute->Set(it->second);
        }
      }
      // it's a vector attribute
      TypedAttribute<unsigned int> *uniqueIdAttributeVector =
          dynamic_cast<TypedAttribute<unsigned int> *>(attributes[x]);
      if (uniqueIdAttributeVector) {
        std::vector<unsigned int> ids = uniqueIdAttributeVector->Get();
        for (size_t x = 0; x < ids.size(); x++) {
          unsigned int currentId = ids[x];
          std::map<unsigned int, unsigned int>::iterator it =
              LocalToGlobalUid.find(currentId);
          if (it == LocalToGlobalUid.end()) {
            ids[x] = 0;
            continue;
          } else {
            ids[x] = it->second;
          }
        }
        uniqueIdAttributeVector->Set(ids);
      }
    }
  }

  std::vector<IDataNode *> childNodes = node->GetNodes();
  for (unsigned int x = 0; x < childNodes.size(); x++)
    UpdateAttributeUid(childNodes[x]);
}
