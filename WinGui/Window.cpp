#include "Window.h"
#include "Button.h"
#include "Listbox.h"
#include "Textbox.h"
#include "Combobox.h"
#include "Progressbar.h"
#include "Slider.h"
#include "Label.h"
#include "Font.h"

//https://learn.microsoft.com/en-us/windows/win32/winmsg/windowing

Window::Window(WindowClass& windowClass, std::wstring title, int width, int height)
: app(app)
, windowClass(windowClass)
{
    auto handler = [this](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return windowHandler(hwnd, uMsg, wParam, lParam); };

    Application::setCreateHandler(handler);
    hwnd = CreateWindowExW(
        WS_EX_WINDOWEDGE,
        windowClass.getClassName(),
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,
        NULL,
        windowClass.getInstanceHandle(),
        NULL
    );
    Application::setCreateHandler(nullptr);

    if (hwnd == NULL) {
        throw std::exception("Window creation failed");
    }

    Application::registerWindowHandler(hwnd, handler);
}

void Window::show() {
    ShowWindow(hwnd, SW_SHOWNORMAL);
}

void Window::setOnDestroyAction(std::function<void()>&& action) {
    onDestroy = std::move(action);
}

size_t Window::getNextId() {
    return ++ids;
}

void Window::setHorizontalScrollbarRange(int min, int max) {
    SetScrollRange(hwnd, SB_HORZ, min, max, TRUE);
}

void Window::setTitle(const char* text) {
    SetWindowTextA(hwnd, text);
}

void Window::setMenuCommand(size_t id, std::function<void(int e)>&& action) {
    commands[id] = std::move(action);
}

void Window::setMessageHandler(HWND hwnd, UINT message, std::function<void(WPARAM e)>&& action) {
    messageHandlers[std::make_pair(hwnd, message)] = std::move(action);
}

LRESULT Window::windowHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
    {
        break;
    }
    case WM_COMMAND:
    {
        int commandId = LOWORD(wParam);
        int notificationCode = HIWORD(wParam);
        auto it = commands.find(commandId);
        if (it != commands.end()) {
            it->second(notificationCode);
            return 0;
        }
        break;
    }

    case WM_DESTROY:
        if (onDestroy) {
            onDestroy();
        }
        return 0;
    }

    auto itMessageHandler = messageHandlers.find(std::make_pair((HWND)lParam, uMsg));
    if (itMessageHandler != messageHandlers.end()) {
        itMessageHandler->second(wParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}