#pragma once
#include <unordered_map>

#define S_INPUT_ACTION_PRESSED 1
#define S_INPUT_ACTION_REPEAT 2

#define S_INPUT_KEY_A 65
#define S_INPUT_KEY_D 68
#define S_INPUT_KEY_E 69
#define S_INPUT_KEY_Q 81
#define S_INPUT_KEY_S 83
#define S_INPUT_KEY_W 87

#define S_INPUT_MOUSE_LEFT 0
#define S_INPUT_MOUSE_RIGHT 1

class CInput
{
public:
    enum AxisType 
    {
        MouseX, MouseY
    };

    static CInput* Get() 
    {
        if (!Instance) 
            Instance = new CInput();
        return Instance;
    };
    
    void Update();
    void SetKeyEvent(int key, int scanCode, int action, int mods);
    void SetMousePosition(double x, double y);
    void SetMouseEvent(int key, int action, int mods);

    float GetAxis(AxisType axis);

    bool GetKeyPressed(int keyCode);
    bool GetMousePressed(int keyCode);
    
private: 
    CInput();
    static CInput* Instance;

    typedef std::unordered_map<int, int> KeyMap;

    KeyMap Keys;
    KeyMap MouseKeys;

    int KeyScanCode = -1;
    int KeyMods = -1;

    double MousePositionX = .0;
    double MousePositionY = .0;

    double MousePrevX = .0;
    double MousePrevY = .0;

    int MouseMods = -1;
};
