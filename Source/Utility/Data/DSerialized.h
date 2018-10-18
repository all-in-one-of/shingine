#include <string>
#include <map>

class ISerialized;
#define ATTRIBUTE_DECL_INIT(CLASSNAME) \
    typedef void (CLASSNAME::*MFP)(ISerialized* &attr); \
    std::map<std::string, MFP> AttributeFunctionMap; \
    std::vector<std::string> AttributeNames; \
    virtual void SetAttribute(ISerialized* &attr) \
    { \
        std::string mapName = std::string("Set_") + attr->SerializedName().GetStdString(); \
        if (AttributeFunctionMap.find(mapName) == AttributeFunctionMap.end()) \
            return; \
        MFP fp = AttributeFunctionMap[mapName]; \
        (this->*fp)(attr); \
    }; \
    virtual void GetAttribute(ISerialized* &attr) \
    { \
        std::string mapName = std::string("Get_") + attr->SerializedName().GetStdString(); \
        if (AttributeFunctionMap.find(mapName) == AttributeFunctionMap.end()) return; \
        MFP fp = AttributeFunctionMap[mapName]; \
        (this->*fp)(attr); \
    }; \
    virtual void GetAllAttributes(std::vector<ISerialized*> &attributes) \
    { \
        for (unsigned int x = 0; x < AttributeNames.size(); x++) \
        { \
            ISerialized* newAttr; \
            MFP fp = AttributeFunctionMap[std::string("Get_") + AttributeNames[x]]; \
            (this->*fp)(newAttr); \
            attributes.push_back(newAttr); \
        } \
    }; \

#define ATTRIBUTE_CLASS_INFO(CLASSNAME,NAME) \
    CLASSNAME* NAME; \
    void Attrib_Set_##NAME(ISerialized* &attr) \
    { \
        NAME = dynamic_cast<CLASSNAME*>(attr); \
    } \
    void Attrib_Get_##NAME(ISerialized* &attr) \
    { \
        attr = NAME; \
    }

#define ATTRIBUTE_VALUE_INFO(TYPE_NAME,NAME) \
    TYPE_NAME NAME; \
    void Attrib_Set_##NAME(ISerialized* &attr) \
    { \
        NAME = ((CTypedAttributeValue<TYPE_NAME> *)attr)->Get(); \
    } \
    void Attrib_Get_##NAME(ISerialized* &attr) \
    { \
        attr = new CTypedAttributeValue<TYPE_NAME>(#NAME, #TYPE_NAME, NAME); \
    }

#define ATTRIBUTE_VECTOR_INFO(TYPE_NAME,NAME) \
    std::vector<TYPE_NAME> NAME; \
    void Attrib_Set_##NAME(ISerialized* &attr) \
    { \
        CTypedAttribute<TYPE_NAME> * typedAttr = (CTypedAttribute<TYPE_NAME> *)attr; \
        if (typedAttr == NULL) return; \
        NAME = typedAttr->Get(); \
    } \
    void Attrib_Get_##NAME(ISerialized* &attr) \
    { \
        attr = new CTypedAttribute<TYPE_NAME>(#NAME, #TYPE_NAME, NAME); \
    } 

#define ATTRIBUTE_REGISTER(CLASSNAME,NAME) \
    AttributeNames.push_back(#NAME); \
    AttributeFunctionMap.insert( std::make_pair( std::string("Get_") + #NAME , &CLASSNAME::Attrib_Get_##NAME )); \
    AttributeFunctionMap.insert( std::make_pair( std::string("Set_") + #NAME , &CLASSNAME::Attrib_Set_##NAME )); 
