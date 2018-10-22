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
#include "Modules/Statics/CSceneManager.h"
#include "Modules/Statics/CAssetManager.h"

CString CResourceLoader::LastError = "";
CString CResourceLoader::GetLastError() { return LastError; }

bool CResourceLoader::LoadText(const CString &fileName, CString &data)
{
    FILE *file;
    errno_t err;

    if((err = fopen_s(&file, fileName.GetCharArray(), "rb")) != 0)
    {
        LastError = "Couldn't open file : " + fileName;
        return false;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* str = new char[fileSize + 1];
    fread(str, fileSize, 1, file);
    fclose(file);

    str[fileSize] = '\0';
    data = CString(str);

    delete[] str;
    return true;
}

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

        // add instance
        ISerializedClass* serializedObject = dynamic_cast<ISerializedClass*>(deserializedDataNode);
        CStatics::Get()->AddSerializedObject(serializedObject);
    }

    reader->Close();
    delete reader;

    std::vector<ISerialized*> entities;

    for (unsigned int x = 0; x < deserializedNodes.size(); x++)
    {
        if(deserializedNodes[x]->SerializedName() == "Entity")
        {
            entities.push_back(deserializedNodes[x]);
            // won't return the unique id
            delete deserializedNodes[x];
        }

        else if(deserializedNodes[x]->SerializedName() == "EntityIdCollection")
        {
            CEntityComponentIdSetter::UpdateIds(deserializedNodes[x]);
            // will return the unique id
            CStatics::Get()->Destroy(dynamic_cast<ISerializedClass*>(deserializedNodes[x]));
        }
    }

    CAssetManager::Get()->RemoveAssetType("Entity");
    CAssetManager::Get()->RemoveAssetType("EntityIdCollection");
    
    // check if the ssd file contains entities
    if (entities.size() > 0)
        CSceneManager::Get()->AddScene(fileName, CSceneMaker::Create(nodes));
    else
        for (size_t x = 0; x < nodes.size(); x++)
            delete nodes[x];
    return true;
}
