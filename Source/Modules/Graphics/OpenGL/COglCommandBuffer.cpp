#include "Modules/Graphics/CGraphics.h"
#include "Utility/Graphics.h"

#include "Modules/Graphics/OpenGL/DOglCommandBuffer.h"
#include "Modules/Graphics/OpenGL/COglCommandBuffer.h"
#include "Modules/Graphics/OpenGL/COglShaderManager.h"
#include "Modules/Graphics/OpenGL/CVaoMeshManager.h"
#include "Modules/Graphics/OpenGL/COpenGLRender.h"

#include <glm/gtc/type_ptr.hpp>
#include "Modules/Statics/CAssetManager.h"
#include "Engine/AssetTypes/CMaterial.h"
#include "Modules/Statics/CActiveCamera.h"

#include <iostream>


COglCommandBuffer::COglCommandBuffer()
{
}

COglCommandBuffer::~COglCommandBuffer()
{
}

void COglCommandBuffer::ResetCommandBuffer()
{
    CommandCount = 0;
    Commands.clear();
}

void COglCommandBuffer::AddCommand(const unsigned char &cmd)
{
    Commands.push_back(cmd);
    CommandCount++;
}

void COglCommandBuffer::EnableDepth()
{
    AddCommand(CB_ENABLE_DEPTH);
}

void COglCommandBuffer::EnableCullFace()
{
    AddCommand(CB_ENABLE_CULL_FACE);
}

void COglCommandBuffer::Clear()
{
    AddCommand(CB_CLEAR);
}

void COglCommandBuffer::UseProgram(int programId)
{
    AddCommand(CB_USE_PROGRAM);
    WRITE_INT(programId)
}

void COglCommandBuffer::SetPolygonMode(ICommandBuffer::EDrawPolygonMode mode)
{
    AddCommand(CB_SET_POLYGON_MODE);
    switch (mode)
    {
        case ICommandBuffer::EDrawPolygonMode::Fill:
            Commands.push_back(CB_POLY_FILL & 0xff);
            break;
        case ICommandBuffer::EDrawPolygonMode::Line:
            Commands.push_back(CB_POLY_LINE & 0xff);
            break;
        case ICommandBuffer::EDrawPolygonMode::Point:
        default:
            Commands.push_back(CB_POLY_POINT & 0xff);
            break;
    }
}

void COglCommandBuffer::DrawMesh(glm::mat4 &matrix, unsigned int &meshAssetId, unsigned int &materialId)
{
    glm::mat4 matrixInv = glm::inverse(glm::transpose(matrix));
    DrawMesh(matrix, matrixInv, meshAssetId, materialId);
}

#define PositionAttributeName "_PositionAttribute"
#define NormalAttributeName "_NormalAttribute"
#define TexCoordAttributeName "_TexCoordAttribute"

#define ModelMatrixName "_ModelMatrix"
#define ModelMatrixInverseName "_ModelMatrixInverseTransposed"
#define ViewMatrixName "_ViewMatrix"
#define ProjectionMatrixName "_ProjectionMatrix"

void COglCommandBuffer::DrawMesh(glm::mat4 &matrix, glm::mat4 &matrixInv, unsigned int &meshAssetId, unsigned int &materialId)
{
    COpenGLRender* oglRender = dynamic_cast<COpenGLRender*>(CGraphics::GetContext());

    std::unordered_map<std::string, 
        std::unordered_map<unsigned int, ISerializedClass*>>::iterator MaterialIterator;

    CAssetManager::Get()->GetAssetIteratorOfType("Material", MaterialIterator);
    CMaterial* material;
    if (materialId == 0)
        materialId = MaterialIterator->second.begin()->first;

    material = dynamic_cast<CMaterial*>(MaterialIterator->second.at(materialId));
    // set material uniforms
    int programId = oglRender->GetShaderManager()->GetShaderProgramId(material->ShaderId);

    SetMatrix(ModelMatrixName, programId, matrix);
    SetMatrix(ModelMatrixInverseName, programId, matrixInv);
   
    SetMatrix(ProjectionMatrixName, programId, CActiveCamera::ProjectionMatrix());
    SetMatrix(ViewMatrixName, programId, CActiveCamera::ViewMatrix());

    // set material uniforms
    {
        // float uniforms
        std::vector<std::string> names;
        std::vector<float> floatValues;
        material->GetFloatUniforms(names, floatValues);
        for (size_t x = 0; x < names.size(); x++)
            SetFloat(names[x], programId, floatValues[x]);
        // vector uniforms
        std::vector<glm::vec4> vectorValues;
        material->GetVectorUniforms(names, vectorValues);
        for (size_t x = 0; x < names.size(); x++)
            SetVector(names[x], programId, vectorValues[x]);
        // TODO add matrices
    }

    unsigned int vaoId, indexCount;
    oglRender->GetMeshManager()->GetVAOForMeshId(programId, meshAssetId, vaoId, indexCount);
    
    SetPolygonMode(ICommandBuffer::EDrawPolygonMode::Fill);
    
    AddCommand(CB_DRAW_MESH);
    { WRITE_INT(programId) }
    { WRITE_UINT(vaoId) }
    { WRITE_UINT(indexCount) }
    
}

