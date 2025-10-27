
#include "leafy.h"
#include <iostream>

namespace leafy {

// --- Motyw ---
Theme theme = {
    {25,25,30,255},     // bg
    {35,35,40,255},     // panel
    {40,120,220,255},   // accent
    {60,150,250,255},   // accentHover
    {230,230,230,255}   // text
};

// --- Layout ---
std::stack<Layout> layoutStack;

void BeginVertical(Rectangle bounds, float spacing, int alignment) {
    Layout l{bounds, VERTICAL, spacing, {bounds.x, bounds.y}, alignment};
    layoutStack.push(l);
}

void BeginHorizontal(Rectangle bounds, float spacing, int alignment) {
    Layout l{bounds, HORIZONTAL, spacing, {bounds.x, bounds.y}, alignment};
    layoutStack.push(l);
}

void EndLayout() { if(!layoutStack.empty()) layoutStack.pop(); }

Rectangle Next(Rectangle elementSize) {
    if(layoutStack.empty()) return elementSize;
    Layout &l = layoutStack.top();
    Rectangle r = elementSize;

    if(l.type == VERTICAL) {
        if(l.alignment==1) r.x = l.bounds.x + (l.bounds.width - r.width)/2;
        else if(l.alignment==2) r.x = l.bounds.x + l.bounds.width - r.width;
        else r.x = l.bounds.x;
        r.y = l.cursor.y;
        l.cursor.y += r.height + l.spacing;
    } else if(l.type == HORIZONTAL) {
        if(l.alignment==1) r.y = l.bounds.y + (l.bounds.height - r.height)/2;
        else if(l.alignment==2) r.y = l.bounds.y + l.bounds.height - r.height;
        else r.y = l.bounds.y;
        r.x = l.cursor.x;
        l.cursor.x += r.width + l.spacing;
    }

    return r;
}

// --- Helper centrowania ---
Rectangle CenteredRect(float width, float height) {
    return { GetScreenWidth()/2.0f - width/2.0f, GetScreenHeight()/2.0f - height/2.0f, width, height };
}

void BeginCenter(float width, float height, float spacing) {
    Rectangle r = CenteredRect(width, height);
    BeginVertical(r, spacing, 1); // wycentrowane poziomo
}

// --- Button ---
bool Button(const char* text) {
    Rectangle r{0,0,DEFAULT_WIDTH, DEFAULT_HEIGHT};
    if(!layoutStack.empty()) r = Next(r);

    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, r);
    bool pressed = hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    Color bg = hovered ? theme.accentHover : theme.accent;

    DrawRectangleRounded(r, 0.3f, 8, bg);
    int textWidth = MeasureText(text, 20);
    DrawText(text, r.x + r.width/2 - textWidth/2, r.y + r.height/2 - 10, 20, theme.text);

    return pressed;
}

// --- TextInput ---
bool TextInput(std::string &value, int maxLength) {
    Rectangle r{0,0,DEFAULT_WIDTH, DEFAULT_HEIGHT};
    if(!layoutStack.empty()) r = Next(r);

    DrawRectangleRounded(r, 0.3f, 8, theme.panel);
    DrawRectangleLines(r.x,r.y,r.width,r.height, theme.accent);

    int key = GetCharPressed();
    while(key > 0) {
        if(value.size() < maxLength) value += (char)key;
        key = GetCharPressed();
    }
    if(IsKeyPressed(KEY_BACKSPACE) && !value.empty()) value.pop_back();

    DrawText(value.c_str(), r.x+6, r.y + r.height/2 - 10, 20, theme.text);

    return IsKeyPressed(KEY_ENTER);
}

} // namespace leafy
