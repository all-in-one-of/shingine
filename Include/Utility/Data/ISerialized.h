#pragma once
#include "../Common.h"
#include <iostream>

class ISerialized {
public:
  virtual ~ISerialized(){};
  virtual String SerializedName() = 0;
  virtual String TypeName() = 0;
};

class IObject : public ISerialized {
public:
  virtual ~IObject(){};
  virtual unsigned int UniqueID() = 0;
  virtual void SetUniqueID(unsigned int newId) = 0;
  virtual String TypeName() { return "SerializedClass"; };
  virtual void SetAttribute(ISerialized *&attr) = 0;
  virtual void SetAttribute(const String &serializedName,
                            ISerialized *&attr) = 0;
  virtual void GetAttribute(ISerialized *&attr) = 0;
  virtual void GetAllAttributes(std::vector<ISerialized *> &attributes) = 0;
};
