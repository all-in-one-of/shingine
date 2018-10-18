#pragma once
#include <vector>
#include "ISceneDescription.h"

class ISerialized;

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

    bool Loaded = false;
    CString LastError = "";

    class std::vector<ISerialized*> Nodes;
};
