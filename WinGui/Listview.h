#pragma once
#include "Window.h"
#include "Font.h"
#include <commctrl.h>

class Listview {
public:
    class Listview(Window& window, int x, int y, int w, int h, int style = LVS_ICON) {
        hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, WC_LISTVIEWW, L"",
            WS_VISIBLE | WS_CHILD | style,
            x, y, w, h, window.hwnd, (HMENU)window.getNextId(), (HINSTANCE)GetWindowLongPtrW(window.hwnd, GWLP_HINSTANCE), NULL);        
    };

    void setImageList(HIMAGELIST hImageList, int type = LVSIL_NORMAL) {
        SendMessageW(hwnd, LVM_SETIMAGELIST, type, (LPARAM)hImageList);
    }

    void setVisible(bool visible) {
        ShowWindow(hwnd, visible ? SW_SHOW : SW_HIDE);
    }

    void insertItem(const wchar_t* text, int image, int position, int column = 0) {
        LVITEMW lvItem;
        lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
        lvItem.iSubItem = column;

        lvItem.iItem = position;
        lvItem.pszText = (wchar_t*)text;
        lvItem.iImage = image;

        SendMessageW(hwnd, LVM_INSERTITEM, 0, (LPARAM)(const LV_ITEM*)(&lvItem));
    }

    void setItem(const wchar_t* text, int image, int position, int column = 0) {
        LVITEMW lvItem;
        lvItem.mask = LVIF_TEXT | (image != -1 ? LVIF_IMAGE : 0);
        lvItem.iSubItem = column;

        lvItem.iItem = position;
        lvItem.pszText = (wchar_t*)text;
        lvItem.iImage = image;
        ListView_SetItem(hwnd, &lvItem);
    }

    void insertColumn(const wchar_t* text, int width, int column) {
        LVCOLUMN lvColumn;
        lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        lvColumn.pszText = (wchar_t*)text;
        lvColumn.cx = width;
        lvColumn.iSubItem = column;
        ListView_InsertColumn(hwnd, 0, &lvColumn);
    }

    int getNextSelectedItem(int fromIndex) {
        return (int)SendMessage(hwnd, LVM_GETNEXTITEM, fromIndex, LVNI_SELECTED);
    }

    std::wstring getItemText(int index, int column = 0) {
        wchar_t buffer[256];
        LVITEMW lvItem = { 0 };
        lvItem.iSubItem = column;
        lvItem.pszText = buffer;
        lvItem.cchTextMax = sizeof(buffer) / sizeof(buffer[0]);

        // Get the item text
        SendMessage(hwnd, LVM_GETITEMTEXT, index, (LPARAM)&lvItem);
        return std::wstring(buffer);

    }

    void addSelectionChangeHandler(Window& window, std::function<void(NMLISTVIEW*)>&& callback) {
        window.setMessageHandler(WM_NOTIFY,
            [hwnd(this->hwnd), callback(std::move(callback))](WPARAM w, LPARAM l) -> bool {
                NMHDR* header = (NMHDR*)l;
                if (header->hwndFrom != hwnd) return false;
                if (header->code == LVN_ITEMCHANGED) {
                    callback((NMLISTVIEW*)l);
                    return true;
                }
                return false;
            }
        );
    }

    void addEditLabelHandler(Window& window, std::function<void(NMLVDISPINFO*)>&& callback) {
        window.setMessageHandler(WM_NOTIFY,
            [hwnd(this->hwnd), callback(std::move(callback))](WPARAM w, LPARAM l) -> bool {
                NMHDR* header = (NMHDR*)l;
                if (header->hwndFrom != hwnd) return false;
                if (header->code == LVN_ENDLABELEDIT) {
                    callback((NMLVDISPINFO*)l);
                    return true;
                }
                return false;
            }
        );
    }

    operator HWND() const {
        return hwnd;
    }
    
    HWND hwnd;
};