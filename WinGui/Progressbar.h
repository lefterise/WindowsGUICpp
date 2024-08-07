#pragma once
#include "Window.h"
#include "Control.h"
#include "Font.h"
#include <commctrl.h>
class Progressbar {
public:

    Progressbar(Window& window, int x, int y, int w, int h)
    {
        hwnd = CreateWindowEx(
            0, PROGRESS_CLASS, NULL,
            WS_CHILD | WS_VISIBLE,
            x, y, w, h,
            window.hwnd, NULL,
            (HINSTANCE)GetWindowLongPtr(window.hwnd, GWLP_HINSTANCE), NULL
        );

        if (hwnd == NULL) {
            throw std::exception("Progressbar creation failed");
        }
    }

    void setRange(short min, short max) {
        SendMessage(hwnd, PBM_SETRANGE, 0, MAKELPARAM(min, max));        
    }

    void setValue(short value) {
        SendMessage(hwnd, PBM_SETPOS, value, 0);
    }
    
private:
    HWND hwnd = 0;
};