#include "CSceneDescription.h"
#include <fstream>
#include <iostream>

std::string CSceneDescription::GetLastError() { return LastError; }

IScene * CSceneDescription::GenerateScene()
{
    return nullptr;
}

CSceneDescription::~CSceneDescription()
{
}

ISceneDescription* CSceneDescription::Load(std::string fileName)
{
    bool readSuccess = false;
    CSceneDescription* sceneDescription = new CSceneDescription(fileName, readSuccess);
    if (!readSuccess)
        std::cout<<sceneDescription->GetLastError()<<std::endl;
    return sceneDescription;
}

CSceneDescription::CSceneDescription(std::string fileName, bool &success)
{
    std::ifstream file;
    file.open(fileName, std::ios::binary | std::ios::in);
    if (!file.is_open())
    {
        LastError = "Couldn't open the file: " + fileName;
        success = false;
        return;
    } 

    Header = SSD::SHeader();
    file.read(Header.Signature, 3);
    file.read((char*)&Header.Version, 1);

    std::cout << "Signature :";
    for (unsigned int x = 0; x < 3; x++)
        std::cout << Header.Signature[x];

    std::cout << std::endl
    << "Version: " << +Header.Version << std::endl;
    
    success = true;
}