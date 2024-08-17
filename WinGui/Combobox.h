#pragma once
#include "Window.h"
#include "Font.h"

class Combobox {
public:

    Combobox(Window& window, int x, int y, int w, int h)
    {
        hwnd = CreateWindowExW(0, L"COMBOBOX", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWN,
            x, y, w, h, window.hwnd, (HMENU)window.getNextId(), (HINSTANCE)GetWindowLongPtrW(window.hwnd, GWLP_HINSTANCE), NULL);

        if (hwnd == NULL) {
            throw std::exception("Combobox creation failed");
        }
    }

    void setCommand(Window& window, std::function<void(int e)>&& action) {
        int id = GetDlgCtrlID(hwnd);
        window.setMenuCommand(id, std::move(action));
    }

    void addItem(const wchar_t* item) {
        SendMessageW(hwnd, CB_ADDSTRING, 0, (LPARAM)item);
    }

    long getSelectedIndex() {
        long selIndex = SendMessageW(hwnd, CB_GETCURSEL, 0, 0);
        return selIndex;
    }

    void setSelectedIndex(int index) {
        SendMessageW(hwnd, CB_SETCURSEL, index, 0);        
    }

    std::wstring getItem(int index) {
        std::wstring str;
        long length = SendMessageW(hwnd, CB_GETLBTEXTLEN, index, NULL);
        if (length == CB_ERR) throw std::exception("Out of bounds");
        str.resize(length);
        SendMessageW(hwnd, CB_GETLBTEXT, index, (LPARAM)str.data());
        return str;
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