#pragma once
#include <map>
#include "ISceneManager.h"
#include "Utility/Data/Serialization.h"

class SceneManager : public ISceneManager, public ISerializedClass
{
public:
    SERIALIZE_CLASS(SceneManager)
    SceneManager();
    virtual ~SceneManager() {}
    virtual void AddScene(const String &fileName, IScene* scene);
    virtual IScene* CurrentScene();
private:
    std::map<String, IScene*> Scenes;
    IScene* CurrentSceneInstance = nullptr;
};
