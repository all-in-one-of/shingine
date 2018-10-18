#include "CResourceReaderFactory.h"
#include "CResourceReaderBinary.h"

IResourceReader* CResourceReaderFactory::CreateReader(const CString &fileName)
{
    std::vector<CString> elems = fileName.Split('.');
    CString fileExtensionName = elems[elems.size() - 1];

    // TODO add ascii and json formats
    if (fileExtensionName == "ssd") return new CResourceReaderBinary(fileName);
    else if (fileExtensionName == "ssda") return NULL;
    else if (fileExtensionName == "ssd_json") return NULL;
    return NULL;
}
