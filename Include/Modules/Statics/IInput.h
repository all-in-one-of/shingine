#pragma once
#include <unordered_map>
#include "Statics.h"


#define S_INPUT_ACTION_RELEASE 0
#define S_INPUT_ACTION_PRESSED 1
#define S_INPUT_ACTION_REPEAT 2
#define S_INPUT_ACTION_KEY_UP 3

#define S_INPUT_KEY_EQUAL 61
#define S_INPUT_KEY_A 65
#define S_INPUT_KEY_D 68
#define S_INPUT_KEY_E 69
#define S_INPUT_KEY_Q 81
#define S_INPUT_KEY_S 83
#define S_INPUT_KEY_W 87

#define S_INPUT_MOUSE_LEFT 0
#define S_INPUT_MOUSE_RIGHT 1

class IInput {
public:
  enum AxisType { MouseX, MouseY };
  virtual ~IInput() {}
  virtual void Update() = 0;
  virtual void SetKeyEvent(int key, int scanCode, int action, int mods) = 0;
  virtual void SetMousePosition(double x, double y) = 0;
  virtual void SetMouseEvent(int key, int action, int mods) = 0;

  virtual float GetAxis(AxisType axis) = 0;

  virtual bool GetKeyPressed(int keyCode) = 0;
  virtual bool GetKeyUp(int keyCode) = 0;
  virtual bool GetMousePressed(int keyCode) = 0;

  virtual void SetScreenReferenceSize(unsigned int width,
                                      unsigned int height) = 0;
};
