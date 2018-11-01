#pragma once
#include "Core.h"
class Asset {
public:
  enum OriginType { Runtime, Scene, External };
  virtual ~Asset() {}
  virtual OriginType GetOrigin() { return Origin; }
  virtual String GetFileName() { return FileName; }
  virtual void SetOrigin(OriginType originType) { Origin = originType; }
  virtual void SetFileName(const String &newFileName) {
    FileName = newFileName;
  }
private:
  OriginType Origin = OriginType::Runtime;
  String FileName = "";
};
