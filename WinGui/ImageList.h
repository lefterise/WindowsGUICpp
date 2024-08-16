#pragma once
#include "Window.h"
#include <commctrl.h>
class ImageList {
public:
    ImageList(int w, int h) {
        hImageList = ImageList_Create(w, h, ILC_COLOR32 | ILC_MASK, 0, 3);
    }

    void addBitmap(const wchar_t* path, int transparentColor) {
        HBITMAP hBitmapFile = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        ImageList_AddMasked(hImageList, hBitmapFile, NULL);
        DeleteObject(hBitmapFile);
    }

    void addBitmap(HINSTANCE instance, int resourceId, int transparentColor) {
        HBITMAP hBitmapRes = LoadBitmap(instance, MAKEINTRESOURCE(resourceId));
        ImageList_AddMasked(hImageList, hBitmapRes, transparentColor);
        DeleteObject(hBitmapRes);
    }

    void addIcon(const wchar_t* path, int w, int h) {
        HICON hIconFile = (HICON)LoadImage(NULL, path, IMAGE_ICON, w, h, LR_LOADFROMFILE);
        ImageList_AddIcon(hImageList, hIconFile);
        DestroyIcon(hIconFile);
    }

    void addIcon(HINSTANCE instance, int resourceId) {
        HICON hIconRes = LoadIcon(instance, MAKEINTRESOURCE(resourceId));
        ImageList_AddIcon(hImageList, hIconRes);
        DestroyIcon(hIconRes);
    }

    operator HIMAGELIST() {
        return hImageList;
    }

    HIMAGELIST hImageList;
};