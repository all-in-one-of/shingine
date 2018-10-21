#include "Modules/Statics/CAssetManager.h"
#include "Utility/Data/ISerialized.h"

CAssetManager* CAssetManager::Instance = NULL;
ISerializedClass* CAssetManager::GetAssetOfType(CString typeName, unsigned int assetId)
{
    auto it = Assets.find(typeName.GetStdString());
    if (it == Assets.end())
        return NULL;
    if (it->second.begin() == it->second.end())
        return NULL;
    if (assetId == 0)
        return it->second.begin()->second;

    auto it2 = it->second.find(assetId);
    if (it2 == it->second.end()) 
        return NULL;
    return it2->second;
}

void CAssetManager::AddInstance(ISerializedClass* newAsset)
{
    Assets[newAsset->SerializedName().GetStdString()][newAsset->UniqueID()] = newAsset;
}

void CAssetManager::RemoveAssetType(CString assetType)
{
    Assets.erase(assetType.GetStdString());
}
