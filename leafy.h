
#pragma once
#include "raylib.h"
#include <stack>
#include <string>

namespace leafy {

// --- Motyw i kontekst ---
struct Theme {
    Color bg;
    Color panel;
    Color accent;
    Color accentHover;
    Color text;
};
extern Theme theme;

// --- Layout ---
enum LayoutType { NONE, VERTICAL, HORIZONTAL };

struct Layout {
    Rectangle bounds;
    LayoutType type;
    float spacing;
    Vector2 cursor;
    int alignment;
};
extern std::stack<Layout> layoutStack;

// --- Layout API ---
void BeginVertical(Rectangle bounds, float spacing = 8, int alignment = 0);
void BeginHorizontal(Rectangle bounds, float spacing = 8, int alignment = 0);
void EndLayout();
Rectangle Next(Rectangle elementSize);

// --- Helper centrowania ---
Rectangle CenteredRect(float width, float height);
void BeginCenter(float width, float height, float spacing = 12);

// --- Domyślne wymiary komponentów ---
constexpr float DEFAULT_WIDTH = 180;
constexpr float DEFAULT_HEIGHT = 40;

// --- Komponenty ---
bool Button(const char* text);
bool TextInput(std::string &value, int maxLength = 32);

} // namespace leafy
