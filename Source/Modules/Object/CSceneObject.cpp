#include "CSceneObject.h"

CSceneObject::CSceneObject(unsigned int id) : CObject(id)
{
}

CString CSceneObject::Name()
{
    return ObjectName;
}

CSceneObject* CSceneObject::Parent()
{
    return ParentObject;
}

CSceneObject::~CSceneObject()
{
}