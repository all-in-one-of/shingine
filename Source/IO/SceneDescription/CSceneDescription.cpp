#include <fstream>
#include <iostream>
#include "CSceneDescription.h"
#include "CSceneReaderFactory.h"
#include "../../Modules/Scene/CSceneMaker.h"

IScene * CSceneDescription::GenerateScene()
{
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

ISceneDescription* CSceneDescription::Load(const CString &fileName, bool &success)
{
    CSceneDescription* sceneDescription = new CSceneDescription(fileName, success);
    if (!success)
        std::cout << sceneDescription->LastError.GetStdString() << std::endl;
    return sceneDescription;
}

CSceneDescription::CSceneDescription(CString fileName, bool &success)
{
    // 3 file types are supported
    // *.ssd - binary version
    // *.ssda - ascii version
    // *.ssd_json - json
    success = Read(fileName);
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
