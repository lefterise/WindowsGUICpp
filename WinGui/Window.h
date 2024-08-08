#pragma once
#include "WindowClass.h"
#include <exception>
#include <vector>
#include "Control.h"
//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa#remarks

class Window {
public:
    Window(WindowClass& windowClass, std::wstring title, int width, int height);
    void show();
    void setOnDestroyAction(std::function<void()>&& action);
    size_t getNextId();    
    void setTitle(const char* text);
    void setHorizontalScrollbarRange(int min, int max);

    void setMenuCommand(size_t id, std::function<void(int e)>&& action);
    void setMessageHandler(HWND hwnd, UINT message, std::function<void(WPARAM e)>&& action);

private:
    LRESULT windowHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    Application& app;
    WindowClass& windowClass;    
    std::map<size_t, std::function<void(int)>> commands;
    std::map<std::pair<HWND, UINT>, std::function<void(WPARAM)>> messageHandlers;
    std::function<void()> onDestroy;
    size_t ids = 0;
public: 
    HWND hwnd;
};