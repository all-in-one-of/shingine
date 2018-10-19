#pragma once
#include <vector>
#include <map>

class IDataNode;

class CUniqueIdSetter
{
public:
    CUniqueIdSetter() {};
    static void SetIds(const std::vector<IDataNode*> &newNodes);
private:
    CUniqueIdSetter(const std::vector<IDataNode*> &newNodes);
    void UpdateUid(IDataNode* node);
    void UpdateAttributeUid(IDataNode* node);

    std::vector<IDataNode*> Nodes;
    std::map<unsigned int, unsigned int> LocalToGlobalUid;
};
