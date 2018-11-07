#pragma once
#include <map>
#include <unordered_map>
#include <vector>

class IDataNode;

class UniqueIdSetter {
public:
  UniqueIdSetter(){};
  UniqueIdSetter(const std::vector<IDataNode *> &newNodes);
  void UpdateAttributeUid(IDataNode *node);

  static void SetIds(const std::vector<IDataNode *> &newNodes);
  static void
  ReplaceIds(std::vector<IDataNode *> &nodes,
             std::unordered_map<unsigned int, unsigned int> &oldToNewIdPairs);

private:
  void UpdateUid(IDataNode *node);

  std::vector<IDataNode *> Nodes;
  std::map<unsigned int, unsigned int> LocalToGlobalUid;
};
