#pragma once
#include "Window.h"
#include "Font.h"
#include <commctrl.h>

class Toolbar {
public:
    class ButtonBuilder {
    public:
        enum class Style : BYTE {
            Button = BTNS_BUTTON,
            Check = BTNS_CHECK,
            ButtonGroup = BTNS_CHECKGROUP
        };

        struct Button {
            int imageId;
            BYTE style;
            std::string caption;
            std::function<void(int e)> action;
            std::function<void(int x, int y)> menu;
        };

        ButtonBuilder(HWND hwnd, Window& window)
            : hwnd(hwnd)
            , window(window)
        {}

        ButtonBuilder& addButton(int imageId, Style style, std::string caption, std::function<void(int e)>&& action) {
            buttons.push_back({ imageId, (BYTE)style, caption, std::move(action), nullptr });
            return *this;
        }

        ButtonBuilder& addSeparator() {
            buttons.push_back({ 0, BTNS_SEP, "", nullptr });
            return *this;
        }

        ButtonBuilder& addDropdownMenu(int imageId, std::string caption, std::function<void(int x, int y)> menuCallback) {
            buttons.push_back({ imageId, BTNS_DROPDOWN, caption, nullptr, std::move(menuCallback) });
            return *this;
        }

        void build() {
            TBBUTTON* btns = (TBBUTTON*)malloc(sizeof(TBBUTTON) * buttons.size());
            int i = 0;
            for (auto& button : buttons) {
                btns[i].iBitmap = button.imageId;
                btns[i].idCommand = window.getNextId();
                btns[i].fsState = TBSTATE_ENABLED;
                btns[i].fsStyle = button.style;
                btns[i].iString = (INT_PTR)button.caption.c_str();
                if (button.menu != nullptr) {
                    window.setMessageHandler(WM_NOTIFY, 
                        [hwnd(this->hwnd), menuCallback(std::move(button.menu)), buttonCommandId(btns[i].idCommand)](WPARAM w, LPARAM l) -> bool {
                            NMHDR* header = (NMHDR*)l;
                            if (header->hwndFrom != hwnd) return false; //event did not originate from toolbar
                            if (header->code == TBN_DROPDOWN) {
                                NMTOOLBARW* dropdown = (NMTOOLBARW*)l;
                                if (dropdown->iItem != buttonCommandId) return false; //event is about another button
                                RECT rc;
                                SendMessage(dropdown->hdr.hwndFrom, TB_GETRECT, (WPARAM)dropdown->iItem, (LPARAM)&rc);
                                MapWindowPoints(dropdown->hdr.hwndFrom, HWND_DESKTOP, (LPPOINT)&rc, 2);

                                menuCallback(rc.left, rc.bottom);
                                return true;
                            }
                            return false;
                        }
                    );
                }
                else {
                    window.setMenuCommand(btns[i].idCommand, std::move(button.action));
                }
                i++;
            }

            SendMessage(hwnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
            SendMessage(hwnd, TB_ADDBUTTONSA, (WPARAM)buttons.size(), (LPARAM)btns);//todo use W for unicode support
            free(btns);

            SendMessage(hwnd, TB_AUTOSIZE, 0, 0);
        }

    private:
        std::vector<Button> buttons;
        HWND hwnd;
        Window& window;
    };


    Toolbar(Window& window, int x, int y, int w, int h)
    {

        hwnd = CreateWindowExA(
            0, TOOLBARCLASSNAMEA, NULL,
            WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS,
            x, y, w, h, window.hwnd, (HMENU)window.getNextId(),
            (HINSTANCE)GetWindowLongPtr(window.hwnd, GWLP_HINSTANCE),
            NULL
        );

        if (hwnd == NULL) {
            throw std::exception("Toolbar creation failed");
        }
    }

    ButtonBuilder buttonBuilder(Window& window) {
        ButtonBuilder builder{ hwnd, window };
        return builder;
    }

    void setFont(Font& font) {
        SendMessage(hwnd, WM_SETFONT, (WPARAM)font.hFont, TRUE);
    }

    void setText(const char* text) {
        SetWindowTextA(hwnd, text);
    }

    int getButtonCount() const {
        return SendMessage(hwnd, TB_BUTTONCOUNT, 0, 0);
    }

    int getButtonCommandId(int index) const{
        TBBUTTON tbButton;
        SendMessage(hwnd, TB_GETBUTTON, index, (LPARAM)&tbButton);
        return tbButton.idCommand;
    }

    operator HWND() const {
        return hwnd;
    }
private:
    HWND hwnd = 0;
};


