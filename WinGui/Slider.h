#pragma once
#include "Window.h"
#include <commctrl.h>
class Slider {
public:

    Slider(Window& window, int x, int y, int w, int h, bool isVertical = false)
    {
        hwnd = CreateWindowExW(
            0, TRACKBAR_CLASSW, NULL,
            WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_BOTH | (isVertical ? TBS_VERT : TBS_HORZ),
            x, y, w, h,
            window.hwnd, NULL,
            (HINSTANCE)GetWindowLongPtrW(window.hwnd, GWLP_HINSTANCE), NULL
        );

        if (hwnd == NULL) {
            throw std::exception("Slider creation failed");
        }
    }

    void setRange(short min, short max) {
        SendMessageW(hwnd, TBM_SETRANGE, 0, MAKELPARAM(min, max));
    }

    void setPosition(short value) {
        SendMessageW(hwnd, TBM_SETPOS, TRUE, value);
    }

    short getPosition() {
        return SendMessageW(hwnd, TBM_GETPOS, 0, 0);
    }

    void setScrollHandler(Window& window, std::function<void(short notificationCode, short value)>&& action) {        
        auto handler = [action, hwnd(this->hwnd)](WPARAM e, LPARAM lParam) {
            if ((HWND)lParam != hwnd) return false;
            short notificationCode = LOWORD(e); //eg TB_THUMBTRACK
            short value = HIWORD(e);
            action(notificationCode, value);
            return true;
        };

        LONG style = GetWindowLongW(hwnd, GWL_STYLE);
        window.setMessageHandler((style & TBS_VERT) ? WM_VSCROLL : WM_HSCROLL, handler);       
    }

    operator HWND() const {
        return hwnd;
    }

private:
    HWND hwnd = 0;
};