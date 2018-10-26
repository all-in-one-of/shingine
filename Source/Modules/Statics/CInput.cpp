#include "Modules/Statics/CInput.h"

CInput* CInput::Instance = NULL;
CInput::CInput()
{
    for (int x = 0; x < 255; x++)
    {
        Keys[x] = -1;
    }
}

  
bool CInput::GetMousePressed(int keyCode)
{
    KeyMap::iterator it = MouseKeys.find(keyCode);
    if (it == MouseKeys.end()) return false;
    int action = it->second;

    if (action == S_INPUT_ACTION_PRESSED)
        return true;

    return false;
}  

bool CInput::GetKeyPressed(int keyCode)
{
    KeyMap::iterator it = Keys.find(keyCode);
    if (it == Keys.end()) return false;
    int action = it->second;

    if (action == S_INPUT_ACTION_PRESSED || action == S_INPUT_ACTION_REPEAT)
        return true;

    return false;
}

void CInput::Update()
{
    
    MousePositionX = MousePrevX;
    MousePositionY = MousePrevY;
}

void CInput::SetKeyEvent(int key, int scanCode, int action, int mods)
{
    Keys[key] = action;
}

float CInput::GetAxis(AxisType axis) 
{
    switch (axis)
    {
        case AxisType::MouseX:
            return (MousePositionX - MousePrevX) / 1280;
        case AxisType::MouseY:
            return (MousePositionY - MousePrevY) / 720;
        default:
            return .0;
    };
}

void CInput::SetMousePosition(double x, double y)
{

    MousePrevX = MousePositionX;
    MousePrevY = MousePositionY;
    MousePositionX = x;
    MousePositionY = y;
}

void CInput::SetMouseEvent(int key, int action, int mods)
{
    MouseKeys[key] = action;
}
