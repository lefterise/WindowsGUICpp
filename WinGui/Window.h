#pragma once
#include "WindowClass.h"
#include <exception>
#include <vector>
#include "Control.h"
//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa#remarks
//https://learn.microsoft.com/en-us/windows/win32/controls/common-control-window-classes
class Window {
public:
    Window(WindowClass& windowClass, const wchar_t* title, int width, int height, DWORD style = WS_OVERLAPPEDWINDOW);
    void show();
    void setOnDestroyAction(std::function<void()>&& action);
    size_t getNextId();    
    void setTitle(const char* text);
    void setHorizontalScrollbarRange(int min, int max);

    void setMenuCommand(size_t id, std::function<void(int e)>&& action);
    void setMessageHandler(UINT message, std::function<bool(WPARAM, LPARAM)>&& action);

    void setVisible(bool visible) {
        ShowWindow(hwnd, visible ? SW_SHOW : SW_HIDE);
    }

    operator HWND() const {
        return hwnd;
    }

    ~Window();
private:
    LRESULT windowHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    Application& app;
    WindowClass& windowClass;    
    std::map<size_t, std::function<void(int)>> commands;
    std::multimap<UINT, std::function<bool(WPARAM, LPARAM)>> messageHandlers;
    std::function<void()> onDestroy;
    size_t ids = 0;
public: 
    HWND hwnd;
};