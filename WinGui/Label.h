#pragma once
#include "Window.h"
#include "Font.h"

class Label {
public:
    Label(Window& window, const std::string& text, int x, int y, int w, int h)
    : id(window.getNextId())
    {
        hwnd = CreateWindowA("STATIC", text.c_str(), WS_VISIBLE | WS_CHILD | ES_LEFT,
            x, y, w, h, window.hwnd, (HMENU)id, (HINSTANCE)GetWindowLongPtr(window.hwnd, GWLP_HINSTANCE), NULL);

        if (hwnd == NULL) {
            throw std::exception("Label creation failed");
        }
    }

    void setCommand(Window& window, std::function<void(int e)>&& action) {
        window.setMenuCommand(id, std::move(action));
    }

    void setFont(Font& font) {
        SendMessage(hwnd, WM_SETFONT, (WPARAM)font.hFont, TRUE);
    }

    void setText(const char* text) {
        SetWindowTextA(hwnd, text);
    }

private:
    size_t id;
    HWND hwnd = 0;
};