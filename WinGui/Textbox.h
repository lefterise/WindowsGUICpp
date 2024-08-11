#pragma once
#include "Window.h"
#include "Font.h"

class Textbox {
public:

    Textbox(Window& window, std::string text, int x, int y, int w, int h)
    {
        hwnd = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", text.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
            x, y, w, h, window.hwnd, (HMENU)window.getNextId(), (HINSTANCE)GetWindowLongPtr(window.hwnd, GWLP_HINSTANCE), NULL);

        if (hwnd == NULL) {
            throw std::exception("Textbox creation failed");
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

    operator HWND() const {
        return hwnd;
    }

private:
    HWND hwnd = 0;
};