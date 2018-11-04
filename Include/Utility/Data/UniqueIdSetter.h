#pragma once
#include <map>
#include <unordered_map>
#include <vector>

class IDataNode;

class UniqueIdSetter {
public:
  UniqueIdSetter(){};
  static void SetIds(const std::vector<IDataNode *> &newNodes);
  static void
  ReplaceIds(std::vector<IDataNode *> &nodes,
             std::unordered_map<unsigned int, unsigned int> &oldToNewIdPairs);

private:
  UniqueIdSetter(const std::vector<IDataNode *> &newNodes);
  void UpdateUid(IDataNode *node);
  void UpdateAttributeUid(IDataNode *node);

  std::vector<IDataNode *> Nodes;
  std::map<unsigned int, unsigned int> LocalToGlobalUid;
};
