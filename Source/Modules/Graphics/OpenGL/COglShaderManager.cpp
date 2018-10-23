#include "Modules/Graphics/OpenGL/COglShaderManager.h"
#include "Utility/Graphics.h"

#include "Modules/Graphics/CGraphics.h"

#include "Modules/Statics/CAssetManager.h"
#include "Modules/Graphics/IShader.h"

#include "Engine/AssetTypes/IMaterial.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

COglShaderManager::COglShaderManager()
{
}

void COglShaderManager::SetMaterialUniforms(IMaterial* material, int programId)
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

void COglShaderManager::GetUniformId(const std::string &uniformName, int programId, int &uniformLoc)
{
    uniformLoc = 0;
    std::unordered_map<std::string, int>::iterator it = UniformLocations[programId].find(uniformName);
    if (it == UniformLocations[programId].end())
    {
        // find uniform location
	    uniformLoc = glGetUniformLocation(programId, uniformName.c_str());
        UniformLocations[programId][uniformName] = uniformLoc;
    }
    else
        uniformLoc = it->second;
}

void COglShaderManager::SetMatrix(const std::string &uniformName, int programId, glm::mat4 matrix)
{
    int uniformLoc;
    GetUniformId(uniformName, programId, uniformLoc);
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void COglShaderManager::SetVector(const std::string &uniformName, int programId, glm::vec4 vector)
{
    int uniformLoc;
    GetUniformId(uniformName, programId, uniformLoc);
	glUniform4f(uniformLoc, vector.x, vector.y, vector.z, vector.w);
}

void COglShaderManager::SetFloat(const std::string &uniformName, int programId, float value)
{
    int uniformLoc;
    GetUniformId(uniformName, programId, uniformLoc);
	glUniform1f(uniformLoc, value);
}

int COglShaderManager::GetShaderProgramId(unsigned int shaderId)
{
    // if the asset id is 0 then assign the default shader id
    if (shaderId == 0)
        shaderId = CGraphics::DefaultShader()->AssetId();

    std::unordered_map<unsigned int, int>::iterator it = AssetIdToShaderProgramId.find(shaderId);
    if (it != AssetIdToShaderProgramId.end())
        return it->second;

    // compile shader source or raise the error
    IShader* shader = dynamic_cast<IShader*>(
        CAssetManager::Get()->GetAssetOfType("Shader", shaderId));
    
    if (shader == NULL)
        throw 1;

    int shaderProgramId = CreateVertexFragmentShaderProgram(shader);

    AssetIdToShaderProgramId[shaderId] = shaderProgramId;
    return shaderProgramId;
}

int COglShaderManager::CreateVertexFragmentShaderProgram(IShader* shader)
{   
    CString vertexShaderSource, fragmentShaderSource;
    // couldn't find the vertex and fragment shader sources
    if (!shader->GetSource(EShaderType::VERTEX, vertexShaderSource))
        throw 1;
    if (!shader->GetSource(EShaderType::FRAGMENT, fragmentShaderSource))
        throw 1;

    int vertexId = CompileShaderSource(EShaderType::VERTEX, vertexShaderSource);
    int fragmentId = CompileShaderSource(EShaderType::FRAGMENT, fragmentShaderSource);

    int programId = glCreateProgram();

    glAttachShader(programId, vertexId);
    glAttachShader(programId, fragmentId);
    glLinkProgram(programId);
    
    CString errorText;
    if(!DidProgramLink(programId, errorText))
    {
        std::cout << "Program link error: " << errorText.GetStdString() << std::endl;
        throw 1;
    }
    return programId;

}

int COglShaderManager::CompileShaderSource(int shaderType, const CString& shaderSource)
{
    int shaderTypeId = 0;
    std::string shaderTypeName = "";
    switch(EShaderType(shaderType))
    {
        case EShaderType::VERTEX:
            shaderTypeId = GL_VERTEX_SHADER; 
            shaderTypeName = "GL_VERTEX_SHADER";  
            break;
        case EShaderType::FRAGMENT:
            shaderTypeId = GL_FRAGMENT_SHADER;
            shaderTypeName = "GL_FRAGMENT_SHADER";  
            break;
        default:break;
    }

    int shaderId = glCreateShader(shaderTypeId);
    // compile shader source
    std::vector<CString> multilineSource = shaderSource.Split('\n');
    unsigned int lineCount = static_cast<unsigned int>(multilineSource.size());
    char** arraySource = new char*[lineCount];
    memset(arraySource, 0, lineCount);

    for (size_t x = 0; x < lineCount; x++)
    {
        unsigned int charCount = multilineSource[x].Length();
        arraySource[x] = new char[charCount + 2];
        memset(arraySource[x], 0, charCount + 2);

        for (unsigned int y = 0; y < charCount; y++)
            arraySource[x][y] = multilineSource[x][y];
        
        arraySource[x][charCount] = '\n';
        arraySource[x][charCount + 1] = '\0';
    }
    
    glShaderSource(shaderId, lineCount, arraySource, NULL);
    glCompileShader(shaderId);

    for (size_t x = 0; x < lineCount; x++)
    {
        delete[] arraySource[x];
    }
    delete[] arraySource;

    CString errorText;
    if(!DidShaderCompile(shaderId, errorText))
    {
        std::cout << "Shader compilation error " << shaderTypeName << " : " << errorText.GetStdString() << std::endl;
        throw 1;
    }

    return shaderId;
}

bool COglShaderManager::DidShaderCompile(unsigned int shaderId, CString& errorText)
{
    errorText = "";
	GLint success = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
		char* logText = new char[maxLength];
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, logText);
		errorText += logText;
		delete [] logText;	
		return false;	
	}
	return true; 
}

bool COglShaderManager::DidProgramLink(unsigned int programId, CString& errorText)
{
    errorText = "";
	GLint success = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if(success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);
		char* logText = new char[maxLength];
		glGetProgramInfoLog(programId, maxLength, &maxLength, logText);
		errorText += logText;
		delete [] logText;	
		return false;	
	}
	return true; 
}