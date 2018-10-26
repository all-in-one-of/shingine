#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
class IShader;
class IMaterial;
class CString;
// handles OpenGL shader compilation, fetches shader program id by shader asset id
class COglShaderManager
{
public:
    COglShaderManager();
    int GetShaderProgramId(unsigned int shaderId);
    // uniforms
    void SetMaterialUniforms(IMaterial* material, int programId);

    void GetUniformId(const std::string &uniformName, int programId, int &uniformLoc);
private:
    int CreateVertexFragmentShaderProgram(IShader* shader);
    int CompileShaderSource(int shaderType, const CString& shaderSource);
    bool DidShaderCompile(unsigned int shaderId, CString& errorText);
    bool DidProgramLink(unsigned int programId, CString& errorText);


    std::unordered_map<unsigned int, int> AssetIdToShaderProgramId;
    std::unordered_map<int, std::unordered_map<std::string, int>> UniformLocations;
};
