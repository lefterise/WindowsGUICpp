#pragma once
#include "Window.h"
#include "Font.h"

class Button {
public:

Button(Window& window, size_t id, std::string label, int x, int y, int w, int h)
: window(window)
, id(id)
{
    hwnd = CreateWindowA("BUTTON", label.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD ,
        x, y, w, h, window.hwnd, (HMENU)id, (HINSTANCE)GetWindowLongPtr(window.hwnd, GWLP_HINSTANCE), NULL);

    if (hwnd == NULL) {
        throw std::exception("Button creation failed");
    }
}

void setCommand(std::function<void(int e)>&& action) {
    window.setMenuCommand(id, std::move(action));
}

void setFont(Font& font) {
    SendMessage(hwnd, WM_SETFONT, (WPARAM)font.hFont, TRUE);
}

private:
Window& window;
size_t id;
HWND hwnd = 0;
};