#pragma once

#include "IScene.h"
#include <map>

namespace SSD { struct SNode; }
class CScene : public IScene
{
public:
    virtual ~CScene();
    static IScene* Create(SSD::SNode** nodes, const unsigned int &nodeCount);

    // Object Management
    virtual IObject* MakeObject();
    virtual IObject* GetObject(const unsigned int &id) const;
    virtual IObject* CloneObject(const IObject* object);

    virtual void GetObjectIdByName(unsigned int &id, bool &didFind) const;
    virtual void DeleteObject(const unsigned int &id);
    virtual void SetParent(const unsigned int &id, const unsigned int &parentId);

    virtual std::vector<IObject*> FindObjectsByType(const CString type) const;

private:
    CScene();
    std::map<unsigned int, IObject*> Objects;
};
