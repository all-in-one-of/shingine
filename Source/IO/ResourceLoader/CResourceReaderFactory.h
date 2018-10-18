#pragma once

#include "Common.h"
#include "IResourceReader.h"

class CResourceReaderFactory
{
public:
    IResourceReader* CreateReader(const CString &fileName);
};
