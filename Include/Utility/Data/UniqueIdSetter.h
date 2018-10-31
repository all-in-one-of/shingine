#pragma once
#include <map>
#include <vector>

class IDataNode;

class UniqueIdSetter {
public:
  UniqueIdSetter(){};
  static void SetIds(const std::vector<IDataNode *> &newNodes);

private:
  UniqueIdSetter(const std::vector<IDataNode *> &newNodes);
  void UpdateUid(IDataNode *node);
  void UpdateAttributeUid(IDataNode *node);

  std::vector<IDataNode *> Nodes;
  std::map<unsigned int, unsigned int> LocalToGlobalUid;
};
