#pragma once
#include "Window.h"
#include "Font.h"

class Button {
public:

Button(Window& window, const wchar_t* label, int x, int y, int w, int h)
{
    hwnd = CreateWindowW(L"BUTTON", label, WS_TABSTOP | WS_VISIBLE | WS_CHILD ,
        x, y, w, h, window.hwnd, (HMENU)window.getNextId(), (HINSTANCE)GetWindowLongPtrW(window.hwnd, GWLP_HINSTANCE), NULL);

    if (hwnd == NULL) {
        throw std::exception("Button creation failed");
    }
}

void setCommand(Window& window, std::function<void(int e)>&& action) {
    int id = GetDlgCtrlID(hwnd);
    window.setMenuCommand(id, std::move(action));
}

void setFont(Font& font) {
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)font.hFont, TRUE);
}

void setText(const wchar_t* text) {
    SetWindowTextW(hwnd, text);
}

std::wstring getText() {
    int length = GetWindowTextLengthW(hwnd);
    std::wstring text;
    text.resize(length);
    GetWindowTextW(hwnd, &text[0], text.length());
    return text;
}

void setVisible(bool visible) {
    ShowWindow(hwnd, visible ? SW_SHOW : SW_HIDE);
}

operator HWND() const {
    return hwnd;
}

private:
HWND hwnd = 0;
};