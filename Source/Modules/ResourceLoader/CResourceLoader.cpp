#include <fstream>
#include <iostream>
#include "Modules/ResourceLoader/CResourceLoader.h"
#include "Modules/ResourceLoader/CResourceReaderFactory.h"

#include "Utility/Data/IDataNode.h"
#include "Utility/Data/ISerialized.h"
#include "Utility/Data/CUniqueIdSetter.h"

#include "Engine/IObject.h"
#include "Modules/Scene/CSceneMaker.h"

#include "Modules/Statics/CStatics.h"

CString CResourceLoader::LastError = "";

CString CResourceLoader::GetLastError() { return LastError; }

bool CResourceLoader::Load(const CString &fileName)
{
    // 3 file types are supported
    // *.ssd - binary version
    // *.ssda - ascii version
    // *.ssd_json - json

    CResourceReaderFactory ResourceReaderFactory;
    IResourceReader* reader = ResourceReaderFactory.CreateReader(fileName);

    if (!reader->Open())
    {
        LastError = "Couldn't open file : " + fileName;
        delete reader;
        return false;
    }

    std::vector<ISerialized*> deserializedNodes;
    std::vector<IDataNode*> nodes;
    reader->ReadNodes(nodes);

    // traverse through each node, create unique ids
    CUniqueIdSetter::SetIds(nodes);

    for (unsigned int x = 0; x < nodes.size(); x++)
    {
        ISerialized* deserializedDataNode = nodes[x]->Deserialize();
        if (deserializedDataNode)
            deserializedNodes.push_back(deserializedDataNode);
        delete nodes[x];
    }
    reader->Close();
    delete reader;

    // Scene assets creation
    std::vector<IObject*> sceneObjects;
    std::vector<ISerialized*> assets;
    for (unsigned int x = 0; x < deserializedNodes.size(); x++)
    {
        if(deserializedNodes[x]->SerializedName() == "Object")
            sceneObjects.push_back(
                dynamic_cast<IObject*>(deserializedNodes[x]));
        else
            assets.push_back(deserializedNodes[x]);
    }

    if (sceneObjects.size() > 0)
        CStatics::SceneManager()->AddScene(fileName, CSceneMaker::Create(sceneObjects));

    for (unsigned int x = 0; x < assets.size(); x++)
    {
        ISerializedClass* assetSerializedClass = dynamic_cast<ISerializedClass*>(assets[x]);
        if (!assetSerializedClass)
        {
            std::cout << "Object isn't recognized as an asset. Attempting to delete it" << std::endl;
            delete assetSerializedClass;
            continue;
        }
        CStatics::AssetManager()->AddAsset(assetSerializedClass);
    }
    
    return true;
}
