#pragma once
#include <vector>
#include "IResourceLoader.h"

class ISerialized;

class CResourceLoader : public IResourceLoader
{
public:
    virtual ~CResourceLoader();
    static IResourceLoader* Load(const CString &fileName);
    virtual bool IsLoaded();
    virtual CString GetLastError();
    virtual IScene* GenerateScene();

private:
    CResourceLoader(CString fileName);

    bool Loaded = false;
    CString LastError = "";

    class std::vector<ISerialized*> Nodes;
};
