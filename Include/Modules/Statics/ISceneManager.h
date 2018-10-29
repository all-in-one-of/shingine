#pragma once
#include <map>
#include "Statics.h"
class IScene;

class ISceneManager
{
public:
    virtual ~ISceneManager() {}
    virtual void AddScene(const String &fileName, IScene* scene) = 0;
    virtual IScene* CurrentScene() = 0;
};