#include <fstream>
#include <iostream>
#include "CResourceLoader.h"
#include "CResourceReaderFactory.h"
#include "../../Modules/Scene/CSceneMaker.h"
#include "../../Utility/Data/IDataNode.h"
#include "../../Utility/Data/ISerialized.h"
#include "../../Modules/Object/IObject.h"

// code which creates IDataNode
// code which will deserialize data node

IScene * CResourceLoader::GenerateScene()
{
    if (!IsLoaded()) return NULL;

    std::vector<IObject*> objects;
    for (unsigned int x = 0; x < Nodes.size(); x++)
        if(Nodes[x]->SerializedName() == "Object")
        {
            objects.push_back(
                dynamic_cast<IObject*>(Nodes[x]));

        }

    return CSceneMaker::Create(objects);
}

CString CResourceLoader::GetLastError() { return LastError; }

CResourceLoader::~CResourceLoader()
{
}

bool CResourceLoader::IsLoaded() { return Loaded; }

IResourceLoader* CResourceLoader::Load(const CString &fileName)
{
    CResourceLoader* ResourceLoader = new CResourceLoader(fileName);
    if (!ResourceLoader->IsLoaded())
        std::cout << ResourceLoader->LastError.GetStdString() << std::endl;
    return ResourceLoader;
}

CResourceLoader::CResourceLoader(CString fileName)
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
        Loaded = false;
        return;
    }
    std::vector<IDataNode*> nodes;
    reader->ReadNodes(nodes);

    for (unsigned int x = 0; x < nodes.size(); x++)
    {
        ISerialized* deserializedDataNode = nodes[x]->Deserialize();
        if (deserializedDataNode)
            Nodes.push_back(deserializedDataNode);
        delete nodes[x];
    }

    reader->Close();
    delete reader;
    Loaded = true;
}
