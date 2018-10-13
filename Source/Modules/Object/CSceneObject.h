#pragma once

#include "CObject.h"

class CSceneObject : public CObject
{
public:
    virtual ~CSceneObject();
    CSceneObject(unsigned int id);
    CString Name();
    CSceneObject* Parent();
    void SetParent(CSceneObject* object);
    void SetName(const CString &newName);

private:
    CSceneObject* ParentObject;
    CString ObjectName;
};
