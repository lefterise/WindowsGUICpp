#include "Application.h"

void Application::run() {
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK Application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (doNotPrint.find(uMsg) == doNotPrint.end()) {
        std::cout << hwnd << " " << msgToString(uMsg) << " " << wParam << " " << lParam << std::endl;
    }

    auto itHandler = windowHandlers.find(hwnd);
    if (itHandler != windowHandlers.end()) {
        return itHandler->second(hwnd, uMsg, wParam, lParam);
    }
    else {
        if (createHandler) {
            return createHandler(hwnd, uMsg, wParam, lParam);
        }
        else {
            std::cout << "ERROR" << std::endl;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Application::registerWindowHandler(HWND window, std::function <LRESULT(HWND, UINT, WPARAM, LPARAM)>&& handler) {
    windowHandlers.emplace(window, std::move(handler));
}

void Application::setCreateHandler(std::function <LRESULT(HWND, UINT, WPARAM, LPARAM)>&& handler) {
    createHandler = std::move(handler);
}

std::map<HWND, std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>> Application::windowHandlers;
std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> Application::createHandler;