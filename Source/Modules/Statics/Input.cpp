#include "Modules/Statics/Input.h"
#include <iostream>

REGISTER_SERIALIZED_NAME(Input)

Input::Input()
{
    for (int x = 0; x < 255; x++)
    {
        Keys[x] = -1;
    }
}

void Input::SetScreenReferenceSize(unsigned int width, unsigned int height)
{
    ScreenWidth = width;
    ScreenHeight = height;

    if (!FirstMousePositionRecorded)
    {
        MousePositionX = MousePrevX = ScreenWidth * .5f;
        MousePositionY = MousePrevX = ScreenHeight * .5f;

        FirstMousePositionRecorded = true;
    }
}

bool Input::GetMousePressed(int keyCode)
{
    KeyMap::iterator it = MouseKeys.find(keyCode);
    if (it == MouseKeys.end()) return false;
    int action = it->second;

    if (action == S_INPUT_ACTION_PRESSED)
        return true;

    return false;
}  

bool Input::GetKeyPressed(int keyCode)
{
    KeyMap::iterator it = Keys.find(keyCode);
    if (it == Keys.end()) return false;
    int action = it->second;

    if (action == S_INPUT_ACTION_PRESSED || action == S_INPUT_ACTION_REPEAT)
        return true;

    return false;
}

void Input::Update()
{
    // MousePositionX = MousePrevX;
    // MousePositionY = MousePrevY;
}

void Input::SetKeyEvent(int key, int scanCode, int action, int mods)
{
    Keys[key] = action;
}

float Input::GetAxis(AxisType axis) 
{
    switch (axis)
    {
        case AxisType::MouseX:
            return (MousePositionX - MousePrevX);
        case AxisType::MouseY:
            return (MousePositionY - MousePrevY);
        default:
            return .0f;
    };
}

void Input::SetMousePosition(double x, double y)
{
    MousePrevX = MousePositionX;
    MousePrevY = MousePositionY;
    MousePositionX = x;
    MousePositionY = y;
}

void Input::SetMouseEvent(int key, int action, int mods)
{
    MouseKeys[key] = action;
}
