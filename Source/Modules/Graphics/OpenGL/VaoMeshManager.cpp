#include "Modules/Graphics/OpenGL/VaoMeshManager.h"
#include "Engine/AssetTypes/Mesh.h"
#include "Modules/Graphics/OpenGL/BuiltInUniformNames.h"
#include "Modules/Statics/IAssetManager.h"
#include "Utility/Graphics.h"

#include "Modules/Graphics/OpenGL/OpenGLRender.h"
#include <iostream>

VaoMeshManager::VaoMeshManager() {}

void VaoMeshManager::DeleteUnusedResources() {
  typedef std::unordered_map<unsigned int, unsigned int> IntMapType;
  typedef std::unordered_map<int, IntMapType> VaoMapType;

  VaoMapType::iterator programIdIterator;
  IntMapType::iterator uintMapIterator;

  IntMapType VaoIdToMeshId;
  for (programIdIterator = VaoMap.begin(); programIdIterator != VaoMap.end();
       programIdIterator++) {
    IntMapType &map = programIdIterator->second;
    for (uintMapIterator = map.begin(); uintMapIterator != map.end();
         uintMapIterator++)
      VaoIdToMeshId[uintMapIterator->second] = uintMapIterator->first;
  }

  std::vector<unsigned int> meshesToRemove;
  IAssetManager *assetManager = Statics::Get<IAssetManager>();
  for (uintMapIterator = VaoIdToMeshId.begin();
       uintMapIterator != VaoIdToMeshId.end(); uintMapIterator++) {
    unsigned int meshId = uintMapIterator->second;
    Mesh *mesh = assetManager->GetAssetOfType<Mesh>(meshId);
    if (mesh)
      continue;
    // mesh was deleted
    meshesToRemove.push_back(uintMapIterator->second);
    std::vector<int> vboToDelete;
    unsigned vaoId = uintMapIterator->first;
    int attribCount = 0;
    
    std::unordered_map<unsigned int, unsigned int>::iterator
        indexCountIterator = IndexCountMap.find(vaoId);
    if (indexCountIterator != IndexCountMap.end())
      IndexCountMap.erase(indexCountIterator);
      
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attribCount);
    glBindVertexArray(vaoId);
    for (int currentAttrib = 0; currentAttrib < attribCount; currentAttrib++) {
      int vboId = 0;
      glGetVertexAttribiv(currentAttrib, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                          &vboId);
      if (vboId > 0)
        vboToDelete.push_back(vboId);
    }
    glDeleteVertexArrays(1, &vaoId);
    for (unsigned int x = 0; x < vboToDelete.size(); x++) {
      unsigned int vboId = vboToDelete[x];
      glDeleteBuffers(1, &vboId);
    }
  }
  // delete records of the mesh asset
  for (programIdIterator = VaoMap.begin(); programIdIterator != VaoMap.end();
       programIdIterator++) {
    IntMapType &map = programIdIterator->second;
    for (unsigned int x = 0; x < meshesToRemove.size(); x++) {
      uintMapIterator = map.find(meshesToRemove[x]);
      if (uintMapIterator != map.end())
        map.erase(uintMapIterator);
    }
  }
}

struct Vertex_xyz_nxnynz_txtytz {
  float x, y, z, nx, ny, nz, tx, ty, tz;
};

void VaoMeshManager::GetVAOForMeshId(int programId, unsigned int meshAssetId,
                                     unsigned int &vaoId,
                                     unsigned int &indexCount) {
  // nested map
  // program_id -> map -> mesh_asset_id
  std::unordered_map<unsigned int, unsigned int>::iterator it =
      VaoMap[programId].find(meshAssetId);
  if (it == VaoMap[programId].end()) {
    // get mesh
    Mesh *mesh =
        Statics::Get<IAssetManager>()->GetAssetOfType<Mesh>(meshAssetId);
    if (!mesh)
      throw 1;

    // load mesh data to gpu
    glGenVertexArrays(1, &(vaoId));
    glBindVertexArray(vaoId);

    unsigned int vertexBufferId;
    glGenBuffers(1, &(vertexBufferId));
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

    unsigned int vertexCount =
        static_cast<unsigned int>(mesh->Positions.size());

    unsigned int vertexBufferSizeInBytes =
        sizeof(Vertex_xyz_nxnynz_txtytz) * vertexCount;

    Vertex_xyz_nxnynz_txtytz *vboVertices =
        new Vertex_xyz_nxnynz_txtytz[vertexCount];

    for (unsigned int x = 0; x < vertexCount; x++) {
      vboVertices[x].x = mesh->Positions[x].x;
      vboVertices[x].y = mesh->Positions[x].y;
      vboVertices[x].z = mesh->Positions[x].z;

      vboVertices[x].nx = mesh->Normals[x].x;
      vboVertices[x].ny = mesh->Normals[x].y;
      vboVertices[x].nz = mesh->Normals[x].z;

      vboVertices[x].tx = mesh->TexCoord[x].x;
      vboVertices[x].ty = mesh->TexCoord[x].y;
      vboVertices[x].tz = mesh->TexCoord[x].z;
    }

    // static draw
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSizeInBytes, vboVertices,
                 GL_STATIC_DRAW);

    unsigned int indexBufferId = 0;
    unsigned int *indicesPointer = &mesh->Indices[0];
    indexCount = static_cast<unsigned int>(mesh->Indices.size());

    glGenBuffers(1, &(indexBufferId)); // vertex_buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount,
                 indicesPointer, GL_STATIC_DRAW);

    GLint posLocation = glGetAttribLocation(programId, PositionAttributeName);
    GLint normalLocation = glGetAttribLocation(programId, NormalAttributeName);
    GLint texcoordLocation =
        glGetAttribLocation(programId, TexCoordAttributeName);

    glEnableVertexAttribArray(posLocation);
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex_xyz_nxnynz_txtytz),
                          (void *)offsetof(Vertex_xyz_nxnynz_txtytz, x));
    glEnableVertexAttribArray(normalLocation);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex_xyz_nxnynz_txtytz),
                          (void *)offsetof(Vertex_xyz_nxnynz_txtytz, nx));
    glEnableVertexAttribArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex_xyz_nxnynz_txtytz),
                          (void *)offsetof(Vertex_xyz_nxnynz_txtytz, tx));

    glBindVertexArray(0);

    glDisableVertexAttribArray(posLocation);
    glDisableVertexAttribArray(normalLocation);
    glDisableVertexAttribArray(texcoordLocation);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    VaoMap[programId][meshAssetId] = vaoId;
    IndexCountMap[vaoId] = indexCount;

    delete[] vboVertices;
    return;
  }

  vaoId = it->second;
  indexCount = IndexCountMap[vaoId];
  return;
}
