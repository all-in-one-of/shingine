#pragma once
class CString;
class IRenderContext
{
public:
    virtual ~IRenderContext() {};
    virtual void Create(unsigned short width, unsigned short height, const CString& title) = 0;
    virtual void UpdateFrame() = 0;
};
