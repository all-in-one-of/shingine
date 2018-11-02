#pragma once
#include <iomanip>
#include <map>
#include <string>
#include <unordered_map>

#include "ISerialized.h"

template <typename T> ISerialized *createT() { return new T; }

class SerializedFactory {
public:
  typedef std::map<std::string, ISerialized *(*)()> TSerializedTypeMap;
  typedef std::unordered_map<std::string, std::string> TypeNameMap;

  static ISerialized *CreateInstance(const std::string &s, bool setUid = true);
  static void GetDemangledName(String &name);

protected:
  static TSerializedTypeMap *GetMap();
  static TypeNameMap *GetTypeNameMap();

private:
  static TSerializedTypeMap *Map;
  static TypeNameMap *TypeNames;
};

template <typename T> class SerializedRegistry : SerializedFactory {
public:
  SerializedRegistry(const String &typeName, const String &mangledName) {
    GetMap()->insert(std::make_pair(typeName, &createT<T>));
    GetTypeNameMap()->insert(std::make_pair(mangledName, typeName));
  }
};
