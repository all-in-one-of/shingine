#pragma once
#include <vector>
#include "../../Utility/SSD.h"
#include "ISceneDescription.h"

class CSceneDescription : public ISceneDescription
{
public:
    virtual ~CSceneDescription();
    static ISceneDescription* Load(const CString &fileName);
    virtual bool IsLoaded();
    virtual CString GetLastError();
    virtual IScene* GenerateScene();
private:
    CSceneDescription(CString fileName);
    bool Read(const CString &fileName);
    bool Loaded = false;
    CString LastError = "";

    struct SSD::SHeader Header;
    unsigned short NodeCount;
    struct SSD::SNode** Nodes;
};
