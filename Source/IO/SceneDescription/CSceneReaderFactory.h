#pragma once

#include "Common.h"
#include "ISceneReader.h"

class CSceneReaderFactory
{
public:
    ISceneReader* CreateReader(const CString &fileName);
};
