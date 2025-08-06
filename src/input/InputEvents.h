
#pragma once
#include <glm/glm.hpp>


enum class MouseButton { None, Left, Right, Middle };
enum class KeyboardModifier { None = 0, Shift = 1, Control = 2, Alt = 4 };

// Equality operator for MouseButton
inline bool operator==(MouseButton a, MouseButton b) {
    return static_cast<int>(a) == static_cast<int>(b);
}
// Bitwise operators for KeyboardModifier
inline KeyboardModifier operator|(KeyboardModifier a, KeyboardModifier b) {
    return static_cast<KeyboardModifier>(static_cast<int>(a) | static_cast<int>(b));
}
inline KeyboardModifier operator&(KeyboardModifier a, KeyboardModifier b) {
    return static_cast<KeyboardModifier>(static_cast<int>(a) & static_cast<int>(b));
}
inline KeyboardModifier& operator|=(KeyboardModifier& a, KeyboardModifier b) {
    a = a | b;
    return a;
}
inline KeyboardModifier& operator&=(KeyboardModifier& a, KeyboardModifier b) {
    a = a & b;
    return a;
}
inline KeyboardModifier operator~(KeyboardModifier a) {
    return static_cast<KeyboardModifier>(~static_cast<int>(a));
}

// Navigation modes for camera controllers
enum class NavigationMode {
    None,
    Maya,        // Maya-style: Alt+LMB=orbit, Alt+MMB=pan, Alt+RMB=zoom
    Blender,     // Blender-style: MMB=orbit, Shift+MMB=pan, Ctrl+MMB=zoom
    CAD,         // CAD-style: RMB=orbit, MMB=pan, wheel=zoom
    FPS          // FPS-style: WASD movement, mouse look
};

// Navigation actions for camera controllers
enum class NavigationAction {
    None,
    Orbit,
    Pan,
    Zoom
};

struct MouseEvent {
    MouseButton button;
    KeyboardModifier modifiers;
    glm::ivec2 pos;
};

struct WheelEvent {
    float delta;
    KeyboardModifier modifiers;
};

struct KeyEvent {
    int key;
    KeyboardModifier modifiers;
};
