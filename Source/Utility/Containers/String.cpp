#include <iostream>
#include <sstream>
#include <string.h>

#include "Utility/Containers/String.h"

String::String() {
  ArrayLength = 0;
  Data = new char[1]{'\0'};
}

String::String(char character) {
  ArrayLength = 1;
  Data = new char[2]{character, '\0'};
}

String::String(const char *sourceString) {
  ArrayLength = 0;
  if (sourceString)
    for (ArrayLength = 0; sourceString[ArrayLength] != '\0'; ArrayLength++)
      ;

  Data = new char[ArrayLength + 1];
  if (ArrayLength > 0)
    for (unsigned int x = 0; x < ArrayLength; x++)
      Data[x] = sourceString[x];
  Data[ArrayLength] = '\0';
}

String::operator std::string() const {
  return std::string(Data);
}

bool String::StartsWith(const String &inputString) {
  return ArrayLength < inputString.Length()
             ? false
             : strncmp(inputString.GetCharArray(), Data,
                       inputString.Length()) == 0;
}

std::ostream &operator<<(std::ostream &outStream, const String &str) {
  for (unsigned int x = 0; x < str.Length(); x++)
    outStream << str[x];
  return outStream;
}

std::istream &operator>>(std::istream &inStream, String &str) {
  char *input = new char[1024];
  inStream >> input;
  delete[] input;
  return inStream;
}

String::String(std::string sourceStdString) : String(sourceStdString.c_str()){};

String::String(const String &sourceString) {
  ArrayLength = sourceString.Length();
  Data = new char[ArrayLength + 1];

  for (unsigned int x = 0; x < ArrayLength; x++)
    Data[x] = sourceString[x];

  Data[ArrayLength] = '\0';
}

String &String::operator=(const String &str) {
  if (this == &str)
    return *this;

  delete[] Data;
  ArrayLength = str.Length();
  Data = new char[ArrayLength + 1];

  for (unsigned int x = 0; x < ArrayLength; x++)
    Data[x] = str[x];

  Data[ArrayLength] = '\0';
  return *this;
}

char String::operator[](unsigned index) {
  if (index >= ArrayLength)
    throw 1;
  return Data[index];
}

String &String::operator+=(const String &str) {
  unsigned int strLength = str.Length();
  unsigned int newLength = ArrayLength + strLength;
  char *newStr = new char[newLength + 1];

  for (unsigned int x = 0; x < ArrayLength; x++)
    newStr[x] = Data[x];

  for (unsigned int x = 0; x < strLength; x++)
    newStr[x + ArrayLength] = str[x];

  delete[] Data;
  ArrayLength = newLength;
  Data = newStr;
  Data[ArrayLength] = '\0';
  return *this;
}

String operator+(const char *chars, const String &str) {
  return String(chars) += str;
}

String operator+(const String &a, const String &b) { return String(a) += b; }

char &String::operator[](unsigned index) const {
  if (index >= ArrayLength)
    throw 1;
  return Data[index];
}

bool operator==(const String &a, const String &b) {
  if (a.Length() != b.Length())
    return false;
  unsigned int check = 0;
  unsigned int aLength = a.Length();
  for (check = 0; check < aLength && a[check] == b[check]; check++)
    ;
  return aLength == check;
}

bool operator==(const String &a, const char *b) { return a == String(b); }

bool operator<(const String &a, const String &b) {
  return !(a == b) && (a > b);
}

bool operator>(const String &a, const String &b) {
  return strcmp(a.GetCharArray(), b.GetCharArray()) > 0;
}

int String::IndexOf(char character) {
  for (unsigned int x = 0; x < ArrayLength; x++)
    if (Data[x] == character)
      return x;
  return -1;
}

String::~String() { delete[] Data; }

unsigned int String::Length() const { return ArrayLength; }
const char *String::GetCharArray() const { return Data; }

std::vector<String> String::Split(char delimeter) const {
  std::stringstream stringStream(Data);
  std::vector<String> e;
  std::string part;
  while (std::getline(stringStream, part, delimeter))
    e.push_back(String(part));
  return e;
}
