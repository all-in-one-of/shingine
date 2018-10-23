#include "Modules/Statics/CAssetManager.h"
#include "Utility/Data/ISerialized.h"
#include "Utility/Data/CSerializedFactory.h"

CAssetManager* CAssetManager::Instance = NULL;
ISerializedClass* CAssetManager::GetAssetOfType(const CString &typeName, unsigned int assetId)
{
    StringMap::iterator it;
    if (!GetAssetIteratorOfType(typeName, it))
        return NULL;

    IdMap &idMap = it->second;
    if (idMap.size() == 0)
        return NULL;

    IdMap::iterator idIterator = idMap.begin();
    if (assetId == 0) return idIterator->second;

    idIterator = idMap.find(assetId);
    if (idIterator == idMap.end()) 
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

bool CAssetManager::GetAssetIteratorOfType(const CString &typeName, StringMap::iterator &iterator)
{
    StringMap::iterator it = Assets.find(typeName.GetStdString());
    if (it == Assets.end()) 
        return false;
    iterator = it;
    return true;
}
