#pragma once
#include "Asset.h"
#include "Utility/Data/Serialization.h"

class Mesh : public Asset, public IObject {
public:
  SERIALIZE_CLASS(Mesh);
  Mesh() {
    ATTRIBUTE_REGISTER(Mesh, Name);
    ATTRIBUTE_REGISTER(Mesh, Indices);
    ATTRIBUTE_REGISTER(Mesh, Normals);
    ATTRIBUTE_REGISTER(Mesh, Positions);
    ATTRIBUTE_REGISTER(Mesh, TexCoord)
  };
  virtual ~Mesh() {}

  ATTRIBUTE_VALUE(String, Name);
  ATTRIBUTE_VECTOR(unsigned int, Indices); // n
  ATTRIBUTE_GLM_VEC3_ARRAY(Normals);
  ATTRIBUTE_GLM_VEC3_ARRAY(Positions);
  ATTRIBUTE_GLM_VEC3_ARRAY(TexCoord);
  // TODO add bounding box calculation
};
