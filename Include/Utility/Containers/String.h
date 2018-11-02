#pragma once
#include <iostream>
#include <string>
#include <vector>

// A wrapper class with additional functionality

class String {
public:
  String();
  String(char character);
  String(const char *sourceString);
  String(std::string sourceStdString);
  String(const String &sourceString);

  ~String();

  unsigned int Length() const;

  String &operator=(const String &str);
  char operator[](unsigned index);
  char &operator[](unsigned index) const;
  operator std::string() const;

  friend bool operator==(const String &a, const String &b);
  friend bool operator==(const String &a, const char *b);

  friend bool operator<(const String &a, const String &b);
  friend bool operator>(const String &a, const String &b);

  String &operator+=(const String &str);
  friend String operator+(const char *chars, const String &str);
  friend String operator+(const String &a, const String &b);

  friend std::ostream &operator<<(std::ostream &outStream, const String &str);
  friend std::istream &operator>>(std::istream &inStream, String &str);

  const char *GetCharArray() const;

  int IndexOf(char character);
  std::vector<String> Split(char delim) const;

  bool StartsWith(const String &inputString);

private:
  char *Data;
  unsigned int ArrayLength;
};
