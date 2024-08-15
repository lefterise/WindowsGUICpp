#pragma once
#include "Window.h"
class Scrollbar {
public:

    Scrollbar(Window& window, int x, int y, int w, int h, bool isVertical = false)
    {
        hwnd = CreateWindowExW(
            0, L"SCROLLBAR", NULL,
            WS_CHILD | WS_VISIBLE | (isVertical ? SBS_VERT : SBS_HORZ),
            x, y, w, h,
            window.hwnd, NULL,
            (HINSTANCE)GetWindowLongPtrW(window.hwnd, GWLP_HINSTANCE), NULL
        );

        if (hwnd == NULL) {
            throw std::exception("Scrollbar creation failed");
        }
    }

    void setRange(int min, int max) {
        SendMessageW(hwnd, SBM_SETRANGE, min, max);
        //SetScrollRange(hwnd, SB_CTL, min, max, TRUE);        
    }

    void setPosition(int value) {
        SendMessageW(hwnd, SBM_SETPOS, value, TRUE);
    }

    int getPosition() {
        return SendMessageW(hwnd, SBM_GETPOS, 0, 0);
    }

    void setScrollHandler(Window& window, std::function<void(short notificationCode, short value)>&& action) {
        //i don't capture this because it may be moved
        auto handler = [action, hwnd(this->hwnd)](WPARAM e, LPARAM lParam) {
            if ((HWND)lParam != hwnd) return false;
            short notificationCode = LOWORD(e);
            short value = HIWORD(e);
            defaultScrollHandler(hwnd, notificationCode, value);
            action(notificationCode, value);
            return true;
        };

        LONG style = GetWindowLong(hwnd, GWL_STYLE);
        window.setMessageHandler((style & TBS_VERT) ? WM_VSCROLL : WM_HSCROLL, handler);
    }

    void setScrollInfo(int min, int max, int pageSize, int position) {
        SCROLLINFO si = { 0 };
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
        si.nPage = pageSize;
        si.nMin = min;
        si.nMax = max;
        si.nPos = position;
        SendMessageW(hwnd, SBM_SETSCROLLINFO, 0, (LPARAM)&si);
    }

    static void defaultScrollHandler(HWND hwnd, short notificationCode, short value) {
        SCROLLINFO si;
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_ALL;
        SendMessageW(hwnd, SBM_GETSCROLLINFO, 0, (LPARAM)&si);

        int scrollPos = si.nPos;

        switch (notificationCode) {
        case SB_LINEUP:      // Scroll one line up
            scrollPos = max(scrollPos - 1, si.nMin);
            break;
        case SB_LINEDOWN:    // Scroll one line down
            scrollPos = min(scrollPos + 1, si.nMax);
            break;
        case SB_PAGEUP:      // Scroll one page up
            scrollPos = max(scrollPos - si.nPage, si.nMin);
            break;
        case SB_PAGEDOWN:    // Scroll one page down
            scrollPos = min(scrollPos + si.nPage, si.nMax);
            break;
        case SB_THUMBTRACK:  // Drag thumb position
            scrollPos = value;
            break;
        }
        SendMessageW(hwnd, SBM_SETPOS, scrollPos, TRUE);
    }

    operator HWND() const {
        return hwnd;
    }
private:
    HWND hwnd = 0;
};