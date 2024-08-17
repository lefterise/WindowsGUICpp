#pragma once
#include "Window.h"
#include "Font.h"
#include <commctrl.h>
class TabControl {
public:
    TabControl(Window& window, int x, int y, int w, int h) {
        hwnd = CreateWindowW(WC_TABCONTROL, NULL,
            WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
            x, y, w, h, window.hwnd, NULL, (HINSTANCE)GetWindowLongPtrW(window.hwnd, GWLP_HINSTANCE), NULL);
        
        TCITEMW tie;
        tie.mask = TCIF_TEXT;
        tie.pszText = (wchar_t*)L"Tab 1";
        SendMessageW(hwnd, TCM_INSERTITEMW, 0, (LPARAM)&tie);

        tie.pszText = (wchar_t*)L"Tab 2";
        SendMessageW(hwnd, TCM_INSERTITEMW, 1, (LPARAM)&tie);
    }

    void setVisible(bool visible) {
        ShowWindow(hwnd, visible ? SW_SHOW : SW_HIDE);
    }
    
    void setTabChangeListener(Window& window, std::function<void()>&& callback) {
        window.setMessageHandler(WM_NOTIFY,
            [hwnd(this->hwnd), callback(std::move(callback))](WPARAM w, LPARAM l) -> bool {
                NMHDR* header = (NMHDR*)l;
                if (header->hwndFrom != hwnd) return false;
                if (header->code == TCN_SELCHANGE) {
                    callback();
                    return true;
                }
                return false;
            }
        );
    }

    int getCurrentTab() {
        return (int)SendMessage(hwnd, TCM_GETCURSEL, 0, 0);
    }

    void setFont(Font& font) {
        SendMessageW(hwnd, WM_SETFONT, (WPARAM)font.hFont, TRUE);
    }

    operator HWND() const {
        return hwnd;
    }

    HWND hwnd;
};