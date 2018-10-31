#include "Modules/Graphics/OpenGL/DOglCommandBuffer.h"
#include "Modules/Graphics/OpenGL/OglCommandBuffer.h"
#include "Modules/Graphics/OpenGL/OglShaderManager.h"
#include "Modules/Graphics/OpenGL/OglTextureManager.h"
#include "Modules/Graphics/OpenGL/VaoMeshManager.h"
#include "Modules/Graphics/OpenGL/OpenGLRender.h"
#include "Modules/Graphics/OpenGL/BuiltInUniformNames.h"

#include "Modules/Statics/IGraphics.h"
#include "Modules/Statics/IActiveCamera.h"

#include "Engine/Components/TransformComponent.h"

#include "Utility/Graphics.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

OglCommandBuffer::OglCommandBuffer()
{
}

OglCommandBuffer::~OglCommandBuffer()
{
}

void OglCommandBuffer::ResetCommandBuffer()
{
    CommandCount = 0;
    Commands.clear();
}

void OglCommandBuffer::AddCommand(const unsigned char &cmd)
{
    Commands.push_back(cmd);
    CommandCount++;
}

void OglCommandBuffer::EnableDepth()
{
    AddCommand(CB_ENABLE_DEPTH);
}

void OglCommandBuffer::EnableCullFace()
{
    AddCommand(CB_ENABLE_CULL_FACE);
}

void OglCommandBuffer::Clear()
{
    AddCommand(CB_CLEAR);
}

void OglCommandBuffer::UseProgram(int programId)
{
    AddCommand(CB_USE_PROGRAM);
    WRITE_INT(programId)
}

void OglCommandBuffer::SetPolygonMode(ICommandBuffer::EDrawPolygonMode mode)
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

void OglCommandBuffer::DrawMesh(glm::mat4 &matrix, unsigned int &meshAssetId, unsigned int &shaderId)
{
    glm::mat4 matrixInv = glm::inverse(glm::transpose(matrix));
    DrawMesh(matrix, matrixInv, meshAssetId, shaderId);
}

void OglCommandBuffer::DrawMesh(glm::mat4 &matrix, glm::mat4 &matrixInv, unsigned int &meshAssetId, unsigned int &shaderId)
{
    OpenGLRender* oglRender = GetContext();

    // set material uniforms
    int programId = oglRender->GetShaderManager()->GetShaderProgramId(shaderId);

    SetMatrixOgl(ModelMatrixName, programId, matrix);
    SetMatrixOgl(ModelMatrixInverseName, programId, matrixInv);
    SetMatrixOgl(ProjectionMatrixName, programId, Statics::Get<IActiveCamera>()->ProjectionMatrix());
    SetMatrixOgl(ViewMatrixName, programId, Statics::Get<IActiveCamera>()->ViewMatrix());
    
    glm::vec4 pos;
    Statics::Get<IActiveCamera>()->GetTransformComponent()->GetPosition(pos);
    SetVectorOgl(CameraWorldPositionName, programId, pos);
    
    unsigned int vaoId, indexCount;
    oglRender->GetMeshManager()->GetVAOForMeshId(programId, meshAssetId, vaoId, indexCount);
    
    SetPolygonMode(ICommandBuffer::EDrawPolygonMode::Fill);
    
    AddCommand(CB_DRAW_MESH);
    { WRITE_INT(programId) }
    { WRITE_UINT(vaoId) }
    { WRITE_UINT(indexCount) }
}

void OglCommandBuffer::ReadValue(unsigned char &cmd)
{
    cmd = Commands[CurrentByte++];
}

OpenGLRender* OglCommandBuffer::GetContext()
{
    if (!CachedRenderContext) 
        CachedRenderContext = dynamic_cast<OpenGLRender*>(Statics::Get<IGraphics>()->GetContext());
    return CachedRenderContext;
}

