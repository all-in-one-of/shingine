#include "Modules/Statics/CAssetManager.h"
#include "Utility/Data/ISerialized.h"
#include "Utility/Data/CSerializedFactory.h"

CAssetManager* CAssetManager::Instance = NULL;
ISerializedClass* CAssetManager::GetAssetOfType(const CString &typeName, unsigned int assetId)
{
    StringMap::iterator it = Assets.find(typeName.GetStdString());
    if (it == Assets.end())
        return NULL;
    if (it->second.begin() == it->second.end())
        return NULL;
    if (assetId == 0)
        return it->second.begin()->second;

    IdMap::iterator idIterator = it->second.find(assetId);
    if (idIterator == it->second.end()) 
        return NULL;
    return idIterator->second;
}


ISerializedClass* CAssetManager::AddAssetOfType(const CString &typeName)
{
    ISerializedClass* newAsset = dynamic_cast<ISerializedClass*>(CSerializedFactory::CreateInstance(typeName.GetStdString()));
    AddInstance(newAsset);
    return newAsset;
}

void CAssetManager::AddInstance(ISerializedClass* newAsset)
{
    Assets[newAsset->SerializedName().GetStdString()][newAsset->UniqueID()] = newAsset;
}

void CAssetManager::RemoveAssetType(CString assetType)
{
    Assets.erase(assetType.GetStdString());
}

void CAssetManager::GetAssetIteratorOfType(const CString &typeName, StringMap::iterator &iterator)
{
    StringMap::iterator it = Assets.find(typeName.GetStdString());
    if (it == Assets.end()) 
        return;
    iterator = it;
}
