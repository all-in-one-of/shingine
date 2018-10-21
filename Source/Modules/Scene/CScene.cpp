#include "Modules/Scene/CScene.h"

CScene::CScene()
{
}

CScene::~CScene() 
{
}

void CScene::Save()
{
}

void CScene::SetSerializedData(const std::vector<IDataNode*> &nodes)
{
    SerializedData = nodes;
}
