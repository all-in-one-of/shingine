#include "Modules/Statics/Input.h"
#include <iostream>

REGISTER_SERIALIZED_CLASS(Input)

Input::Input() {
  for (int x = 0; x < 255; x++) {
    Keys[x] = -1;
  }
}

void Input::SetScreenReferenceSize(unsigned int width, unsigned int height) {
  ScreenWidth = width;
  ScreenHeight = height;

  if (!FirstMousePositionRecorded) {
    MousePositionX = MousePrevX = ScreenWidth * .5f;
    MousePositionY = MousePrevX = ScreenHeight * .5f;

    FirstMousePositionRecorded = true;
  }
}

bool Input::GetMousePressed(int keyCode) {
  KeyMap::iterator it = MouseKeys.find(keyCode);
  if (it == MouseKeys.end())
    return false;
  int action = it->second;

  return action == S_INPUT_ACTION_PRESSED;
}

int Input::GetKeyAction(int keyCode) {
  KeyMap::iterator it = Keys.find(keyCode);
  if (it == Keys.end())
    return -1;
  int action = it->second;
  if (action == S_INPUT_ACTION_KEY_UP)
    Keys[keyCode] = S_INPUT_ACTION_RELEASE;
  return action;
}

bool Input::GetKeyPressed(int keyCode) {
  int action = GetKeyAction(keyCode);
  if (action == -1)
    return false;

  return action == S_INPUT_ACTION_PRESSED || action == S_INPUT_ACTION_REPEAT;
}

bool Input::GetKeyUp(int keyCode) {
  int action = GetKeyAction(keyCode);
  if (action == -1)
    return false;
  return action == S_INPUT_ACTION_KEY_UP;
}

void Input::Update() {}

void Input::SetKeyEvent(int key, int scanCode, int action, int mods) {
  int keyAction = Keys[key];
  // track the key up event here
  if ((keyAction == S_INPUT_ACTION_PRESSED ||
       keyAction == S_INPUT_ACTION_REPEAT) &&
      action == S_INPUT_ACTION_RELEASE)
    Keys[key] = S_INPUT_ACTION_KEY_UP;
  else
    Keys[key] = action;
}

float Input::GetAxis(AxisType axis) {
  switch (axis) {
  case AxisType::MouseX:
    return (MousePositionX - MousePrevX);
  case AxisType::MouseY:
    return (MousePositionY - MousePrevY);
  default:
    return .0f;
  };
}

void Input::SetMousePosition(double x, double y) {
  MousePrevX = MousePositionX;
  MousePrevY = MousePositionY;
  MousePositionX = x;
  MousePositionY = y;
}

void Input::SetMouseEvent(int key, int action, int mods) {
  MouseKeys[key] = action;
}
