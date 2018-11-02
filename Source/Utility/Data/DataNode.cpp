#include "Utility/Data/DataNode.h"
#include "Utility/Data/DataStruct.h"
#include "Utility/Data/SSD.h"
#include "Utility/Data/SerializedFactory.h"
#include "Utility/Data/TypedAttribute.h"

DataNode::DataNode(SSD::Node *node) {
  NodeName = String(node->Name);
  UniqueID = node->UniqueID;
  for (unsigned int x = 0; x < node->NodeCount; x++)
    Nodes.push_back(new DataNode(node->Nodes[x]));

  for (unsigned int x = 0; x < node->AttributeCount; x++) {
    SSD::Attribute *attribute = node->Attributes[x];
    String typeName = String(attribute->DataType);
    Attributes.push_back(MakeTypedAttribute(attribute));
  }
}

unsigned int DataNode::GetUniqueID() { return UniqueID; }

void DataNode::SetUniqueID(const unsigned int &newID) { UniqueID = newID; }

DataNode::~DataNode() {
  for (unsigned int x = 0; x < Attributes.size(); x++)
    delete Attributes[x];

  for (unsigned int x = 0; x < Nodes.size(); x++)
    delete Nodes[x];
}

ISerialized *DataNode::Deserialize() {
  ISerializedClass *serializedClass = dynamic_cast<ISerializedClass *>(
      SerializedFactory::CreateInstance(Name(), false));

  serializedClass->SetUniqueID(UniqueID);

  if (serializedClass == nullptr)
    return serializedClass;

  for (size_t x = 0; x < Attributes.size(); x++) {
    // if it's a serialized class then deserialize each element
    if (Attributes[x]->TypeName() == "SerializedClass") {
      // check if it's an array of a single value
      TypedAttribute<IDataNode *> *attr =
          dynamic_cast<TypedAttribute<IDataNode *> *>(Attributes[x]);
      // d
      std::vector<IDataNode *> attributeNodes = attr->Get();
      std::vector<ISerialized *> deserializedAttributeNodes;
      for (size_t x = 0; x < attributeNodes.size(); x++)
        deserializedAttributeNodes.push_back(attributeNodes[x]->Deserialize());
      ISerialized *attributeClassVector = new CAttributeClassVector(
          attr->SerializedName(), deserializedAttributeNodes);
      serializedClass->SetAttribute(attributeClassVector);
    } else {
      serializedClass->SetAttribute(Attributes[x]);
    }
  }

  for (size_t x = 0; x < Nodes.size(); x++) {
    ISerialized *deserializedDataNode = Nodes[x]->Deserialize();
    serializedClass->SetAttribute(deserializedDataNode);
  }
  return serializedClass;
}

ISerialized *DataNode::MakeTypedAttribute(SSD::Attribute *attribute) {
  // unpack data here
  String name = String(attribute->Name);
  String typeName = String(attribute->DataType);

  if (typeName == "char") {
    if (attribute->IsSingleElement) {
      // single string
      char *temp = new char[attribute->ByteCount];
      for (size_t x = 0; x < attribute->ByteCount; x++)
        temp[x] = attribute->Values[x];
      String tempStr = String(temp);
      delete[] temp;
      return new TypedAttributeValue<String>(name, typeName, tempStr);
    } else {
      // an array of strings
      std::vector<String> strings;
      size_t x = 0, stringChar = 0;
      char *temp = new char[attribute->ByteCount];
      while (x != attribute->ByteCount) {
        temp[stringChar++] = attribute->Values[x++];
        if (temp[stringChar - 1] == '\0') {
          strings.push_back(temp);
          stringChar = 0;
        }
      }
      delete[] temp;
      return new TypedAttribute<String>(name, typeName, strings);
    }
  }

  if (typeName == "SerializedClass") {
    std::vector<IDataNode *> nodes;
    for (size_t x = 0; x < attribute->ElementCount; x++) {
      nodes.push_back(new DataNode(attribute->Nodes[x]));
    }
    return new TypedAttribute<IDataNode *>(name, typeName, nodes);
  }

  unsigned char stride;
  DataStruct::GetStride(typeName, stride);

  std::vector<int> data_int;
  std::vector<unsigned int> data_uint;
  std::vector<unsigned short> data_ushort;
  std::vector<short> data_short;
  std::vector<float> data_float;
  std::vector<unsigned char> data_uchar;

  for (unsigned int x = 0; x < attribute->ElementCount * stride; x += stride) {
    unsigned char *value = new unsigned char[stride];
    for (unsigned char y = 0; y < stride; y++)
      value[y] = attribute->Values[x + y];

    if (typeName == "int") {
      if (attribute->IsSingleElement) {
        ISerialized *singleElementAttribute = new TypedAttributeValue<int>(
            name, typeName, DataStruct::GetInt32(value));
        delete[] value;
        return singleElementAttribute;
      }
      data_int.push_back(DataStruct::GetInt32(value));
    }

    if (typeName == "unsigned int" || typeName == "uid") {
      if (attribute->IsSingleElement) {
        ISerialized *singleElementAttribute =
            new TypedAttributeValue<unsigned int>(name, typeName,
                                                  DataStruct::GetUInt32(value));
        delete[] value;
        return singleElementAttribute;
      }
      data_uint.push_back(DataStruct::GetUInt32(value));
    }
    if (typeName == "unsigned short") {
      if (attribute->IsSingleElement) {
        ISerialized *singleElementAttribute =
            new TypedAttributeValue<unsigned short>(
                name, typeName, DataStruct::GetUShort(value));
        delete[] value;
        return singleElementAttribute;
      }
      data_ushort.push_back(DataStruct::GetUShort(value));
    }
    if (typeName == "short") {
      if (attribute->IsSingleElement) {
        ISerialized *singleElementAttribute = new TypedAttributeValue<short>(
            name, typeName, DataStruct::GetShort(value));
        delete[] value;
        return singleElementAttribute;
      }
      data_short.push_back(DataStruct::GetShort(value));
    }
    if (typeName == "float") {
      if (attribute->IsSingleElement) {
        ISerialized *singleElementAttribute = new TypedAttributeValue<float>(
            name, typeName, DataStruct::GetFloat(value));
        delete[] value;
        return singleElementAttribute;
      }
      data_float.push_back(DataStruct::GetFloat(value));
    }
    if (typeName == "unsigned char") {
      if (attribute->IsSingleElement) {
        ISerialized *singleElementAttribute =
            new TypedAttributeValue<unsigned char>(name, typeName, value[0]);
        delete[] value;
        return singleElementAttribute;
      }

      data_uchar.push_back(value[0]);
    }
    delete[] value;
  }

  if (typeName == "int")
    return new TypedAttribute<int>(name, typeName, data_int);
  if (typeName == "unsigned int" || typeName == "uid")
    return new TypedAttribute<unsigned int>(name, typeName, data_uint);
  if (typeName == "unsigned short")
    return new TypedAttribute<unsigned short>(name, typeName, data_ushort);
  if (typeName == "short")
    return new TypedAttribute<short>(name, typeName, data_short);
  if (typeName == "float")
    return new TypedAttribute<float>(name, typeName, data_float);
  if (typeName == "unsigned char")
    return new TypedAttribute<unsigned char>(name, typeName, data_uchar);
  return nullptr;
}

String DataNode::Name() { return NodeName; }
std::vector<ISerialized *> DataNode::GetAttributes() { return Attributes; }
std::vector<IDataNode *> DataNode::GetNodes() { return Nodes; }
