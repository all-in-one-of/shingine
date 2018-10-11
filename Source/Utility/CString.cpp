#include "CString.h"
#include <sstream>

CString::CString() 
{
    ArrayLength = 0;
    Data = new char[0];
}

CString::CString(char character) 
{ 
    ArrayLength = 1;
    Data = new char[1] { character }; 
}

CString::CString(const char* sourceString) 
{ 
    if (sourceString)
        for (ArrayLength = 0; sourceString[ArrayLength] != '\0'; ArrayLength++);

    Data = new char[ArrayLength];
    if (ArrayLength > 0)
    {
        for (unsigned int x = 0; x < ArrayLength; x++)
        {
            Data[x] = sourceString[x];
        }
    }
}

CString::CString(std::string sourceStdString) { CString(sourceStdString.c_str()); }

CString::CString(const CString &sourceString)
{
    ArrayLength = sourceString.Length();
    Data = new char[ArrayLength];
    for (unsigned int x = 0; x < ArrayLength; x++)
        Data[x] = sourceString[x];
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
    char* newStr = new char[newLength];
    for (unsigned int x = 0; x < ArrayLength; x++)
        newStr[x] = Data[x];
    for (unsigned int x = 0; x < strLength; x++)
        newStr[x + ArrayLength] = str[x];
    delete Data;
    ArrayLength = newLength;
    Data = newStr;
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
const char* CString::GetCharArray() { return Data; }
std::string CString::GetStdString() { return std::string(Data); }

std::vector<CString> CString::Split(char delimeter)
{
    std::stringstream stringStream(GetStdString());
    std::vector<CString> e;
    std::string part;
    while(std::getline(stringStream, part, delimeter))
        e.push_back(CString(part));
    return e;
}
