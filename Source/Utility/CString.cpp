#include "CString.h"
#include <sstream>
#include <iostream>
#include <string.h>

CString::CString() 
{
    ArrayLength = 0;
    Data = new char[1]{ '\0' };
}

CString::CString(char character) 
{ 
    ArrayLength = 1;
    Data = new char[2] { character, '\0' }; 
}

CString::CString(const char* sourceString) 
{ 
    if (sourceString)
        for (ArrayLength = 0; sourceString[ArrayLength] != '\0'; ArrayLength++);

    Data = new char[ArrayLength + 1];
    if (ArrayLength > 0)
        for (unsigned int x = 0; x < ArrayLength; x++)
            Data[x] = sourceString[x];
    Data[ArrayLength] = '\0';
}

CString::CString(std::string sourceStdString) : CString(sourceStdString.c_str()) {};

CString::CString(const CString &sourceString)
{
    ArrayLength = sourceString.Length();
    Data = new char[ArrayLength + 1];

    for (unsigned int x = 0; x < ArrayLength; x++)
        Data[x] = sourceString[x];

    Data[ArrayLength] = '\0';
}

CString & CString::operator=(const CString & str)
{
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

char CString::operator[] (unsigned index)
{
    if (index >= ArrayLength) throw 1;
    return Data[index];
}

CString& CString::operator+=(const CString &str)
{
    unsigned int strLength = str.Length();
    unsigned int newLength = ArrayLength + strLength;
    char* newStr = new char[newLength + 1];

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

CString operator+(const char* chars, const CString& str)
{
    return CString(chars) += str;
}

char& CString::operator[] (unsigned index) const
{
    if (index >= ArrayLength) throw 1;
    return Data[index];
}

bool operator==(const CString &a, const CString &b)
{
    if (a.Length() != b.Length()) return false;
    unsigned int check = 0;
    unsigned int aLength = a.Length();
    for (check = 0; check < aLength && a[check] == b[check]; check++);
    return aLength == check;
}

bool operator==(const CString &a, const char* b)
{
    return a == CString(b);
}

bool operator<(const CString &a, const CString &b)
{
    return !(a == b) && (a > b);
}

bool operator>(const CString &a, const CString &b)
{
    return strcmp(a.GetCharArray(), b.GetCharArray()) > 0;
}

int CString::IndexOf(char character)
{
    for (unsigned int x = 0; x < ArrayLength; x++)
        if (Data[x] == character) return x;
    return -1;
}

CString::~CString()
{
    delete[] Data;
}

unsigned int CString::Length() const { return ArrayLength; }
const char* CString::GetCharArray() const { return Data; }
std::string CString::GetStdString() const { return std::string(Data); }

std::vector<CString> CString::Split(char delimeter) const
{
    std::stringstream stringStream(GetStdString());
    std::vector<CString> e;
    std::string part;
    while (std::getline(stringStream, part, delimeter))
    {
        auto cpart = CString(part);
        e.push_back(cpart);
    }
    return e;
}
