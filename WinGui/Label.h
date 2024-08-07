#pragma once
#include "Window.h"
#include "Font.h"

class Label {
public:
    Label(Window& window, size_t id, std::string text, int x, int y, int w, int h)
        : window(window)
        , id(id)
    {
        hwnd = CreateWindowA("STATIC", text.c_str(), WS_VISIBLE | WS_CHILD | ES_LEFT,
            x, y, w, h, window.hwnd, (HMENU)id, (HINSTANCE)GetWindowLongPtr(window.hwnd, GWLP_HINSTANCE), NULL);

        if (hwnd == NULL) {
            throw std::exception("Label creation failed");
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