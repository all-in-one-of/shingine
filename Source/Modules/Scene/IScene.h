#pragma once
#include <vector>
#include "../Object/IObject.h"
#include "Common.h"

class IScene // : public IObject
{
public:
    virtual ~IScene() {};
    virtual IObject* MakeObject() = 0;
    virtual IObject* GetObject(const unsigned int &id) const = 0;

    virtual void GetObjectIdByName(unsigned int &id, bool &didFind) const = 0;
    virtual void DeleteObject(const unsigned int &id) = 0;
    virtual void SetParent(const unsigned int &id, const unsigned int &parentId) = 0;
    virtual IObject* CloneObject(const IObject* object) = 0;

    virtual std::vector<IObject*> FindObjectsByType(const CString type) const = 0;
};
