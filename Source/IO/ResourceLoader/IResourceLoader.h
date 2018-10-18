#pragma once
#include <string>
#include "../../Modules/Scene/IScene.h"
#include "Common.h"

class IResourceLoader
{
public:
    virtual ~IResourceLoader() {};
    virtual bool IsLoaded() = 0;
    virtual IScene* GenerateScene() = 0;
    virtual CString GetLastError() = 0;
};
