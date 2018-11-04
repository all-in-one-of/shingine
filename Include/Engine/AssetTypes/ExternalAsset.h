#pragma once
#include "Asset.h"
#include "Utility/Data/Serialization.h"

class ExternalAsset : public IObject {
public:
  SERIALIZE_CLASS(ExternalAsset);
  ExternalAsset() {
    ATTRIBUTE_REGISTER(ExternalAsset, Name);
    ATTRIBUTE_REGISTER(ExternalAsset, FileName);
  }
  virtual ~ExternalAsset(){};
  ATTRIBUTE_VALUE(String, Name)
  ATTRIBUTE_VALUE(String, FileName)
};
