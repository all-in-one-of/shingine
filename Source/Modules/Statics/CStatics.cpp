#include "Modules/Statics/CStatics.h"

CStatics* CStatics::Instance = NULL;

CSceneManager* CStatics::SceneManager() { return CSceneManager::Get(); }