void COglCommandBuffer::ReadValue(unsigned char &cmd)
{
    cmd = Commands[CurrentByte++];
}

void COglCommandBuffer::SetMatrix(const std::string &name, int programId, glm::mat4 matrix)
{
    int uniformLoc;
    COpenGLRender* oglRender = dynamic_cast<COpenGLRender*>(CGraphics::GetContext());
    oglRender->GetShaderManager()->GetUniformId(name, programId, uniformLoc);
    SetMatrix(uniformLoc, matrix);
}

void COglCommandBuffer::SetFloat(const std::string &name, int programId, float value)
{
    int uniformLoc;
    COpenGLRender* oglRender = dynamic_cast<COpenGLRender*>(CGraphics::GetContext());
    oglRender->GetShaderManager()->GetUniformId(name, programId, uniformLoc);
    SetFloat(uniformLoc, value);
}

void COglCommandBuffer::SetVector(const std::string &name, int programId, glm::vec4 &vector)
{
    int uniformLoc;
    COpenGLRender* oglRender = dynamic_cast<COpenGLRender*>(CGraphics::GetContext());
    oglRender->GetShaderManager()->GetUniformId(name, programId, uniformLoc);
    SetVector(uniformLoc, vector);
}

void COglCommandBuffer::SetMatrix(int uniformLocation, glm::mat4 &matrix)
{
    AddCommand(CB_SET_MATRIX_UNIFORM);
    WRITE_INT(uniformLocation)
    WM(0,0) WM(0,1) WM(0,2) WM(0,3) 
    WM(1,0) WM(1,1) WM(1,2) WM(1,3) 
    WM(2,0) WM(2,1) WM(2,2) WM(2,3) 
    WM(3,0) WM(3,1) WM(3,2) WM(3,3) 
}

void COglCommandBuffer::SetFloat(int uniformLocation, float value)
{
    AddCommand(CB_SET_FLOAT_UNIFORM);
    WRITE_INT(uniformLocation)
    { WRITE_FLOAT(value) }
}

void COglCommandBuffer::SetVector(int uniformLocation, glm::vec4 &vector)
{
    AddCommand(CB_SET_VECTOR_UNIFORM);
    WRITE_INT(uniformLocation)
    { WRITE_FLOAT(vector.x) }
    { WRITE_FLOAT(vector.y) }
    { WRITE_FLOAT(vector.z) }
    { WRITE_FLOAT(vector.w) }
}

// set matrix uniform command
// set...
void COglCommandBuffer::Execute()
{
    CurrentByte = 0;
    unsigned char cmd;
    for (unsigned int x = 0; x < CommandCount; x++)
    {
        ReadValue(cmd);
        switch(cmd)
        {
            case CB_ENABLE_DEPTH: 
            {
                glEnable(GL_DEPTH);
                glEnable(GL_DEPTH_TEST);
            }
            break;
            case CB_ENABLE_CULL_FACE: 
            {
                glEnable(GL_CULL_FACE);
            }
            break;
            case CB_CLEAR:
            {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }
            break;
            case CB_SET_POLYGON_MODE: 
            {
                ReadValue(cmd);
                switch(cmd)
                {
                    case CB_POLY_POINT:
                        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                        break;
                    case CB_POLY_FILL:
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        break;
                    case CB_POLY_LINE:
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        break;
                }
            }
            break;
            case CB_USE_PROGRAM:
            {
                int programId;
                { READ_INT(programId) }
                glUseProgram(programId);
            }
            break;
            case CB_SET_FLOAT_UNIFORM:
            {
                int uniformLocFloat;
                float floatValue;
                { READ_INT(uniformLocFloat) }
                { READ_FLOAT(floatValue) }
                glUniform1f(uniformLocFloat, floatValue);
            }
            break;
            case CB_SET_VECTOR_UNIFORM:
            {
                int uniformLocInt;
                glm::vec4 vectorValue;
                { READ_INT(uniformLocInt) }
                { READ_FLOAT(vectorValue.x) }
                { READ_FLOAT(vectorValue.y) }
                { READ_FLOAT(vectorValue.z) }
                { READ_FLOAT(vectorValue.w) }
	            glUniform4f(uniformLocInt, 
                    vectorValue.x, vectorValue.y, vectorValue.z, vectorValue.w);
            }
            break;
            case CB_SET_MATRIX_UNIFORM:
            {
                int uniformLocMat;
                glm::mat4 matrix;
                { READ_INT(uniformLocMat) }
                RM(0,0) RM(0,1) RM(0,2) RM(0,3) 
                RM(1,0) RM(1,1) RM(1,2) RM(1,3) 
                RM(2,0) RM(2,1) RM(2,2) RM(2,3) 
                RM(3,0) RM(3,1) RM(3,2) RM(3,3)            
                glUniformMatrix4fv(uniformLocMat,
                   1, GL_FALSE, glm::value_ptr(matrix));
            }
            break;
            case CB_DRAW_MESH:
            {
                unsigned int vaoId, indexCount;
                int programId;
                { READ_INT(programId) }
                { READ_UINT(vaoId) }
                { READ_UINT(indexCount) }
                glUseProgram(programId);
                glBindVertexArray(vaoId);
                glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
            break;
            default: break;
        }
    }
}
