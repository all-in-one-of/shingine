#include "CSceneObject.h"

CSceneObject::CSceneObject(unsigned int id) : CObject(id)
{
}

CString CSceneObject::Name()
{
    return ObjectName;
}

void CSceneObject::SetParent(CSceneObject* object)
{
    ParentObject = object;
}

void CSceneObject::SetName(const CString &newName)
{
    ObjectName = newName;
}

CSceneObject* CSceneObject::Parent()
{
    return ParentObject;
}

CSceneObject::~CSceneObject()
{
}
