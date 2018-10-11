#pragma once
#include <string>
#include "../../Modules/IScene.h"

class ISceneDescription
{
public:
    virtual ~ISceneDescription() {};
    static ISceneDescription* Load(std::string fileName);
    virtual std::string GetLastError() = 0;
    virtual IScene* GenerateScene() = 0;
};
