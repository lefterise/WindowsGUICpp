#pragma once
#include "Window.h"
#include "Font.h"

class Textbox {
public:

    Textbox(Window& window, const wchar_t* text, int x, int y, int w, int h)
    {
        hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", text, WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
            x, y, w, h, window.hwnd, (HMENU)window.getNextId(), (HINSTANCE)GetWindowLongPtrW(window.hwnd, GWLP_HINSTANCE), NULL);

        if (hwnd == NULL) {
            throw std::exception("Textbox creation failed");
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