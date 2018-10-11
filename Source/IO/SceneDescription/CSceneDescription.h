#pragma once
#include <vector>
#include "ISceneDescription.h"
#include "SSD.h"

class CSceneDescription : public ISceneDescription
{
public:
    virtual ~CSceneDescription();
    static ISceneDescription* Load(std::string fileName);
    virtual std::string GetLastError();
    virtual IScene* GenerateScene();
private:
    CSceneDescription(std::string fileName, bool &success);
    struct SSD::SHeader Header;
    struct SSD::SNode* Nodes;
    std::string LastError = "";
};
