#pragma once
#include <vector>
#include "SSD.h"
#include "ISceneDescription.h"

class CSceneDescription : public ISceneDescription
{
public:
    virtual ~CSceneDescription();
    static ISceneDescription* Load(const CString &fileName);
    virtual CString GetLastError();
    virtual IScene* GenerateScene();
private:
    CSceneDescription(CString fileName, bool &success);
    bool Read(const CString &fileName);
    struct SSD::SHeader Header;
    unsigned short NodeCount;
    struct SSD::SNode** Nodes;
    CString LastError = "";
};
