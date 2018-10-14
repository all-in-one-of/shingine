#pragma once
#include <string>
#include "../../Modules/Scene/IScene.h"
#include "Common.h"

class ISceneDescription
{
public:
    virtual ~ISceneDescription() {};
    virtual bool IsLoaded() = 0;
    virtual IScene* GenerateScene() = 0;
    virtual CString GetLastError() = 0;
};
