#pragma once

namespace SSD {
const unsigned char NodeBegin = 0xaa;
const unsigned char NodeEnd = 0xab;
const unsigned char AttributeBegin = 0xba;
const unsigned char AttributeEnd = 0xbb;

struct Header {
  char Signature[3];
  unsigned char Version;
};

struct Node;

struct Attribute {
  unsigned char NameLength;
  char *Name;
  unsigned char DataTypeLength;
  char *DataType;
  unsigned char IsSingleElement;
  unsigned int ByteCount;
  unsigned int ElementCount;
  unsigned char *Values;
  Node **Nodes;
  ~Attribute();
};

struct Node {
  unsigned char NameLength;
  char *Name;
  unsigned int UniqueID;
  unsigned char AttributeCount;
  unsigned char NodeCount;
  Attribute **Attributes;
  Node **Nodes;
  ~Node();
};
}; // namespace SSD
