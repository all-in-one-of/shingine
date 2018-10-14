#include <fstream>
#include <iostream>
#include "CSceneDescription.h"
#include "CSceneReaderFactory.h"
#include "../../Modules/Scene/CSceneMaker.h"

IScene * CSceneDescription::GenerateScene()
{
    if (!IsLoaded()) return NULL;
    return CSceneMaker::Create(Nodes, NodeCount);
}

CString CSceneDescription::GetLastError() { return LastError; }

CSceneDescription::~CSceneDescription()
{
    for (unsigned int x = 0; x < NodeCount; x++)
    {
        delete Nodes[x];
    }
    delete [] Nodes;
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
    Loaded = Read(fileName);
}

bool CSceneDescription::Read(const CString &fileName)
{
    CSceneReaderFactory sceneReaderFactory;
    ISceneReader* reader = sceneReaderFactory.CreateReader(fileName);

    if (!reader->Open())
    {
        LastError = "Couldn't open file : " + fileName;
        delete reader;
        return false;
    }

    reader->ReadHeader(Header);
    reader->ReadUShort(NodeCount);

    Nodes = new SSD::SNode*[NodeCount];

    for (unsigned int x = 0; x < NodeCount; x++)
        Nodes[x] = reader->ReadNode();

    reader->Close();
    delete reader;
    return true;
}
