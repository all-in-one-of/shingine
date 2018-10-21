#pragma once
#include "IScene.h"

class CScene : public IScene
{
public:
    CScene();
    virtual ~CScene();
    virtual void Save();
    virtual void SetSerializedData(const std::vector<IDataNode*> &nodes);
private:
    std::vector<IDataNode*> SerializedData;
};
