#pragma once
#include "Window.h"
#include <commctrl.h>
class Slider {
public:

    Slider(Window& window, int x, int y, int w, int h, bool isVertical = false)
    {
        hwnd = CreateWindowEx(
            0, TRACKBAR_CLASS, NULL,
            WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_BOTH | (isVertical ? TBS_VERT : TBS_HORZ),
            x, y, w, h,
            window.hwnd, NULL,
            (HINSTANCE)GetWindowLongPtr(window.hwnd, GWLP_HINSTANCE), NULL
        );

        if (hwnd == NULL) {
            throw std::exception("Slider creation failed");
        }
    }

    void setRange(short min, short max) {
        SendMessage(hwnd, TBM_SETRANGE, 0, MAKELPARAM(min, max));
    }

    void setValue(short value) {
        SendMessage(hwnd, TBM_SETPOS, TRUE, value);
    }

    short getValue() {
        return SendMessage(hwnd, TBM_GETPOS, 0, 0);
    }

    void setMessageHandler(Window& window, UINT message, std::function<void(WPARAM e)>&& action) {
        window.setMessageHandler(hwnd, message, std::move(action));
    }

private:
    HWND hwnd = 0;
};