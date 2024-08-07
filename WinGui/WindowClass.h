#pragma once
#include "Application.h"
#include <string>

class WindowClass {
public:
    WindowClass(const wchar_t* className)
    : className(className){
        wc.lpfnWndProc = Application::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = className;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);

        RegisterClassW(&wc);
    }

    const wchar_t* getClassName() const {
        return className.c_str();
    }

    HINSTANCE getInstanceHandle() const {
        return wc.hInstance;
    }
    
    const std::wstring className;
    WNDCLASSW wc{};    
};