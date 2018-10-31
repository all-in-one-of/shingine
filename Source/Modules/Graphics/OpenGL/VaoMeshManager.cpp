#include "Modules/Graphics/OpenGL/VaoMeshManager.h"
#include "Modules/Graphics/OpenGL/BuiltInUniformNames.h"
#include "Utility/Graphics.h"
#include "Modules/Statics/IAssetManager.h"
#include "Engine/AssetTypes/Mesh.h"

#include "Modules/Graphics/OpenGL/OpenGLRender.h"
#include <iostream>

VaoMeshManager::VaoMeshManager()
{
}

struct Vertex_xyz_nxnynz_txtytz
{
    float x,y,z,nx,ny,nz,tx,ty,tz;
};

void VaoMeshManager::GetVAOForMeshId(int programId, unsigned int meshAssetId, unsigned int &vaoId, unsigned int &indexCount)
{
    // nested map
    // program_id -> map -> mesh_asset_id 
    std::unordered_map<unsigned int, unsigned int>::iterator it = VaoMap[programId].find(meshAssetId);
    if (it == VaoMap[programId].end())
    {
        // get mesh
        Mesh* mesh = Statics::Get<IAssetManager>()->GetAssetOfType<Mesh>(meshAssetId);
        if (!mesh) 
            throw 1;

        std::cout << "Loading Mesh : " << meshAssetId << std::endl;

        // load mesh data to gpu
        glGenVertexArrays(1, &(vaoId));
        glBindVertexArray(vaoId);

        unsigned int vertexBufferId;
        glGenBuffers(1, &(vertexBufferId));	
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

        unsigned int vertexCount = static_cast<unsigned int>(mesh->Positions.size() / 3);

	    unsigned int vertexBufferSizeInBytes = 
            sizeof(Vertex_xyz_nxnynz_txtytz) * vertexCount;

        Vertex_xyz_nxnynz_txtytz* vboVertices = new Vertex_xyz_nxnynz_txtytz[vertexCount];


        for (unsigned int x = 0; x < vertexCount; x++)
        {
            unsigned int vertexIndex = x * 3;
            vboVertices[x].x = mesh->Positions[vertexIndex + 0];
            vboVertices[x].y = mesh->Positions[vertexIndex + 1];
            vboVertices[x].z = mesh->Positions[vertexIndex + 2];

            vboVertices[x].nx = mesh->Normals[vertexIndex + 0];
            vboVertices[x].ny = mesh->Normals[vertexIndex + 1];
            vboVertices[x].nz = mesh->Normals[vertexIndex + 2];

            vboVertices[x].tx = mesh->TexCoord[vertexIndex + 0];
            vboVertices[x].ty = mesh->TexCoord[vertexIndex + 1];
            vboVertices[x].tz = mesh->TexCoord[vertexIndex + 2];
        }

        // static draw
		glBufferData(GL_ARRAY_BUFFER, vertexBufferSizeInBytes, vboVertices, GL_STATIC_DRAW);
            
        unsigned int indexBufferId = 0;
        unsigned int* indicesPointer = &mesh->Indices[0];
        indexCount = static_cast<unsigned int>(mesh->Indices.size());

        glGenBuffers(1, &(indexBufferId) );			// vertex_buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, indicesPointer, GL_STATIC_DRAW);

        GLint posLocation = glGetAttribLocation(programId, PositionAttributeName);
        GLint normalLocation = glGetAttribLocation(programId, NormalAttributeName);
        GLint texcoordLocation = glGetAttribLocation(programId, TexCoordAttributeName);

        glEnableVertexAttribArray(posLocation);
        glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_xyz_nxnynz_txtytz), (void*)offsetof(Vertex_xyz_nxnynz_txtytz, x));
        glEnableVertexAttribArray(normalLocation);
        glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_xyz_nxnynz_txtytz), (void*)offsetof(Vertex_xyz_nxnynz_txtytz, nx));
        glEnableVertexAttribArray(texcoordLocation);
        glVertexAttribPointer(texcoordLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_xyz_nxnynz_txtytz), (void*)offsetof(Vertex_xyz_nxnynz_txtytz, tx));

        glBindVertexArray( 0 );

        glDisableVertexAttribArray(posLocation);
        glDisableVertexAttribArray(normalLocation);
        glDisableVertexAttribArray(texcoordLocation);

        glBindBuffer(GL_ARRAY_BUFFER, 0 );
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
