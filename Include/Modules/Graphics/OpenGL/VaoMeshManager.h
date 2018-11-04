#pragma once
#include <unordered_map>
// finds vao by ogl shader program id and mesh asset id
class VaoMeshManager {
public:
  VaoMeshManager();
  void GetVAOForMeshId(int programId, unsigned int meshAssetId,
                       unsigned int &vaoId, unsigned int &indexCount);
  void DeleteUnusedResources();
private:
  // [program id -> [mesh id -> vao id]]
  std::unordered_map<int, std::unordered_map<unsigned int, unsigned int>>
      VaoMap;
  std::unordered_map<unsigned int, unsigned int> IndexCountMap;
};
