#pragma once
#include <vector>
class CString;
class ISerialized;

class CResourceLoader
{
public:
    static bool Load(const CString &fileName);
    static bool LoadText(const CString &fileName, CString &data);
    static CString GetLastError();
private:
    CResourceLoader() {};
    static void SetupPath(const CString &localPath, CString& outPath);
    static CString LastError;
};
