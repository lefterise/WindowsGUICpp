#pragma once
#include "Window.h"
class Scrollbar {
public:

    Scrollbar(Window& window, int x, int y, int w, int h, bool isVertical = false)
    {
        hwnd = CreateWindowExA(
            0, "SCROLLBAR", NULL,
            WS_CHILD | WS_VISIBLE | (isVertical ? SBS_VERT : SBS_HORZ),
            x, y, w, h,
            window.hwnd, NULL,
            (HINSTANCE)GetWindowLongPtr(window.hwnd, GWLP_HINSTANCE), NULL
        );

        if (hwnd == NULL) {
            throw std::exception("Scrollbar creation failed");
        }
    }

    void setRange(int min, int max) {
        SendMessage(hwnd, SBM_SETRANGE, min, max);
        //SetScrollRange(hwnd, SB_CTL, min, max, TRUE);        
    }

    void setValue(int value) {
        SendMessage(hwnd, SBM_SETPOS, value, TRUE);
    }

    int getValue() {
        return SendMessage(hwnd, SBM_GETPOS, 0, 0);
    }

    void setMessageHandler(Window& window, UINT message, std::function<void(WPARAM e)>&& action) {
        window.setMessageHandler(hwnd, message, std::move(action));
    }

    void setScrollInfo(int min, int max, int pageSize, int position) {
        SCROLLINFO si = { 0 };
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
        si.nPage = pageSize;
        si.nMin = min;
        si.nMax = max;
        si.nPos = position;
        this->pageSize = pageSize;
        SendMessage(hwnd, SBM_SETSCROLLINFO, 0, (LPARAM)&si);
    }

    void defaultScrollHandler(WPARAM wParam) {
        int action = LOWORD(wParam);
        int pos = HIWORD(wParam);

        int scrollPos = getValue();
        switch (action) {
        case SB_LINEUP:      // Scroll one line up
            scrollPos = max(scrollPos - 1, 0);
            break;
        case SB_LINEDOWN:    // Scroll one line down
            scrollPos = min(scrollPos + 1, 100);
            break;
        case SB_PAGEUP:      // Scroll one page up
            scrollPos = max(scrollPos - pageSize, 0);
            break;
        case SB_PAGEDOWN:    // Scroll one page down
            scrollPos = min(scrollPos + pageSize, 100);
            break;
        case SB_THUMBTRACK:  // Drag thumb position
            scrollPos = pos;
            break;
        }
        setValue(scrollPos);
    }
private:
    HWND hwnd = 0;
    int pageSize = 10;
};