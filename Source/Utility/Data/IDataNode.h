#include <vector>
#include "../Common.h"
#include "ITypedAttribute.h"

class IDataNode
{
public:
    virtual ~IDataNode() {};
    virtual unsigned int ID() = 0;
    virtual CString Name() = 0;
    virtual std::vector<ITypedAttribute*> GetAttributes() = 0;
    virtual std::vector<IDataNode*> GetNodes() = 0;
};
