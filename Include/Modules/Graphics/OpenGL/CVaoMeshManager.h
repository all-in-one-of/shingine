#pragma once
#include <unordered_map>
// finds vao by ogl shader program id and mesh asset id
class CVaoMeshManager
{
public:
    CVaoMeshManager();
    void GetVAOForMeshId(int programId, unsigned int meshAssetId, unsigned int &vaoId, unsigned int &indexCount);
private:
    std::unordered_map<int, std::unordered_map<unsigned int, unsigned int>> VaoMap;
    std::unordered_map<unsigned int, unsigned int> IndexCountMap;
};
