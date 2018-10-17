#include <fstream>
#include <iostream>
#include "CSceneDescription.h"
#include "CSceneReaderFactory.h"
#include "../../Modules/Scene/CSceneMaker.h"
#include "../../Utility/Data/IDataNode.h"

IScene * CSceneDescription::GenerateScene()
{
    if (!IsLoaded()) return NULL;
    return CSceneMaker::Create(Nodes);
}

CString CSceneDescription::GetLastError() { return LastError; }

CSceneDescription::~CSceneDescription()
{
    for (unsigned int x = 0; x < Nodes.size(); x++)
    {
        delete Nodes[x];
    }
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

    reader->ReadNodes(Nodes);

    reader->Close();
    delete reader;
    Loaded = true;
}
