#pragma once

#include <map>
#include "IScene.h"

class CScene : public IScene
{
public:
    CScene();
    virtual ~CScene();

    // Object Management
    virtual IObject* MakeObject();
    virtual IObject* GetObject(const unsigned int &id) const;
    virtual IObject* CloneObject(const IObject* object);

    virtual unsigned int GetObjectIdByName(const CString &name, bool &didFind) const;
    virtual void DeleteObject(const unsigned int &id);
    virtual void SetParent(const unsigned int &id, const unsigned int &parentId);

    virtual void FindObjectsByType(const CString type, std::vector<IObject*> &objects) const;

private:
    std::map<unsigned int, IObject*> Objects;
    std::map<unsigned int, unsigned int> IdToParentId;
    unsigned int NextID = 1;
};
