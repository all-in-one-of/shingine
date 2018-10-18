#include <fstream>
#include <iostream>
#include "CSceneDescription.h"
#include "CSceneReaderFactory.h"
#include "../../Modules/Scene/CSceneMaker.h"
#include "../../Utility/Data/IDataNode.h"
#include "../../Utility/Data/ISerialized.h"
#include "../../Modules/Object/IObject.h"

// code which creates IDataNode
// code which will deserialize data node

IScene * CSceneDescription::GenerateScene()
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

CString CSceneDescription::GetLastError() { return LastError; }

CSceneDescription::~CSceneDescription()
{
    //for (unsigned int x = 0; x < Nodes.size(); x++)
    //{
    //    delete Nodes[x];
    //}
}

bool CSceneDescription::IsLoaded() { return Loaded; }

ISceneDescription* CSceneDescription::Load(const CString &fileName)
{
    CSceneDescription* sceneDescription = new CSceneDescription(fileName);
    if (!sceneDescription->IsLoaded())
        std::cout << sceneDescription->LastError.GetStdString() << std::endl;
    return sceneDescription;
}

CSceneDescription::CSceneDescription(CString fileName)
{
    // 3 file types are supported
    // *.ssd - binary version
    // *.ssda - ascii version
    // *.ssd_json - json

    CSceneReaderFactory sceneReaderFactory;
    ISceneReader* reader = sceneReaderFactory.CreateReader(fileName);

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
