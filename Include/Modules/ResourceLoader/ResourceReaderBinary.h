#pragma once
#include "IResourceReader.h"

namespace SSD {
struct Header;
struct Node;
struct Attribute;
} // namespace SSD

class ResourceReaderBinary : public IResourceReader {
public:
  ResourceReaderBinary(const String &fileName);
  virtual ~ResourceReaderBinary();
  virtual bool Open();
  virtual void Close();
  virtual void ReadNodes(std::vector<IDataNode *> &nodes);
  virtual String GetLastError() { return LastError; }
  // virtual
private:
  void ReadHeader(SSD::Header &header);
  void ReadUShort(unsigned short &val);
  void ReadByte(unsigned char &val);
  void ReadUInt32(unsigned int &val);
  SSD::Node *ReadNode();
  SSD::Attribute *ReadAttribute();

  String FileName;
  String LastError;
  std::ifstream FileStream;
};
