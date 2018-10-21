#include <fstream>
#include <iostream>
#include "Modules/ResourceLoader/CResourceLoader.h"
#include "Modules/ResourceLoader/CResourceReaderFactory.h"

#include "Utility/Data/IDataNode.h"
#include "Utility/Data/ISerialized.h"
#include "Utility/Data/CUniqueIdSetter.h"
#include "Utility/Data/CEntityComponentIdSetter.h"
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

    for (size_t x = 0; x < nodes.size(); x++)
    {
        ISerialized* deserializedDataNode = nodes[x]->Deserialize();
        if (deserializedDataNode)
            deserializedNodes.push_back(deserializedDataNode);
    }
    reader->Close();
    delete reader;

    std::vector<ISerialized*> entities;
    CInstanceManager* instanceManager = CStatics::InstanceManager();

    for (unsigned int x = 0; x < deserializedNodes.size(); x++)
    {
        if(deserializedNodes[x]->SerializedName() == "Entity")
        {
            entities.push_back(deserializedNodes[x]);
            instanceManager->Destroy(dynamic_cast<ISerializedClass*>(deserializedNodes[x]));
        }

        else if(deserializedNodes[x]->SerializedName() == "EntityIdCollection")
        {
            CEntityComponentIdSetter::UpdateIds(deserializedNodes[x]);
            instanceManager->Destroy(dynamic_cast<ISerializedClass*>(deserializedNodes[x]));
        }
    }
    
    // check if the ssd file contains entities
    if (entities.size() > 0)
        CStatics::SceneManager()->AddScene(fileName, CSceneMaker::Create(nodes));
    else
        for (size_t x = 0; x < nodes.size(); x++)
            delete nodes[x];
    return true;
}
