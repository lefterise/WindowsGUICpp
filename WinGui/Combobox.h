#pragma once
#include "Window.h"
#include "Font.h"

class Combobox {
public:

    Combobox(Window& window, int x, int y, int w, int h)
    {
        hwnd = CreateWindowExA(0, "COMBOBOX", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWN,
            x, y, w, h, window.hwnd, (HMENU)window.getNextId(), (HINSTANCE)GetWindowLongPtr(window.hwnd, GWLP_HINSTANCE), NULL);

        if (hwnd == NULL) {
            throw std::exception("Combobox creation failed");
        }
    }

    void setCommand(Window& window, std::function<void(int e)>&& action) {
        int id = GetDlgCtrlID(hwnd);
        window.setMenuCommand(id, std::move(action));
    }

    void addItem(const char* item) {
        SendMessageA(hwnd, CB_ADDSTRING, 0, (LPARAM)item);
    }

    long getSelectedIndex() {
        long selIndex = SendMessageA(hwnd, CB_GETCURSEL, 0, 0);
        return selIndex;
    }

    void setSelectedIndex(int index) {
        SendMessageA(hwnd, CB_SETCURSEL, index, 0);        
    }

    std::string getItem(int index) {
        std::string str;
        long length = SendMessageA(hwnd, CB_GETLBTEXTLEN, index, NULL);
        if (length == CB_ERR) throw std::exception("Out of bounds");
        str.resize(length);
        SendMessageA(hwnd, CB_GETLBTEXT, index, (LPARAM)str.data());
        return str;
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