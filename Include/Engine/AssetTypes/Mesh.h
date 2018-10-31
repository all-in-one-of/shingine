#pragma once
#include "Utility/Data/Serialization.h"

class Mesh : public ISerializedClass {
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
  ATTRIBUTE_VECTOR(float, Normals);        // n * 3
  ATTRIBUTE_VECTOR(float, Positions);      // n * 3
  ATTRIBUTE_VECTOR(float, TexCoord);       // n * 3

  // TODO add bounding box calculation
};
