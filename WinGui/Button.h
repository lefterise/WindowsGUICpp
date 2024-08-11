#pragma once
#include "Window.h"
#include "Font.h"

class Button {
public:

Button(Window& window, const std::string& label, int x, int y, int w, int h)
{
    hwnd = CreateWindowA("BUTTON", label.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD ,
        x, y, w, h, window.hwnd, (HMENU)window.getNextId(), (HINSTANCE)GetWindowLongPtr(window.hwnd, GWLP_HINSTANCE), NULL);

    if (hwnd == NULL) {
        throw std::exception("Button creation failed");
    }
}

void setCommand(Window& window, std::function<void(int e)>&& action) {
    int id = GetDlgCtrlID(hwnd);
    window.setMenuCommand(id, std::move(action));
}

void setFont(Font& font) {
    SendMessage(hwnd, WM_SETFONT, (WPARAM)font.hFont, TRUE);
}

void setText(const char* text) {
    SetWindowTextA(hwnd, text);
}

std::string getText() {
    int length = GetWindowTextLength(hwnd);
    std::string text;
    text.resize(length);
    GetWindowTextA(hwnd, &text[0], text.length());
    return text;
}

operator HWND() const {
    return hwnd;
}

private:
HWND hwnd = 0;
};