#pragma once
#include "Window.h"
#include "Font.h"
#include <commctrl.h>

class Treeview {
public:
    class Treeview (Window& window, int x, int y, int w, int h, int style = TVS_HASLINES){
        hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, WC_TREEVIEWW, L"",
            WS_VISIBLE | WS_CHILD | style , x, y, w, h,
            window.hwnd, (HMENU)window.getNextId(), (HINSTANCE)GetWindowLongPtrW(window.hwnd, GWLP_HINSTANCE), NULL);               
    };

    HTREEITEM addItem(HTREEITEM parent, const wchar_t* text, int image, int selectedImage) {
        TVINSERTSTRUCTW tvi = { 0 };
        tvi.hParent = parent;
        tvi.hInsertAfter = TVI_ROOT;
        tvi.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvi.item.pszText = (wchar_t*)text;
        tvi.item.iImage = image;
        tvi.item.iSelectedImage = selectedImage;
        HTREEITEM item = (HTREEITEM)SendMessageW(hwnd, TVM_INSERTITEMW, 0, (LPARAM)&tvi);        
        return item;
    }

    void setItem(HTREEITEM item, const wchar_t* text, int image=-1, int selectedImage=-1) {
        TVITEMW tvi = { 0 };
        tvi.mask = TVIF_TEXT | (image != -1 ? TVIF_IMAGE : 0) | (selectedImage != -1 ? TVIF_SELECTEDIMAGE : 0);
        tvi.hItem = item;
        tvi.pszText = (LPWSTR)text; 
        tvi.iImage = image;
        tvi.iSelectedImage = selectedImage;
        SendMessageW(hwnd, TVM_SETITEMW, 0, (LPARAM)&tvi);
    }

    std::wstring getItemText(HTREEITEM item) {
        wchar_t buffer[256] = {0};
        TVITEMW tvi = { 0 };
        tvi.mask = TVIF_TEXT;
        tvi.hItem = item;
        tvi.pszText = buffer;
        tvi.cchTextMax = 256;

        int success = SendMessageW(hwnd, TVM_GETITEMW, 0, (LPARAM)&tvi);
        if (!success) throw std::runtime_error("Invalid item handle");
        return std::wstring(buffer);
    }

    HTREEITEM getSelectedItem() {
        HTREEITEM hSelectedItem = (HTREEITEM)SendMessageW(hwnd, TVM_GETNEXTITEM, TVGN_CARET, 0);    
        return hSelectedItem;
    }

    void expand(HTREEITEM hRootItem, bool value) {
        SendMessageW(hwnd, TVM_EXPAND, (WPARAM)(value ? TVE_EXPAND : TVE_COLLAPSE), (LPARAM)hRootItem);
    }

    void setImageList(HIMAGELIST hImageList) {
        SendMessageW(hwnd, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)hImageList);
    }

    void setVisible(bool visible) {
        ShowWindow(hwnd, visible ? SW_SHOW : SW_HIDE);
    }

    void setSelectionChangeListener(Window& window, std::function<void(LPNMTREEVIEW l)>&& callback) {
        window.setMessageHandler(WM_NOTIFY,
            [hwnd(this->hwnd), callback(std::move(callback))](WPARAM w, LPARAM l) -> bool {
                NMHDR* header = (NMHDR*)l;
                if (header->hwndFrom != hwnd) return false;
                if (header->code == TVN_SELCHANGEDW) {                   
                    callback((LPNMTREEVIEW)l);
                    return true;
                }
                return false;
            }
        );
    }

    void addEditLabelHandler(Window& window, std::function<void(NMTVDISPINFO*)>&& callback) {
        window.setMessageHandler(WM_NOTIFY,
            [hwnd(this->hwnd), callback(std::move(callback))](WPARAM w, LPARAM l) -> bool {
                NMHDR* header = (NMHDR*)l;
                if (header->hwndFrom != hwnd) return false;
                if (header->code == TVN_ENDLABELEDIT) {
                    callback((NMTVDISPINFO*)l);
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