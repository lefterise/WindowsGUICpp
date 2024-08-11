#pragma once
#include <Windows.h>
#include <string>

class DeviceContext {
public:
    /*
    Device context given from outside.
    Eg in the WM_PAINT event:
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    DeviceContext dc(hdc);
    dc.drawRect();
    EndPaint(hwnd, &ps);
    */
    DeviceContext(HDC hdc) 
        : hdc(hdc)
    {
    }

    /*
    Device context given from a component eg a window
    Use NULL to get the context of the screen
    DeviceContext dc(window.hwnd);
    */
    DeviceContext(HWND handle)
    : handle(handle)
    {
        hdc = GetDC(handle);
        wasCaptured = true;
    }
    
    /*
    Memory Device context, compatible with some other device context (eg has same bitdepth)
    */
    DeviceContext(HDC other, int w, int h) {
        handle = 0;
        hdc = CreateCompatibleDC(other);        
        hBitmap = CreateCompatibleBitmap(other, w, h);
        hOldBitmap = SelectObject(hdc, hBitmap);
        wasCreated = true;
    }
    
    void setTextBackgroundTransparent(bool transparent) {
        SetBkMode(hdc, transparent ? TRANSPARENT : OPAQUE);
    }

    void setPen(HPEN pen) {
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        if (originalPen == 0) originalPen = oldPen;
    }

    void setBrush(HBRUSH brush) {
        HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, brush);
        if (originalBrush == 0) originalBrush = oldbrush;
    }

    void setFont(HFONT font) {
        HFONT oldFont = (HFONT)SelectObject(hdc, font);
        if (originalFont == 0) originalFont = oldFont;
    }

    void drawRectangle(int left, int top, int right, int bottom) {
        Rectangle(hdc, left, top, right, bottom);
    }

    void drawText(int x, int y, std::wstring text) {
        TextOutW(hdc, x, y, text.c_str(), text.size());
    }

    void drawEllipse(int left, int top, int right, int bottom) {
        Ellipse(hdc, left, top, right, bottom);
    }

    void beginPath() {
        BeginPath(hdc);
    }

    void moveTo(int x, int y) {
        MoveToEx(hdc, x, y, NULL);
    }

    void lineTo(int x, int y) {
        LineTo(hdc, x, y);
    }

    void closeFigure() {
        CloseFigure(hdc);
    }

    void endPath() {
        EndPath(hdc);
    }

    void fillPath() {
        FillPath(hdc);
    }

    void strokePath() {
        StrokePath(hdc);
    }

    void bitblt(int xDest, int yDest, int width, int height, HDC src, int xSrc, int ySrc, DWORD rop = SRCCOPY) {
        BitBlt(hdc, xDest, yDest, width, height, src, xSrc, ySrc, rop);
    }

    ~DeviceContext() {
        if (originalPen != 0) {
            SelectObject(hdc, originalPen);
        }

        if (originalBrush != 0) {
            SelectObject(hdc, originalBrush);
        }

        if (originalFont != 0) {
            SelectObject(hdc, originalFont);
        }

        if (wasCreated) {
            SelectObject(hdc, hOldBitmap);
            DeleteObject(hBitmap);
            DeleteDC(hdc);
        }

        if (wasCaptured){
            ReleaseDC(handle, hdc);
        }
    }

    operator HDC() const{
        return hdc;
    }

    HPEN originalPen = 0;
    HBRUSH originalBrush = 0;
    HFONT originalFont = 0;
    HBITMAP hBitmap;
    HGDIOBJ hOldBitmap;
    HWND handle;
    HDC hdc;
    bool wasCreated = false;
    bool wasCaptured = false;
};