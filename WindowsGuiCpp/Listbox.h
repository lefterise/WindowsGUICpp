#pragma once
#include "Window.h"
#include "Control.h"

class Listbox {
public:

    Listbox(Window& window, size_t id, int x, int y, int w, int h)
        : window(window)
        , id(id)
    {
        hwnd = CreateWindowA("LISTBOX", NULL, WS_VISIBLE | WS_CHILD | LBS_STANDARD,
            x, y, w, h, window.hwnd, (HMENU)id, (HINSTANCE)GetWindowLongPtr(window.hwnd, GWLP_HINSTANCE), NULL);

        if (hwnd == NULL) {
            throw std::exception("Listbox creation failed");
        }
    }

    void addItem(const char* item) {
        SendMessageA(hwnd, LB_ADDSTRING, 0, (LPARAM)item);
    }

    void setCommand(std::function<void(int e)>&& action) {
        window.setMenuCommand(id, std::move(action));
    }

    long getSelectedItem() {
        long selIndex = SendMessageA(hwnd, LB_GETCURSEL, 0, 0);
        return selIndex;
    }

    std::string getItem(int index) {
        std::string str;
        long length = SendMessageA(hwnd, LB_GETTEXTLEN, index, NULL);
        if (length == LB_ERR) throw std::exception("Out of bounds");
        str.resize(length);
        SendMessageA(hwnd, LB_GETTEXT, index, (LPARAM)str.data());
        return str;
    }
private:
    Window& window;
    size_t id;
    HWND hwnd = 0;
};