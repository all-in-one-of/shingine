#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

class IMaterial
{
public:
    virtual ~IMaterial() {};
    virtual void GetFloatUniforms(std::vector<std::string> &names, std::vector<float> &values) = 0;
    virtual void GetVectorUniforms(std::vector<std::string> &names, std::vector<glm::vec4> &values) = 0;
};
