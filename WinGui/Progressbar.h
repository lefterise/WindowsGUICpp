#pragma once
#include "Window.h"
#include "Control.h"
#include "Font.h"
#include <commctrl.h>
class Progressbar {
public:

    Progressbar(Window& window, int x, int y, int w, int h)
    {
        hwnd = CreateWindowExW(
            0, PROGRESS_CLASSW, NULL,
            WS_CHILD | WS_VISIBLE,
            x, y, w, h,
            window.hwnd, NULL,
            (HINSTANCE)GetWindowLongPtrW(window.hwnd, GWLP_HINSTANCE), NULL
        );

        if (hwnd == NULL) {
            throw std::exception("Progressbar creation failed");
        }
    }

    void setRange(short min, short max) {
        SendMessageW(hwnd, PBM_SETRANGE, 0, MAKELPARAM(min, max));        
    }

    void setValue(short value) {
        SendMessageW(hwnd, PBM_SETPOS, value, 0);
    }

    operator HWND() const {
        return hwnd;
    }
    
private:
    HWND hwnd = 0;
};