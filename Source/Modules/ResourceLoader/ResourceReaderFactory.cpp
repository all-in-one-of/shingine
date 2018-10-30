#include "Modules/ResourceLoader/CResourceReaderFactory.h"
#include "Modules/ResourceLoader/ResourceReaderBinary.h"

IResourceReader* CResourceReaderFactory::CreateReader(const String &fileName)
{
    std::vector<String> elems = fileName.Split('.');
    String fileExtensionName = elems[elems.size() - 1];

    // TODO add ascii and json formats
    if (fileExtensionName == "ssd") return new ResourceReaderBinary(fileName);
    else if (fileExtensionName == "ssda") return nullptr;
    else if (fileExtensionName == "ssd_json") return nullptr;
    return nullptr;
}
