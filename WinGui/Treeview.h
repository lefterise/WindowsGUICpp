#pragma once
#include "Window.h"
#include "Font.h"
#include <commctrl.h>

class Treeview {
public:
    class Treeview (Window& window, int x, int y, int w, int h){
        hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, WC_TREEVIEWW, L"",
            WS_VISIBLE | WS_CHILD | TVS_HASLINES, x, y, w, h,
            window.hwnd, (HMENU)window.getNextId(), (HINSTANCE)GetWindowLongPtrW(window.hwnd, GWLP_HINSTANCE), NULL);               
    };

    HTREEITEM addItem(HTREEITEM parent, const wchar_t* text, int image, int selectedImage) {
        TVINSERTSTRUCTW tvis = { 0 };
        tvis.hParent = parent;
        tvis.hInsertAfter = TVI_ROOT;
        tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvis.item.pszText = (wchar_t*)text;
        tvis.item.iImage = image;
        tvis.item.iSelectedImage = selectedImage;
        HTREEITEM item = (HTREEITEM)SendMessageW(hwnd, TVM_INSERTITEMW, 0, (LPARAM)&tvis);        
        return item;
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
    HWND hwnd;
};