void OglCommandBuffer::SetTexture(const std::string &name, unsigned int shaderId, unsigned int textureId)
{
    OpenGLRender* context = GetContext();
    OglTextureManager* textureManager = context->GetTextureManager();
    OglShaderManager* shaderManager = context->GetShaderManager();
    unsigned int textureOglId = textureManager->GetTextureIdByAssetId(textureId);
    int programId = shaderManager->GetShaderProgramId(shaderId);
    int textureSlot = 0;
    bool isNew = false;
    textureManager->GetTextureSlotForShaderProgramByName(name, programId, textureSlot, isNew);
    if (isNew)
    {
        // need to set texture slot first
        UseProgram(programId);
        int uniformLoc;
        shaderManager->GetUniformId(name, programId, uniformLoc);

        AddCommand(CB_SET_INTEGER_UNIFORM);
        WRITE_INT(uniformLoc)
        { WRITE_INT(textureSlot) }
    }
    AddCommand(CB_BIND_TEXTURE);
    { WRITE_INT(textureSlot) }
    { WRITE_INT(textureOglId) }
}

void OglCommandBuffer::SetMatrix(const std::string &name, unsigned int shaderId, glm::mat4 matrix)
{
    int programId = GetContext()->GetShaderManager()->GetShaderProgramId(shaderId);
    SetMatrixOgl(name, programId, matrix);
}

void OglCommandBuffer::SetFloat(const std::string &name, unsigned int shaderId, float value)
{
    int programId = GetContext()->GetShaderManager()->GetShaderProgramId(shaderId);
    SetFloatOgl(name, programId, value);
}

void OglCommandBuffer::SetVector(const std::string &name, unsigned int shaderId, glm::vec4 &vector)
{
    int programId = GetContext()->GetShaderManager()->GetShaderProgramId(shaderId);
    SetVectorOgl(name, programId, vector);
}

void OglCommandBuffer::SetMatrixOgl(const std::string &name, int programId, glm::mat4 matrix)
{
    int uniformLoc;
    GetContext()->GetShaderManager()->GetUniformId(name, programId, uniformLoc);
    SetMatrixOgl(uniformLoc, matrix);
}

void OglCommandBuffer::SetFloatOgl(const std::string &name, int programId, float value)
{
    int uniformLoc;
    GetContext()->GetShaderManager()->GetUniformId(name, programId, uniformLoc);
    SetFloatOgl(uniformLoc, value);
}

void OglCommandBuffer::SetVectorOgl(const std::string &name, int programId, glm::vec4 &vector)
{
    int uniformLoc;
    GetContext()->GetShaderManager()->GetUniformId(name, programId, uniformLoc);
    SetVectorOgl(uniformLoc, vector);
}

void OglCommandBuffer::SetMatrixOgl(int uniformLocation, glm::mat4 &matrix)
{
    AddCommand(CB_SET_MATRIX_UNIFORM);
    WRITE_INT(uniformLocation)
    WM(0,0) WM(0,1) WM(0,2) WM(0,3) 
    WM(1,0) WM(1,1) WM(1,2) WM(1,3) 
    WM(2,0) WM(2,1) WM(2,2) WM(2,3) 
    WM(3,0) WM(3,1) WM(3,2) WM(3,3) 
}

void OglCommandBuffer::SetFloatOgl(int uniformLocation, float value)
{
    AddCommand(CB_SET_FLOAT_UNIFORM);
    WRITE_INT(uniformLocation)
    { WRITE_FLOAT(value) }
}

void OglCommandBuffer::SetVectorOgl(int uniformLocation, glm::vec4 &vector)
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
void OglCommandBuffer::Execute()
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
            case CB_SET_INTEGER_UNIFORM:
            {
                int uniformLoc;
                int intValue;
                { READ_INT(uniformLoc) }
                { READ_INT(intValue) }
                glUniform1i(uniformLoc, intValue);
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
            case CB_BIND_TEXTURE:
            {
                int textureSlot, textureId;
                { READ_INT(textureSlot) }
                { READ_INT(textureId) }
                glActiveTexture(GL_TEXTURE0 + textureSlot);
                glBindTexture(GL_TEXTURE_2D, textureId);
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
