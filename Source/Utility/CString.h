#pragma once
#include <string>
#include <vector>

// A wrapper class with additional functionality

class CString 
{
public:
    CString();
    CString(char character);
    CString(const char* sourceString);
    CString(std::string sourceStdString);
    CString(const CString &sourceString);

    ~CString();

    unsigned int Length() const;

    char operator[] (unsigned index);
    char& operator[] (unsigned index) const;

    friend bool operator==(const CString &a, const CString &b);
    friend bool operator==(const CString &a, const char* b);

    CString& operator+=(const CString &str);
    friend CString operator+(const char* chars, const CString& str);

    std::string GetStdString();
    const char* GetCharArray();
    
    int IndexOf(char character);
    std::vector<CString> Split(char delim);

private:
    char* Data;
    unsigned int ArrayLength;
};
