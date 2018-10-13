#pragma once

#include "CObject.h"

class CSceneObject : public CObject
{
public:
    virtual ~CSceneObject();
    CSceneObject(unsigned int id);
    CString Name();
    CSceneObject* Parent();
private:
    CSceneObject* ParentObject;
    CString ObjectName;
};
