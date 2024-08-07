#pragma once
#include <windows.h>
#include <map>
#include <functional>
#include <iostream>
#include "WindowsMessages.h"
#include <set>

static std::set<int> doNotPrint = {
WM_NCMOUSEMOVE,
WM_SETCURSOR,
WM_NCHITTEST,
WM_NCMOUSELEAVE,
WM_NCACTIVATE,
WM_ACTIVATE,
WM_ACTIVATEAPP,
WM_KILLFOCUS,
WM_IME_SETCONTEXT,
WM_IME_NOTIFY,
WM_MOUSEFIRST,
WM_MOVE,
WM_MOVING,
WM_WINDOWPOSCHANGING,
WM_GETMINMAXINFO,
WM_WINDOWPOSCHANGED,
WM_CAPTURECHANGED
};

class Application {
public:
    static void init();
    static void run();
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void registerWindowHandler(HWND window, std::function <LRESULT(HWND, UINT, WPARAM, LPARAM)>&& handler);
    static void setCreateHandler(std::function <LRESULT(HWND, UINT, WPARAM, LPARAM)>&& handler);

private:
    static std::map<HWND, std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>> windowHandlers;
    static std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> createHandler;
};

