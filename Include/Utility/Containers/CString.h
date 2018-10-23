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

    CString& operator= (const CString& str);
    char operator[] (unsigned index);
    char& operator[] (unsigned index) const;

    friend bool operator==(const CString &a, const CString &b);
    friend bool operator==(const CString &a, const char* b);

    
    friend bool operator< (const CString &a, const CString &b);
    friend bool operator> (const CString &a, const CString &b);

    CString& operator+=(const CString &str);
    friend CString operator+(const char* chars, const CString& str);
    friend CString operator+(const CString& a, const CString& b);

    std::string GetStdString() const;
    const char* GetCharArray() const;
    
    int IndexOf(char character);
    std::vector<CString> Split(char delim) const;

    bool StartsWith(const CString &inputString);

private:
    char* Data;
    unsigned int ArrayLength;
};
