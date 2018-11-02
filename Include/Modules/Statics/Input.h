#pragma once
#include "IInput.h"
#include "Utility/Data/Serialization.h"

class Input : public IInput, public ISerializedClass {
public:
  SERIALIZE_CLASS(Input);
  Input();
  virtual ~Input() {}
  virtual void Update();
  virtual void SetKeyEvent(int key, int scanCode, int action, int mods);
  virtual void SetMousePosition(double x, double y);
  virtual void SetMouseEvent(int key, int action, int mods);

  virtual float GetAxis(AxisType axis);

  virtual bool GetKeyPressed(int keyCode);
  virtual bool GetKeyUp(int keyCode);
  virtual bool GetMousePressed(int keyCode);

  virtual void SetScreenReferenceSize(unsigned int width, unsigned int height);

private:
  int GetKeyAction(int keyCode);
  typedef std::unordered_map<int, int> KeyMap;

  KeyMap Keys;
  KeyMap MouseKeys;

  double MousePositionX = .0;
  double MousePositionY = .0;

  double MousePrevX = .0;
  double MousePrevY = .0;

  unsigned int ScreenWidth = 320;
  unsigned int ScreenHeight = 240;

  bool FirstMousePositionRecorded = false;
};
