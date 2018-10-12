#pragma once
#include <string>
#include "../../Modules/IScene.h"
#include "Common.h"

class ISceneDescription
{
public:
    virtual ~ISceneDescription() {};
    virtual IScene* GenerateScene() = 0;
    virtual CString GetLastError() = 0;
};
