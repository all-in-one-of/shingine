#pragma once
#include <vector>
class CString;
class ISerialized;

class CResourceLoader
{
public:
    static bool Load(const CString &fileName);
    static CString GetLastError();
private:
    CResourceLoader() {};
    static CString LastError;
};
