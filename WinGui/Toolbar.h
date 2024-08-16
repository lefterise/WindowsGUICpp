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
            std::wstring caption;
            std::function<void(int e)> action;
            std::function<void(int x, int y)> menu;
        };

        ButtonBuilder(HWND hwnd, Window& window)
            : hwnd(hwnd)
            , window(window)
        {}

        ButtonBuilder& addButton(int imageId, Style style, std::wstring caption, std::function<void(int e)>&& action) {
            buttons.push_back({ imageId, (BYTE)style, std::move(caption), std::move(action), nullptr });
            return *this;
        }

        ButtonBuilder& addSeparator() {
            buttons.push_back({ 0, BTNS_SEP, L"", nullptr });
            return *this;
        }

        ButtonBuilder& addDropdownMenu(int imageId, std::wstring caption, std::function<void(int x, int y)> menuCallback) {
            buttons.push_back({ imageId, BTNS_DROPDOWN, std::move(caption), nullptr, std::move(menuCallback) });
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
                                SendMessageW(dropdown->hdr.hwndFrom, TB_GETRECT, (WPARAM)dropdown->iItem, (LPARAM)&rc);
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

            SendMessageW(hwnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
            SendMessageW(hwnd, TB_ADDBUTTONSW, (WPARAM)buttons.size(), (LPARAM)btns);//todo use W for unicode support
            free(btns);

            SendMessageW(hwnd, TB_AUTOSIZE, 0, 0);
        }

    private:
        std::vector<Button> buttons;
        HWND hwnd;
        Window& window;
    };


    Toolbar(Window& window, int x, int y, int w, int h)
    {

        hwnd = CreateWindowExW(
            0, TOOLBARCLASSNAMEW, NULL,
            WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS,
            x, y, w, h, window.hwnd, (HMENU)window.getNextId(),
            (HINSTANCE)GetWindowLongPtrW(window.hwnd, GWLP_HINSTANCE),
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

    void setImageList(HIMAGELIST hImageList) {
        SendMessage(hwnd, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
        SendMessageW(hwnd, TB_AUTOSIZE, 0, 0);
    }
    
    void changeIcon(int buttonId, int imageId) {
        TBBUTTONINFO tbbi;
        tbbi.cbSize = sizeof(TBBUTTONINFO);
        tbbi.dwMask = TBIF_IMAGE;
        tbbi.iImage = imageId;
        SendMessage(hwnd, TB_SETBUTTONINFO, (WPARAM)buttonId, (LPARAM)&tbbi);
    }

    void setFont(Font& font) {
        SendMessageW(hwnd, WM_SETFONT, (WPARAM)font.hFont, TRUE);
    }

    void setText(const wchar_t* text) {
        SetWindowTextW(hwnd, text);
    }

    int getButtonCount() const {
        return SendMessageW(hwnd, TB_BUTTONCOUNT, 0, 0);
    }

    int getButtonCommandId(int index) const{
        TBBUTTON tbButton;
        SendMessageW(hwnd, TB_GETBUTTON, index, (LPARAM)&tbButton);
        return tbButton.idCommand;
    }

    operator HWND() const {
        return hwnd;
    }
private:
    HWND hwnd = 0;
};


