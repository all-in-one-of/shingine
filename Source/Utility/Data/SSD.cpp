#include "Utility/Data/SSD.h"
#include <stddef.h>

namespace SSD {
Node::~Node() {
  delete[] Name;
  for (unsigned char x = 0; x < AttributeCount; x++)
    delete Attributes[x];
  delete[] Attributes;

  for (unsigned char x = 0; x < NodeCount; x++)
    delete Nodes[x];
  delete[] Nodes;
}

Attribute::~Attribute() {
  delete[] Name;
  delete[] DataType;
  delete[] Values;

  if (!Nodes)
    return;

  for (size_t x = 0; x < ElementCount; x++) {
    if (!Nodes[x])
      continue;
    delete Nodes[x];
  }
  delete[] Nodes;
}
}; // namespace SSD
