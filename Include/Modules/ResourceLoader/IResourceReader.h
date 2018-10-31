#pragma once
#include "Common.h"
#include <fstream>
#include <vector>

class IDataNode;

class IResourceReader {
public:
  virtual ~IResourceReader(){};

  virtual String GetLastError() = 0;
  virtual bool Open() = 0;
  virtual void ReadNodes(std::vector<IDataNode *> &nodes) = 0;
  virtual void Close() = 0;
};
