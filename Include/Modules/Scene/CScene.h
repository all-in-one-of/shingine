#pragma once

#include <map>
#include "IScene.h"

class CScene : public IScene
{
public:
    CScene();
    virtual ~CScene();

    // Object Management
    virtual IObject* AddObject(const CString &objectName = "Object");
    virtual IObject* GetObject(const unsigned int &id) const;
    virtual IObject* CloneObject(const IObject* object);

    virtual unsigned int GetObjectIdByName(const CString &name, bool &didFind) const;
    virtual void DeleteObject(const unsigned int &id);
    virtual void SetParent(const unsigned int &id, const unsigned int &parentId);

    virtual void FindObjectsByType(const CString type, std::vector<IObject*> &objects) const;
    
    virtual void GetComponents(const CString &type, std::map<unsigned int, IComponent*> &componentMap) const;
    virtual void GetComponents(const CString &type, std::vector<IComponent*> &components) const;

    void AddCreatedObject(IObject* object);
private:
    std::map<unsigned int, IObject*> Objects;
    unsigned int NextID = 1;
};
