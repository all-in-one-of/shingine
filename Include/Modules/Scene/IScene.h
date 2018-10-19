#pragma once
#include <vector>
#include "Engine/IObject.h"
#include "Common.h"

class IScene // : public IObject
{
public:
    virtual ~IScene() {};
    virtual IObject* AddObject(const CString &objectName) = 0;
    virtual IObject* GetObject(const unsigned int &id) const = 0;

    virtual unsigned int GetObjectIdByName(const CString &name, bool &didFind) const = 0;
    virtual void DeleteObject(const unsigned int &id) = 0;
    virtual void SetParent(const unsigned int &id, const unsigned int &parentId) = 0;
    virtual IObject* CloneObject(const IObject* object) = 0;

    virtual void FindObjectsByType(const CString type, std::vector<IObject*> &objects) const = 0;

    virtual void GetComponents(const CString &type, std::map<unsigned int, IComponent*> &componentMap) const = 0;
    virtual void GetComponents(const CString &type, std::vector<IComponent*> &components) const = 0;
};
