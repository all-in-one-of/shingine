#pragma once
#include "IScene.h"

class Scene : public IScene
{
public:
    Scene();
    virtual ~Scene();
    virtual void Save();
    virtual void SetSerializedData(const std::vector<IDataNode*> &nodes);
private:
    std::vector<IDataNode*> SerializedData;
};
